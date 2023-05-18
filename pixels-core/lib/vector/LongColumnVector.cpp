//
// Created by liyu on 3/17/23.
//

#include "vector/LongColumnVector.h"

LongColumnVector::LongColumnVector(int len): ColumnVector(len) {
    longVector = nullptr;
	intVector = nullptr;
    memoryUsage += (long) sizeof(long) * len;

}

void LongColumnVector::close() {
	if(!closed) {
		ColumnVector::close();
		longVector = nullptr;
		intVector = nullptr;
	}
}

void LongColumnVector::print(int rowCount) {
//	throw InvalidArgumentException("not support print longcolumnvector.");
    for(int i = 0; i < rowCount; i++) {
        std::cout<<longVector[i]<<std::endl;
		std::cout<<intVector[i]<<std::endl;
    }
}
LongColumnVector::~LongColumnVector() {
	if(!closed) {
		LongColumnVector::close();
	}
}
