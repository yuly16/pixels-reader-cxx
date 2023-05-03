//
// Created by yuly on 03.05.23.
//

#ifndef DUCKDB_TIMEPROFILER_H
#define DUCKDB_TIMEPROFILER_H

#include <iostream>
#include <memory>
#include <string>
#include "exception/InvalidArgumentException.h"
#include <chrono>
#include <map>
#include <mutex>

constexpr bool enableProfile = true;

class TimeProfiler {
public:
    static TimeProfiler & Instance();
    void Start(const std::string& label);
    void End(const std::string& label);
    long Get(const std::string &label);
    void Reset();
    void Print();
    int GetResultSize();
private:
    TimeProfiler();
    static thread_local std::map<std::string,std::chrono::steady_clock::time_point> profiling;
    std::mutex lock;
    std::map<std::string, long> result;
};

#endif //DUCKDB_TIMEPROFILER_H
