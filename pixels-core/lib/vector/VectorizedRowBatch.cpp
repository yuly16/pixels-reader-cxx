//
// Created by liyu on 3/7/23.
//

#include "vector/VectorizedRowBatch.h"

/*
 * This number is carefully chosen to minimize overhead and typically allows
 * one VectorizedRowBatch to fit in cache.
 */
int VectorizedRowBatch::DEFAULT_SIZE = 1024;

VectorizedRowBatch::VectorizedRowBatch(int nCols, int size) {
    numCols = nCols;
    size = 0;
    maxSize = size;
    cols.clear();
    cols.resize(numCols);

}
