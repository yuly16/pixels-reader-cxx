//
// Created by liyu on 3/10/23.
//

#ifndef PIXELS_SCHEDULERFACTORY_H
#define PIXELS_SCHEDULERFACTORY_H

#include "physical/Scheduler.h"
#include "physical/scheduler/NoopScheduler.h"

class SchedulerFactory {
public:
    static SchedulerFactory * Instance();
    Scheduler * getScheduler();
private:
    static SchedulerFactory * instance;
    Scheduler * scheduler;
    SchedulerFactory();
};
#endif //PIXELS_SCHEDULERFACTORY_H
