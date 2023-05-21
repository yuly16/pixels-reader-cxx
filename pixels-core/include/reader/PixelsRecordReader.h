//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_PIXELSRECORDREADER_H
#define PIXELS_PIXELSRECORDREADER_H

#include "vector/VectorizedRowBatch.h"
#include "physical/RequestBatch.h"
#include "TypeDescription.h"
#include "duckdb/common/types/chunk_collection.hpp"
#include "duckdb.h"

class PixelsRecordReader {
public:
	//    virtual int prepareBatch(int batchSize) = 0;
    virtual void readBatch(duckdb::DataChunk &output, duckdb::vector<duckdb::column_t> column_ids) = 0;


	/**
     * Get the schema of the included columns in the read option.
     *
     * @return result schema, null if PixelsRecordReader is not initialized successfully.
	 */

	virtual bool isEndOfFile() = 0;

	virtual void close() = 0;
};
#endif //PIXELS_PIXELSRECORDREADER_H
