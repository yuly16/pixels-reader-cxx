//
// Created by liyu on 3/17/23.
//

#ifndef PIXELS_BINARYCOLUMNVECTOR_H
#define PIXELS_BINARYCOLUMNVECTOR_H

#include "vector/ColumnVector.h"
#include "vector/VectorizedRowBatch.h"

class BinaryColumnVector: public ColumnVector {
public:
    uint8_t * vector;

    /**
    * Use this constructor by default. All column vectors
    * should normally be the default size.
    */
    BinaryColumnVector(int len = VectorizedRowBatch::DEFAULT_SIZE);

    void close() override;
};
#endif //PIXELS_BINARYCOLUMNVECTOR_H
