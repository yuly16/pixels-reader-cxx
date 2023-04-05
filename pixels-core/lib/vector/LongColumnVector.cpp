//
// Created by liyu on 3/17/23.
//

#include "vector/LongColumnVector.h"

LongColumnVector::LongColumnVector(int len): ColumnVector(len) {
    vector = new long[len];
    memoryUsage += (long) sizeof(long) * len;

}

void LongColumnVector::close() {
	if(!closed) {
		ColumnVector::close();
		delete[] vector;
		vector = nullptr;
	}
}

void LongColumnVector::print(int rowCount) {
//	throw InvalidArgumentException("not support print longcolumnvector.");
    for(int i = 0; i < rowCount; i++) {
        std::cout<<vector[i]<<std::endl;
    }
}
LongColumnVector::~LongColumnVector() {
	if(!closed) {
		LongColumnVector::close();
	}
}
