//
// Created by liyu on 3/7/23.
//

#include "reader/PixelsRecordReaderImpl.h"

PixelsRecordReaderImpl::PixelsRecordReaderImpl(PhysicalReader *reader,
                                               const pixels::proto::PostScript& pixelsPostScript,
                                               const pixels::proto::Footer& pixelsFooter,
                                               const PixelsFooterCache& pixelsFooterCache) {
    physicalReader = reader;
    footer = pixelsFooter;
    postScript = pixelsPostScript;
    footerCache = pixelsFooterCache;
    // TODO: intialize all kinds of variables
    queryId = 0;
    everRead = false;
    targetRGNum = 0;
    curRGIdx = 0;
    curRowInRG = 0;
    chunkBuffers = nullptr;
}




VectorizedRowBatch PixelsRecordReaderImpl::readBatch(int batchSize, bool reuse) {
    VectorizedRowBatch resultRowBatch;
    if(!everRead) {
        if(!read()) {
            throw std::runtime_error("failed to read file");
        }
    }

    return resultRowBatch;
}

bool PixelsRecordReaderImpl::read() {
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


