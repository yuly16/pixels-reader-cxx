//
// Created by yuly on 01.05.23.
//

#include "physical/scheduler/SortMergeScheduler.h"

Scheduler * SortMergeScheduler::instance = nullptr;

Scheduler * SortMergeScheduler::Instance() {
    if(instance == nullptr) {
        instance = new SortMergeScheduler();
    }
    return instance;
}

std::vector<std::shared_ptr<ByteBuffer>> SortMergeScheduler::executeBatch(std::shared_ptr<PhysicalReader> reader, RequestBatch batch, long queryId) {
    if(batch.getSize() < 0) {
        return std::vector<std::shared_ptr<ByteBuffer>>{};
    }
    auto requests = batch.getRequests();
    std::vector<std::shared_ptr<ByteBuffer>> bbs;
    bbs.resize(batch.getSize());
    for(int i = 0; i < batch.getSize(); i++) {
        Request request = requests[i];
        reader->seek(request.start);
        reader->readAsync(request.length, i);
//        bbs.emplace_back(reader->readFully(request.length));
    }
    for(int i = 0; i < batch.getSize(); i++) {
        auto result = reader->completeAsync();
        int idx = result.first;
        auto bb = result.second;
        bbs.at(idx) = bb;
    }

    return bbs;
}

