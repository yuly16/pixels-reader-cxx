//
// Created by liyu on 3/7/23.
//

#include "physical/Request.h"


Request::Request(long queryId_, long start_, int length_) {
    queryId = queryId_;
    start = start_;
    length = length_;
}

int Request::hashCode() {
    return (int) ((start << 32) >> 32);
}

int Request::comparedTo(Request o) {
    return start == o.start;
}
