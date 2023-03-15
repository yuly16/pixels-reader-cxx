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
//#include <linux/io_uring.h>

class ChunkId {
public:
    int rowGroupId;
    int columnId;
    long offset;
    long length;

    ChunkId(int rgId, int cId, long off, long len) {
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
    VectorizedRowBatch readBatch(int batchSize, bool reuse) override;
    ~PixelsRecordReaderImpl();
private:
    bool read();
    void prepareRead();
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
    ByteBuffer ** chunkBuffers;
    std::vector<int> targetColumns;


    pixels::proto::RowGroupFooter * rowGroupFooters;

};
#endif //PIXELS_PIXELSRECORDREADERIMPL_H
