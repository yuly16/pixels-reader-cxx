//
// Created by liyu on 3/17/23.
//

#include "vector/BinaryColumnVector.h"

BinaryColumnVector::BinaryColumnVector(int len, bool encoding): ColumnVector(len, encoding) {
    vector = new duckdb::string_t[len];
    memoryUsage += (long) sizeof(uint8_t) * len;
}

void BinaryColumnVector::close() {
	if(!closed) {
		ColumnVector::close();
		delete[] vector;
		vector = nullptr;
	}
}

void BinaryColumnVector::setRef(int elementNum, uint8_t * const &sourceBuf, int start, int length) {
    if(elementNum >= writeIndex) {
        writeIndex = elementNum + 1;
    }
    this->vector[elementNum] = duckdb::string_t((const char *)sourceBuf + start, length);
}

void BinaryColumnVector::print(int rowCount) {
//	throw InvalidArgumentException("not support print binarycolumnvector.");
    for(int i = 0; i < rowCount; i++) {
        auto str = this->vector[i].GetData();
        int len = this->vector[i].GetSize();
        for(int j = 0; j < len; j++) {
            std::cout<<(char)(*(str + j));
        }
        std::cout<<std::endl;
    }
}
BinaryColumnVector::~BinaryColumnVector() {
	if(!closed) {
		BinaryColumnVector::close();
	}
}
