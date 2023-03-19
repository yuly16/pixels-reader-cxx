//
// Created by liyu on 3/8/23.
//

#ifndef PIXELS_NOOPSCHEDULER_H
#define PIXELS_NOOPSCHEDULER_H

#include "physical/Scheduler.h"

class NoopScheduler : public Scheduler {
    // TODO: logger
public:
    static Scheduler * Instance();
    std::vector<std::shared_ptr<ByteBuffer>> executeBatch(PhysicalReader * reader, RequestBatch batch, long queryId) override;
private:
    static Scheduler * instance;
};
#endif //PIXELS_NOOPSCHEDULER_H
