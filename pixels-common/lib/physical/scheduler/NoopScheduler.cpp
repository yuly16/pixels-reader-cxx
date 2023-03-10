//
// Created by liyu on 3/8/23.
//

#include "physical/scheduler/NoopScheduler.h"

Scheduler * NoopScheduler::instance = nullptr;

Scheduler * NoopScheduler::Instance() {
    if(instance == nullptr) {
        instance = new NoopScheduler();
    }
    return instance;
}

std::vector<ByteBuffer *> NoopScheduler::executeBatch(PhysicalReader * reader, RequestBatch batch, long queryId) {
    if(batch.getSize() < 0) {
        return std::vector<ByteBuffer *>{};
    }
//    auto futures = batch.getFutures();
    auto requests = batch.getRequests();
    std::vector<ByteBuffer *> bb;
    // TODO: support async read
    for(int i = 0; i < batch.getSize(); i++) {
        Request request = requests[i];
        reader->seek(request.start);
        // TODO: here we didn't find a alternative for futures.complete
        bb.push_back(reader->readFully(request.length));
    }
    return bb;
}

