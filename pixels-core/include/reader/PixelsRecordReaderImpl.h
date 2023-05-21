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
#include "reader/ColumnReader.h"
#include "reader/ColumnReaderBuilder.h"
#include "profiler/TimeProfiler.h"

class ChunkId {
public:
    uint32_t rowGroupId;
    uint32_t columnId;
    uint64_t offset;
    uint64_t length;
    ChunkId() = default;
    ChunkId(int rgId, int cId, uint64_t off, uint64_t len) {
        rowGroupId = rgId;
        columnId = cId;
        offset = off;
        length = len;
    }
};

class PixelsRecordReaderImpl: public PixelsRecordReader {
public:
    explicit PixelsRecordReaderImpl(std::shared_ptr<PhysicalReader> reader,
                                    const pixels::proto::PostScript& pixelsPostScript,
                                    const pixels::proto::Footer& pixelsFooter,
                                    const PixelsReaderOption& opt,
                                    std::shared_ptr<PixelsFooterCache> pixelsFooterCache
                                    );
    void readBatch(duckdb::DataChunk &output, duckdb::vector<duckdb::column_t> column_ids) override;
	bool isEndOfFile() override;
    ~PixelsRecordReaderImpl();
	void close() override;
private:
    bool read();
    void prepareRead();
    void checkBeforeRead();
	void UpdateRowGroupInfo();
    std::shared_ptr<PhysicalReader> physicalReader;
    pixels::proto::Footer footer;
    pixels::proto::PostScript postScript;
	std::shared_ptr<PixelsFooterCache> footerCache;
    PixelsReaderOption option;
    long queryId;
    int RGStart;
    int RGLen;
    bool everRead;
    int targetRGNum;
    int curRGIdx;
    int curRowInRG;
	int curRowInStride;
    std::string fileName;
	bool endOfFile;
	int curRGRowCount;
	pixels::proto::RowGroupFooter curRGFooter;
	std::vector<pixels::proto::ColumnEncoding> curEncoding;
	std::vector<int> curChunkBufferIndex;
	std::vector<pixels::proto::ColumnChunkIndex> curChunkIndex;
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
    std::vector<std::shared_ptr<ByteBuffer>> chunkBuffers;
    // column readers for each target columns
    std::vector<std::shared_ptr<ColumnReader>> readers;
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
