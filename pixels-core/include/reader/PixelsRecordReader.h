//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_PIXELSRECORDREADER_H
#define PIXELS_PIXELSRECORDREADER_H

#include "vector/VectorizedRowBatch.h"
#include "physical/RequestBatch.h"

class PixelsRecordReader {
    /**
     * Prepare for the next row batch. This method is independent from readBatch().
     *
     * @param batchSize the willing batch size
     * @return the real batch size
     */
//    virtual int prepareBatch(int batchSize) = 0;
    virtual VectorizedRowBatch readBatch(int batchSize, bool reuse) = 0;



};
#endif //PIXELS_PIXELSRECORDREADER_H
