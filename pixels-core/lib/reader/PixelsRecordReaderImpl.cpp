//
// Created by liyu on 3/7/23.
//

#include "reader/PixelsRecordReaderImpl.h"

PixelsRecordReaderImpl::PixelsRecordReaderImpl(PhysicalReader *reader,
                                               const pixels::proto::PostScript& pixelsPostScript,
                                               const pixels::proto::Footer& pixelsFooter,
                                               const PixelsReaderOption& opt,
                                               const PixelsFooterCache& pixelsFooterCache) {
    physicalReader = reader;
    footer = pixelsFooter;
    postScript = pixelsPostScript;
    footerCache = pixelsFooterCache;
    option = opt;
    // TODO: intialize all kinds of variables
    queryId = option.getQueryId();
    RGStart = option.getRGStart();
    RGLen = option.getRGLen();
    everRead = false;
    targetRGNum = 0;
    curRGIdx = 0;
    curRowInRG = 0;
    chunkBuffers = nullptr;
    fileName = physicalReader->getName();
    rowGroupFooters = nullptr;
}




VectorizedRowBatch PixelsRecordReaderImpl::readBatch(int batchSize, bool reuse) {
    VectorizedRowBatch resultRowBatch;
    read();
//    if(!everRead) {
//        if(!read()) {
//            throw std::runtime_error("failed to read file");
//        }
//    }

    return resultRowBatch;
}

void PixelsRecordReaderImpl::prepareRead() {
    // TODO: finish the remaining part of this function
    auto rowGroupStatistics = footer.rowgroupstats();
    std::vector<bool> includedRGs;
    includedRGs.reserve(RGLen);

    uint64_t includedRowNum = 0;
    // read row group statistics and find target row groups
    for(int i = 0; i < RGLen; i++) {
        includedRGs.emplace_back(true);
        includedRowNum += footer.rowgroupinfos(RGStart + i).numberofrows();
    }

    targetRGs.clear();
    targetRGs.reserve(RGLen);
    int targetRGIdx = 0;
    for(int i = 0; i < RGLen; i++) {
        if(includedRGs[i]) {
            targetRGs.emplace_back(i + RGStart);
            targetRGIdx++;
        }
    }
    targetRGNum = targetRGIdx;

    // TODO: if taregetRGNum == 0

    // read row group footers
    delete[] rowGroupFooters;
    rowGroupFooters = new pixels::proto::RowGroupFooter[targetRGNum];

    /**
     * Issue #114:
     * Use request batch and read scheduler to execute the read requests.
     *
     * Here, we create an empty batch as footer cache is very likely to be hit in
     * the subsequent queries on the same table.
     */
    RequestBatch requestBatch;
    std::vector<int> fis;
    std::vector<std::string> rgCacheIds;
    for(int i = 0; i < targetRGNum; i++) {
        int rgId = targetRGs[i];
        std::string rgCacheId = fileName + "-" + std::to_string(rgId);
        rgCacheIds.emplace_back(rgCacheId);
        if(footerCache.containsRGFooter(rgCacheId)) {
            // cache hit
            pixels::proto::RowGroupFooter rowGroupFooter = footerCache.getRGFooter(rgCacheId);
            rowGroupFooters[i] = rowGroupFooter;
        } else {
            // cache miss, read from disk and put it into cache
            pixels::proto::RowGroupInformation rowGroupInformation = footer.rowgroupinfos(rgId);
            uint64_t footerOffset = rowGroupInformation.footeroffset();
            uint64_t footerLength = rowGroupInformation.footerlength();
            fis.push_back(i);
            requestBatch.add(queryId, (int) footerOffset, (int) footerLength);
        }


    }
    Scheduler * scheduler = SchedulerFactory::Instance()->getScheduler();
    auto bbs = scheduler->executeBatch(physicalReader, requestBatch, queryId);
    // TODO: the return value should be unique_ptr?
    for(int i = 0; i < bbs.size(); i++) {
        pixels::proto::RowGroupFooter parsed;
        parsed.ParseFromArray(bbs[i]->getPointer(), (int)bbs[i]->size());
        rowGroupFooters[fis[i]] = parsed;
        footerCache.putRGFooter(rgCacheIds[fis[i]], parsed);
    }

    bbs.clear();

}

bool PixelsRecordReaderImpl::read() {
    prepareRead();
    everRead = true;
    // TODO: change targetRGNum number to the correct one
    targetRGNum = 1;
    // TODO: this should remove later
    includedColumns.push_back(true);
    includedColumns.push_back(true);
    includedColumns.push_back(true);
    includedColumns.push_back(true);
    targetColumns.push_back(0);
    targetColumns.push_back(1);
    targetColumns.push_back(2);
    targetColumns.push_back(3);
    chunkBuffers = new ByteBuffer *[targetRGNum * includedColumns.size()];
    std::vector<ChunkId> diskChunks;
    diskChunks.reserve(targetRGNum * targetColumns.size());


    // TODO: support cache read

    // TODO: here we manually set the value of diskChunks. We should remove it later
    ChunkId chunk1 = ChunkId(0, 0, 0, 2);
    ChunkId chunk2 = ChunkId(0, 1, 0, 3);
    ChunkId chunk3 = ChunkId(0, 1, 1, 3);
    diskChunks.push_back(chunk1);
    diskChunks.push_back(chunk2);
    diskChunks.push_back(chunk3);

    if(!diskChunks.empty()) {
        RequestBatch requestBatch((int)diskChunks.size());
        Scheduler * scheduler = SchedulerFactory::Instance()->getScheduler();
        for(ChunkId chunk : diskChunks) {
            int rgIdx = chunk.rowGroupId;
            int numCols = (int)includedColumns.size();
            int colId = chunk.columnId;
            requestBatch.add(queryId, chunk.offset, (int)chunk.length);
        }
        std::vector<ByteBuffer *> byteBuffers =
                scheduler->executeBatch(physicalReader, requestBatch, queryId);
        for(int index = 0; index < diskChunks.size(); index++) {
            ChunkId chunk = diskChunks[index];
            ByteBuffer * bb = byteBuffers[index];
            int rgIdx = chunk.rowGroupId;
            int numCols = (int)includedColumns.size();
            int colId = chunk.columnId;
            if(bb != nullptr) {
                chunkBuffers[rgIdx * numCols + colId] = bb;
            }
        }
    }
    return true;

}

PixelsRecordReaderImpl::~PixelsRecordReaderImpl() {
    // TODO: chunkBuffers, physicalReader should be deleted?
}




