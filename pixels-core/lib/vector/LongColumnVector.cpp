//
// Created by liyu on 3/17/23.
//

#include "vector/LongColumnVector.h"

LongColumnVector::LongColumnVector(int len): ColumnVector(len) {
    vector = new long[len];
    memoryUsage += (long) sizeof(long) * len;
    size = len;
}

void LongColumnVector::close() {
    ColumnVector::close();
    delete vector;
    vector = nullptr;
}

void LongColumnVector::print() {
    for(int i = 0; i < size; i++) {
        std::cout<<vector[i]<<std::endl;
    }
}
