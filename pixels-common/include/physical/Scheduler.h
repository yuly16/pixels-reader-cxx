//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_SCHEDULER_H
#define PIXELS_SCHEDULER_H
#include "physical/PhysicalReader.h"
#include "physical/RequestBatch.h"

class Scheduler {
public:
    /**
      * Execute a batch of read requests, and return the future of the completion of
      * all the requests.
      * @param reader
      * @param batch
      * @param queryId
      */
    virtual std::vector<ByteBuffer *> executeBatch(PhysicalReader * reader, RequestBatch batch, long queryId) = 0;
};
#endif //PIXELS_SCHEDULER_H
