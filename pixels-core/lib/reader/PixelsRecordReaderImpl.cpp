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
    fileName = physicalReader->getName();
    enableEncodedVector = option.isEnableEncodedColumnVector();
    includedColumnNum = 0;
    checkBeforeRead();
}

void PixelsRecordReaderImpl::checkBeforeRead() {
    // get file schema
    auto fileColTypesFooterType = footer.types();
    auto fileColTypes = std::vector<pixels::proto::Type>(
            fileColTypesFooterType.begin(),
            fileColTypesFooterType.end());
    // TODO: if fileCOlTypes == null
    fileSchema = std::make_shared<TypeDescription>
            (TypeDescription::createSchema(fileColTypes));
    // TODO: getChildren == NULL
    // filter included columns
    includedColumnNum = 0;
    auto optionIncludedCols = option.getIncludedCols();
    // TODO: if size of cols is 0, create an empty row batch
    // TODO: what if false is caused? we must debug this! Currently I didn't understand why we need includedColumns yet. So just leave it alone.
    includedColumns.clear();
    includedColumns.reserve(fileColTypes.size());
    std::vector<int> optionColsIndices;
    for(const auto& col: optionIncludedCols) {
        bool isIncluded = false;
        for(int j = 0; j < fileColTypes.size(); j ++) {
            if(icompare(col, fileColTypes.at(j).name())) {
                optionColsIndices.emplace_back(j);
                isIncluded = true;
                includedColumnNum++;
                break;
            }
        }
        includedColumns.emplace_back(isIncluded);
    }
    // TODO: check includedColumns
    // create result columns storing result column ids in user specified order
    resultColumns.clear();
    resultColumns.reserve(includedColumnNum);
    for(int i = 0; i < includedColumnNum; i++) {
        resultColumns.emplace_back(optionColsIndices[i]);
    }


    auto optionColsIndicesSet = std::set<int>(
            optionColsIndices.begin(), optionColsIndices.end());
    int targetColumnNum = (int)optionColsIndicesSet.size();
    targetColumns.clear();
    targetColumns.reserve(targetColumnNum);
    for(int i = 0; i < includedColumns.size(); i++) {
        if(includedColumns[i]) {
            targetColumns.emplace_back(i);
        }
    }

    // create column readers
    includedColumnTypes.clear();
    for(int resultColumn: resultColumns) {
        includedColumnTypes.emplace_back(fileColTypes.at(resultColumn));
    }
    resultSchema = std::make_shared<TypeDescription>(
            TypeDescription::createSchema(includedColumnTypes));

}



VectorizedRowBatch PixelsRecordReaderImpl::readBatch(int batchSize, bool reuse) {

    read();
//    if(!everRead) {
//        if(!read()) {
//            throw std::runtime_error("failed to read file");
//        }
//    }
    VectorizedRowBatch resultRowBatch;

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
    rowGroupFooters.clear();
    rowGroupFooters.resize(targetRGNum);

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
            rowGroupFooters.at(i) = rowGroupFooter;
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
        rowGroupFooters.at(fis[i]) = parsed;
        footerCache.putRGFooter(rgCacheIds[fis[i]], parsed);
    }

    bbs.clear();
    resultColumnsEncoded.clear();
    resultColumnsEncoded.resize(includedColumnNum);
    pixels::proto::RowGroupEncoding firstRgEncoding = rowGroupFooters[0].rowgroupencoding();
    for(int i = 0; i < includedColumnNum; i++) {
        resultColumnsEncoded.at(i) = firstRgEncoding
                .columnchunkencodings(targetColumns[i])
                .kind() != pixels::proto::ColumnEncoding_Kind_NONE &&
                enableEncodedVector;
        if(firstRgEncoding
                   .columnchunkencodings(targetColumns[i])
                   .kind() == pixels::proto::ColumnEncoding_Kind_NONE) {
            std::cout<<"caonima"<<std::endl;
        }
    }
}

bool PixelsRecordReaderImpl::read() {
    prepareRead();
    everRead = true;

    // read chunk offset and length of each target column chunks

    // TODO: this should remove later
    for(auto chunkBuffer: chunkBuffers) {
        delete chunkBuffer;
    }
    chunkBuffers.clear();
    chunkBuffers.resize(targetRGNum * includedColumns.size());
    std::vector<ChunkId> diskChunks;
    diskChunks.reserve(targetRGNum * targetColumns.size());


    // TODO: support cache read
    for(int rgIdx = 0; rgIdx < targetRGNum; rgIdx++) {
        pixels::proto::RowGroupIndex rowGroupIndex =
                rowGroupFooters[rgIdx].rowgroupindexentry();
        for(int colId: targetColumns) {
            const pixels::proto::ColumnChunkIndex& chunkIndex =
                    rowGroupIndex.columnchunkindexentries(colId);
            ChunkId chunk(rgIdx, colId, chunkIndex.chunkoffset(), chunkIndex.chunklength());
            diskChunks.emplace_back(chunk);
        }
    }

    if(!diskChunks.empty()) {
        RequestBatch requestBatch((int)diskChunks.size());
        Scheduler * scheduler = SchedulerFactory::Instance()->getScheduler();
        for(ChunkId chunk : diskChunks) {
            requestBatch.add(queryId, chunk.offset, (int)chunk.length);
        }
        std::vector<ByteBuffer *> byteBuffers =
                scheduler->executeBatch(physicalReader, requestBatch, queryId);
        for(int index = 0; index < diskChunks.size(); index++) {
            ChunkId chunk = diskChunks[index];
            ByteBuffer * bb = byteBuffers[index];
            uint32_t rgIdx = chunk.rowGroupId;
            uint32_t numCols = includedColumns.size();
            uint32_t colId = chunk.columnId;
            if(bb != nullptr) {
                chunkBuffers.at(rgIdx * numCols + colId) = bb;
            }
        }
    }
    return true;

}

PixelsRecordReaderImpl::~PixelsRecordReaderImpl() {
    // TODO: chunkBuffers, physicalReader should be deleted?
}





