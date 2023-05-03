//
// Created by yuly on 03.05.23.
//
#include "utils/TimeProfiler.h"



thread_local std::map<std::string,std::chrono::steady_clock::time_point> TimeProfiler::profiling;

TimeProfiler &TimeProfiler::Instance() {
    static TimeProfiler instance;
    return instance;
}

TimeProfiler::TimeProfiler() {
}

void TimeProfiler::Start(const std::string& label) {
    if constexpr(enableProfile) {
        if (profiling.find(label) != profiling.end()) {
            throw InvalidArgumentException(
                    "TimeProfiler::Start: The same label has already been started. ");
        } else if (label.size() == 0) {
            throw InvalidArgumentException(
                    "TimeProfiler::Start: Label cannot be the empty string. ");
        } else {
            profiling[label] = std::chrono::steady_clock::now();
        }
    }
}

void TimeProfiler::End(const std::string& label) {
    if constexpr(enableProfile) {
        if (profiling.find(label) == profiling.end()) {
            throw InvalidArgumentException(
                    "TimeProfiler::End: The label is not started yet. ");
        } else if (label.size() == 0) {
            throw InvalidArgumentException(
                    "TimeProfiler::End: Label cannot be the empty string. ");
        }

        std::unique_lock<std::mutex> parallel_lock(lock);
        auto startTime = profiling[label];
        auto endTime = std::chrono::steady_clock::now();
        if (result.find(label) == result.end()) {
            result[label] = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (endTime - startTime).count();
        } else {
            result[label] = result[label] + std::chrono::duration_cast<std::chrono::nanoseconds>
                    (endTime - startTime).count();
        }
        profiling.erase(label);
    }
}

void TimeProfiler::Print() {
    if constexpr(enableProfile) {
        for(auto iter: result) {
            std::cout<<iter.first<<" "<<1.0 * iter.second / 1000000000 <<"ms(thread time)"<<std::endl;
        }
    }
}

void TimeProfiler::Reset() {
    profiling.clear();
    result.clear();
}

long TimeProfiler::Get(const std::string &label) {
    std::unique_lock<std::mutex> parallel_lock(lock);
    if(result.find(label) != result.end()) {
        return result[label];
    } else {
        throw InvalidArgumentException(
                "TimeProfiler::Get: The label is not contained in Timeprofiler. ");
    }
}

int TimeProfiler::GetResultSize() {
    return result.size();
}

