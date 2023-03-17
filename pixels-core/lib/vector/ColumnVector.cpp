//
// Created by liyu on 3/7/23.
//

#include "vector/ColumnVector.h"

ColumnVector::ColumnVector(int len) {
    writeIndex = 0;
    length = len;
    memoryUsage = len + sizeof(int) * 3 + 4;
    std::cout<<"base"<<std::endl;

}

void ColumnVector::close() {
    writeIndex = 0;
    // TODO: reset other variables
}



