//
// Created by liyu on 3/17/23.
//

#include "vector/LongColumnVector.h"

LongColumnVector::LongColumnVector(int len): ColumnVector(len) {
    vector = new long[len];
    memoryUsage += (long) sizeof(long) * len;
    std::cout<<"long"<<std::endl;
}

void LongColumnVector::close() {
    ColumnVector::close();
    delete vector;
    vector = nullptr;
}