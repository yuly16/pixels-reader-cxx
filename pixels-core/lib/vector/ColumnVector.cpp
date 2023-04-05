//
// Created by liyu on 3/7/23.
//

#include "vector/ColumnVector.h"

ColumnVector::ColumnVector(int len) {
    writeIndex = 0;
    length = len;
    memoryUsage = len + sizeof(int) * 3 + 4;
	closed = false;
}

void ColumnVector::close() {
	if(!closed) {
		writeIndex = 0;
		closed = true;
		// TODO: reset other variables
	}
}

void ColumnVector::reset() {
    writeIndex = 0;
    // TODO: reset other variables
}

void ColumnVector::print() {
    throw InvalidArgumentException("This columnVector doesn't implement this function.");
}





