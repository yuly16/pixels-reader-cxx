//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_COLUMNVECTOR_H
#define PIXELS_COLUMNVECTOR_H

/**
 * ColumnVector derived from org.apache.hadoop.hive.ql.exec.vector.
 * <p>
 * ColumnVector contains the shared structure for the sub-types,
 * including NULL information, and whether this vector
 * repeats, i.e. has all values the same, so only the first
 * one is set. This is used to accelerate query performance
 * by handling a whole vector in O(1) time when applicable.
 * <p>
 * The fields are public by design since this is a performance-critical
 * structure that is used in the inner loop of query execution.
 */

#include <iostream>

class ColumnVector {
public:
    /**
      * length is the capacity, i.e., maximum number of values, of this column vector
      * <b>DO NOT</b> modify it or used it as the number of values in-used.
      */
    int length;
    int writeIndex;
    long memoryUsage;

    ColumnVector() = default;
    explicit ColumnVector(int len);
    void close();
};

#endif //PIXELS_COLUMNVECTOR_H
