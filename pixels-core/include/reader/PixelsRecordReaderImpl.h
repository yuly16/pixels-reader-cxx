//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_PIXELSRECORDREADERIMPL_H
#define PIXELS_PIXELSRECORDREADERIMPL_H

#include "PixelsRecordReader.h"
#include "physical/PhysicalReader.h"
#include "vector/VectorizedRowBatch.h"
#include "physical/Scheduler.h"
#include "physical/SchedulerFactory.h"
#include "pixels-common/pixels.pb.h"
#include "PixelsFooterCache.h"
#include "reader/PixelsReaderOption.h"
#include "utils/String.h"
#include "TypeDescription.h"

class ChunkId {
public:
    uint32_t rowGroupId;
    uint32_t columnId;
    uint64_t offset;
    uint64_t length;

    ChunkId(int rgId, int cId, uint64_t off, uint64_t len) {
        rowGroupId = rgId;
        columnId = cId;
        offset = off;
        length = len;
    }
};

class PixelsRecordReaderImpl: public PixelsRecordReader {
public:
    explicit PixelsRecordReaderImpl(PhysicalReader *reader,
                                    const pixels::proto::PostScript& pixelsPostScript,
                                    const pixels::proto::Footer& pixelsFooter,
                                    const PixelsReaderOption& opt,
                                    const PixelsFooterCache& pixelsFooterCache
                                    );
    std::shared_ptr<VectorizedRowBatch> readBatch(int batchSize, bool reuse) override;
    ~PixelsRecordReaderImpl();
private:
    bool read();
    void prepareRead();
    void checkBeforeRead();
    PhysicalReader * physicalReader;
    pixels::proto::Footer footer;
    pixels::proto::PostScript postScript;
    PixelsFooterCache footerCache;
    PixelsReaderOption option;
    long queryId;
    int RGStart;
    int RGLen;
    bool everRead;
    int targetRGNum;
    int curRGIdx;
    int curRowInRG;
    std::string fileName;
    /**
     * Columns included by reader option; if included, set true
     */
    std::vector<bool> includedColumns;
    /**
     * Target row groups to read after matching reader option,
     * each element represents a row group id.
     */
    std::vector<int> targetRGs;

    // buffers of each chunk in this file, arranged by chunk's row group id and column id
    std::vector<ByteBuffer *> chunkBuffers;
    // column readers for each target columns
    std::vector<std::shared_ptr<ColumnVector>> readers;
    std::vector<int> targetColumns;
    std::vector<int> resultColumns;
    std::vector<bool> resultColumnsEncoded;
    bool enableEncodedVector;
    std::vector<pixels::proto::RowGroupFooter> rowGroupFooters;

    int includedColumnNum; // the number of columns to read
    std::vector<pixels::proto::Type> includedColumnTypes;

    std::shared_ptr<TypeDescription> fileSchema;
    std::shared_ptr<TypeDescription> resultSchema;
};
#endif //PIXELS_PIXELSRECORDREADERIMPL_H
