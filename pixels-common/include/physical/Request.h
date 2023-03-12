//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_REQUEST_H
#define PIXELS_REQUEST_H

class Request {
public:
    long queryId;
    long start;
    int length;
    Request(long queryId_, long start_, int length_);
    int hashCode();
    int comparedTo(Request o);
};
#endif //PIXELS_REQUEST_H
