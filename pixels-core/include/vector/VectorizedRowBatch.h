//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_VECTORIZEDROWBATCH_H
#define PIXELS_VECTORIZEDROWBATCH_H

/**
 * VectorizedRowBatch derived from org.apache.hadoop.hive.ql.exec.vector
 * <p>
 * A VectorizedRowBatch is a set of rows, organized with each column
 * as a vector. It is the unit of query execution, organized to minimize
 * the cost per row and achieve high cycles-per-instruction.
 * The major fields are public by design to allow fast and convenient
 * access by the vectorized query execution code.
 */

#include <iostream>
#include <vector>
#include "vector/ColumnVector.h"

class VectorizedRowBatch {
public:
    int numCols;                      // number of columns
    std::vector<ColumnVector> cols;   // a vector for each column

};
#endif //PIXELS_VECTORIZEDROWBATCH_H
