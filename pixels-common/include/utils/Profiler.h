//
// Created by yuly on 03.05.23.
//

#ifndef DUCKDB_PROFILER_H
#define DUCKDB_PROFILER_H

#include <iostream>
#include <memory>
#include <string>
#include "exception/InvalidArgumentException.h"
#include <chrono>
#include <map>
#include <mutex>

class Profiler {
public:
    static Profiler & Instance();
    void Start(const std::string& label);
    void End(const std::string& label);
    int Get(const std::string &label);
    void Reset();
    void Print();
    int GetResultSize();
private:
    Profiler();
    static thread_local std::map<std::string,std::chrono::steady_clock::time_point> profiling;
    std::mutex lock;
    std::map<std::string, int> result;
};

#endif //DUCKDB_PROFILER_H
