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
    rowIndex = 0L;
    checkBeforeRead();
}

void PixelsRecordReaderImpl::checkBeforeRead() {
    // get file schema
    auto fileColTypesFooterTypes = footer.types();
    auto fileColTypes = std::vector<pixels::proto::Type>{};
    for(const auto& type : fileColTypesFooterTypes) {
        fileColTypes.emplace_back(type);
    }
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
    includedColumns.resize(fileColTypes.size());
    std::vector<int> optionColsIndices;
    for(const auto& col: optionIncludedCols) {
        for(int j = 0; j < fileColTypes.size(); j ++) {
            if(icompare(col, fileColTypes.at(j).name())) {
                optionColsIndices.emplace_back(j);
                includedColumns.at(j) = true;
                includedColumnNum++;
                break;
            }
        }

    }
    // TODO: check includedColumns
    // create result columns storing result column ids in user specified order
    resultColumns.clear();
    resultColumns.resize(includedColumnNum);
    for(int i = 0; i < includedColumnNum; i++) {
        resultColumns.at(i) = optionColsIndices[i];
    }


    auto optionColsIndicesSet = std::set<int>(
            optionColsIndices.begin(), optionColsIndices.end());
    int targetColumnNum = (int)optionColsIndicesSet.size();
    targetColumns.clear();
    targetColumns.resize(targetColumnNum);
    int targetColIdx = 0;
    for(int i = 0; i < includedColumns.size(); i++) {
        if(includedColumns[i]) {
            targetColumns.at(targetColIdx) = i;
            targetColIdx++;
        }
    }

    // create column readers
    auto columnSchemas = fileSchema->getChildren();
    readers.clear();
    readers.resize(resultColumns.size());
    for(int i = 0; i < resultColumns.size(); i++) {
        int index = resultColumns[i];
        readers.at(i) = ColumnReaderBuilder::newColumnReader(columnSchemas.at(index));
    }

    // create result vectorized row batch
    for(int resultColumn: resultColumns) {
        includedColumnTypes.emplace_back(fileColTypes.at(resultColumn));
    }
    resultSchema = std::make_shared<TypeDescription>(
            TypeDescription::createSchema(includedColumnTypes));

}



std::shared_ptr<VectorizedRowBatch> PixelsRecordReaderImpl::readBatch(int batchSize, bool reuse) {

    read();
//    if(!everRead) {
//        if(!read()) {
//            throw std::runtime_error("failed to read file");
//        }
//    }
    std::shared_ptr<VectorizedRowBatch> resultRowBatch;
    resultRowBatch = resultSchema->createRowBatch(batchSize, resultColumnsEncoded);
    // TODO: resultRowBatch.projectionSize

    int rgRowCount = 0;
    int curBatchSize = 0;
    auto columnVectors = resultRowBatch->cols;
    if(curRGIdx < targetRGNum) {
        rgRowCount = (int) footer.rowgroupinfos(
                targetRGs.at(curRGIdx)).numberofrows();
    }

    while (resultRowBatch->rowCount < batchSize && curRowInRG < rgRowCount) {
        // update current batch size
        curBatchSize = rgRowCount - curRowInRG;
        if (curBatchSize + resultRowBatch->rowCount >= batchSize)
        {
            curBatchSize = batchSize - resultRowBatch->rowCount;
        }

        // read vectors
        for(int i = 0; i < resultColumns.size(); i++) {
            // TODO: if !columnVectors[i].duplicate
            pixels::proto::RowGroupFooter rowGroupFooter = rowGroupFooters.at(curRGIdx);
            pixels::proto::ColumnEncoding encoding = rowGroupFooter.rowgroupencoding()
                    .columnchunkencodings(resultColumns.at(i));
            int index = curRGIdx * includedColumns.size() + resultColumns.at(i);
            pixels::proto::ColumnChunkIndex chunkIndex = rowGroupFooter.rowgroupindexentry()
                    .columnchunkindexentries(resultColumns.at(i));
            readers.at(i)->read(chunkBuffers.at(index), encoding, curRowInRG, curBatchSize,
                                postScript.pixelstride(), resultRowBatch->rowCount,
                                columnVectors.at(i), chunkIndex);
        }

        // update current row index in the row group
        curRowInRG += curBatchSize;
        rowIndex += curBatchSize;
        resultRowBatch->rowCount += curBatchSize;
        // update row group index if current row index exceeds max row count in the row group
        if(curRowInRG >= rgRowCount) {
            curRGIdx++;
            if(curRGIdx < targetRGNum) {
                // if not end of file, update row count
                rgRowCount = (int) footer.rowgroupinfos(targetRGs.at(curRGIdx)).numberofrows();
                // refresh resultColumnsEncoded for reading the column vectors in the next row group.
                pixels::proto::RowGroupEncoding rgEncoding = rowGroupFooters.at(curRGIdx).rowgroupencoding();
                for(int i = 0; i < includedColumnNum; i++) {
                    resultColumnsEncoded.at(i) =
                            rgEncoding.columnchunkencodings(targetColumns.at(i))
                            .kind() != pixels::proto::ColumnEncoding_Kind_NONE
                            && enableEncodedVector;
                }
            } else {
                // if end of file, set result vectorized row batch endOfFile
                break;
            }
            curRowInRG = 0;
        }
    }
    return resultRowBatch;
}

void PixelsRecordReaderImpl::prepareRead() {
    // TODO: finish the remaining part of this function
    auto rowGroupStatistics = footer.rowgroupstats();
    std::vector<bool> includedRGs;
    includedRGs.resize(RGLen);

    uint64_t includedRowNum = 0;
    // read row group statistics and find target row groups
    for(int i = 0; i < RGLen; i++) {
        includedRGs.at(i) = true;
        includedRowNum += footer.rowgroupinfos(RGStart + i).numberofrows();
    }

    targetRGs.clear();
    targetRGs.resize(RGLen);
    int targetRGIdx = 0;
    for(int i = 0; i < RGLen; i++) {
        if(includedRGs[i]) {
            targetRGs.at(targetRGIdx) = i + RGStart;
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
    pixels::proto::RowGroupEncoding firstRgEncoding = rowGroupFooters.at(0).rowgroupencoding();
    for(int i = 0; i < includedColumnNum; i++) {
        resultColumnsEncoded.at(i) = firstRgEncoding
                .columnchunkencodings(targetColumns[i])
                .kind() != pixels::proto::ColumnEncoding_Kind_NONE &&
                enableEncodedVector;
    }
}

bool PixelsRecordReaderImpl::read() {
    prepareRead();
    everRead = true;

    // read chunk offset and length of each target column chunks

    // TODO: this should remove later
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
        std::vector<std::shared_ptr<ByteBuffer>> byteBuffers =
                scheduler->executeBatch(physicalReader, requestBatch, queryId);
        for(int index = 0; index < diskChunks.size(); index++) {
            ChunkId chunk = diskChunks[index];
            std::shared_ptr<ByteBuffer> bb = byteBuffers[index];
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





