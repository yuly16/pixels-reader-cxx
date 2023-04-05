//
// Created by yuly on 05.04.23.
//

#include "vector/DecimalColumnVector.h"

/**
 * The decimal column vector with precision and scale.
 * The values of this column vector are the unscaled integer value
 * of the decimal. For example, the unscaled value of 3.14, which is
 * of the type decimal(3,2), is 314. While the precision and scale
 * of this decimal are 3 and 2, respectively.
 *
 * <p><b>Note: it only supports short decimals with max precision
 * and scale 18.</b></p>
 *
 * Created at: 05/03/2022
 * Author: hank
 */

DecimalColumnVector::DecimalColumnVector(int precision, int scale) {
    DecimalColumnVector(VectorizedRowBatch::DEFAULT_SIZE, precision, scale);
}

DecimalColumnVector::DecimalColumnVector(int len, int precision, int scale): ColumnVector(len) {
    this->vector = new long[len];
    this->precision = precision;
    this->scale = scale;
    memoryUsage += (long) sizeof(long) * len;
}

void DecimalColumnVector::close() {
    if(!closed) {
        ColumnVector::close();
        delete[] vector;
        vector = nullptr;
    }
}

void DecimalColumnVector::print(int rowCount) {
//    throw InvalidArgumentException("not support print Decimalcolumnvector.");
    for(int i = 0; i < rowCount; i++) {
        std::cout<<vector[i]<<std::endl;
    }
}
DecimalColumnVector::~DecimalColumnVector() {
    if(!closed) {
        DecimalColumnVector::close();
    }
}

