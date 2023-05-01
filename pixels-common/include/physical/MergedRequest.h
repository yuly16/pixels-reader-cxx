//
// Created by yuly on 01.05.23.
//

#ifndef DUCKDB_MERGEDREQUEST_H
#define DUCKDB_MERGEDREQUEST_H

class MergedRequest {
public:
    MergedRequest() {
    }
private:
    long queryId;
    long start;
    long end;
    int length; // the length of merged request
    int size;   // the number of sub-requests
};
#endif //DUCKDB_MERGEDREQUEST_H
