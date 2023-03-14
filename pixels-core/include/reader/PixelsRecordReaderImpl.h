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
                                    const PixelsFooterCache& pixelsFooterCache
                                    );
    VectorizedRowBatch readBatch(int batchSize, bool reuse) override;
private:
    PhysicalReader * physicalReader;
    pixels::proto::Footer footer;
    pixels::proto::PostScript postScript;
    PixelsFooterCache footerCache;
    long queryId;
    bool everRead;
    int targetRGNum;
    int curRGIdx;
    int curRowInRG;
    /**
     * Columns included by reader option; if included, set true
     */
    std::vector<bool> includedColumns;
    bool read();
    // buffers of each chunk in this file, arranged by chunk's row group id and column id
    ByteBuffer ** chunkBuffers;
    std::vector<int> targetColumns;

};
#endif //PIXELS_PIXELSRECORDREADERIMPL_H
