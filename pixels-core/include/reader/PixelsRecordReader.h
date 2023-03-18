//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_PIXELSRECORDREADER_H
#define PIXELS_PIXELSRECORDREADER_H

#include "vector/VectorizedRowBatch.h"
#include "physical/RequestBatch.h"

class PixelsRecordReader {
public:
//    virtual int prepareBatch(int batchSize) = 0;
    virtual std::shared_ptr<VectorizedRowBatch> readBatch(int batchSize, bool reuse) = 0;



};
#endif //PIXELS_PIXELSRECORDREADER_H
