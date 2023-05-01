//
// Created by yuly on 01.05.23.
//

#ifndef DUCKDB_SORTMERGESCHEDULER_H
#define DUCKDB_SORTMERGESCHEDULER_H

#include "physical/Scheduler.h"

class SortMergeScheduler : public Scheduler {
    // TODO: logger
public:
    static Scheduler * Instance();
    std::vector<std::shared_ptr<ByteBuffer>> executeBatch(std::shared_ptr<PhysicalReader> reader, RequestBatch batch, long queryId) override;
private:
    static Scheduler * instance;
};

#endif //DUCKDB_SORTMERGESCHEDULER_H
