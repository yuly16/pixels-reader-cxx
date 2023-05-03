//
// Created by yuly on 03.05.23.
//
#include "utils/Profiler.h"


thread_local std::map<std::string,std::chrono::steady_clock::time_point> Profiler::profiling;

Profiler &Profiler::Instance() {
    static Profiler instance;
    return instance;
}

Profiler::Profiler() {
}

void Profiler::Start(const std::string& label) {
    if(profiling.find(label) != profiling.end()) {
        throw InvalidArgumentException(
                "Profiler::Start: The same label has already been started. ");
    } else if(label.size() == 0) {
        throw InvalidArgumentException(
                "Profiler::Start: Label cannot be the empty string. ");
    } else {
        profiling[label] = std::chrono::steady_clock::now();
    }
}

void Profiler::End(const std::string& label) {
    if(profiling.find(label) == profiling.end()) {
        throw InvalidArgumentException(
                "Profiler::Start: The same label has already been started. ");
    } else if(label.size() == 0) {
        throw InvalidArgumentException(
                "Profiler::Start: Label cannot be the empty string. ");
    }

    std::unique_lock<std::mutex> parallel_lock(lock);
    auto startTime = profiling[label];
    auto endTime = std::chrono::steady_clock::now();
    if(result.find(label) == result.end()) {
        result[label] = std::chrono::duration_cast<std::chrono::milliseconds>
                (endTime - startTime).count();
    } else {
        result[label] = result[label] + std::chrono::duration_cast<std::chrono::milliseconds>
                (endTime - startTime).count();
    }
    profiling.erase(label);
}

void Profiler::Print() {

    for(auto iter: result) {
        std::cout<<iter.first<<" "<<iter.second<<std::endl;
    }
}

void Profiler::Reset() {
    profiling.clear();
    result.clear();
}

int Profiler::Get(const std::string &label) {
    std::unique_lock<std::mutex> parallel_lock(lock);
    if(result.find(label) != result.end()) {
        return result[label];
    } else {
        throw InvalidArgumentException(
                "Profiler::Get: The label is not contained in profiler. ");
    }
}

int Profiler::GetResultSize() {
    return result.size();
}

