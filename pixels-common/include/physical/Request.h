//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_REQUEST_H
#define PIXELS_REQUEST_H

#include <iostream>

class Request {
public:
    uint64_t queryId;
    uint64_t start;
    uint64_t length;
    Request(uint64_t queryId_, uint64_t start_, uint64_t length_);
    int hashCode();
    int comparedTo(Request o);
};
#endif //PIXELS_REQUEST_H
