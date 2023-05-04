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

std::vector<std::shared_ptr<ByteBuffer>> NoopScheduler::executeBatch(std::shared_ptr<PhysicalReader> reader, RequestBatch batch, long queryId) {
    if(batch.getSize() < 0) {
        return std::vector<std::shared_ptr<ByteBuffer>>{};
    }


    auto requests = batch.getRequests();
    std::vector<std::shared_ptr<ByteBuffer>> bbs;
//	bbs.resize(batch.getSize());
    for(int i = 0; i < batch.getSize(); i++) {
        Request request = requests[i];
        reader->seek(request.start);
//		reader->readAsync(request.length, i);
        bbs.emplace_back(reader->readFully(request.length));
    }
//	for(int i = 0; i < batch.getSize(); i++) {
//		auto result = reader->completeAsync();
//		int idx = result.first;
//		auto bb = result.second;
//		bbs.at(idx) = bb;
//	}

    return bbs;
}

