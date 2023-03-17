//
// Created by liyu on 3/17/23.
//

#ifndef PIXELS_TEMPLATE_H
#define PIXELS_TEMPLATE_H

#include "vector/ColumnVector.h"
#include "vector/VectorizedRowBatch.h"

class ByteColumnVector: public ColumnVector {
public:
    uint8_t * vector;

    /**
    * Use this constructor by default. All column vectors
    * should normally be the default size.
    */
    ByteColumnVector(int len = VectorizedRowBatch::DEFAULT_SIZE);

    void close();
};
#endif //PIXELS_TEMPLATE_H
