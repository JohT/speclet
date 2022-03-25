// This implementation is inspired by these references:
// - The Cherno - VISUAL BENCHMARKING in C++ - https://www.youtube.com/watch?v=xlAH4dbMVnU&t=197s
// - TheCherno/Instrumentor.h - https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e
// - David Churchill - https://pastebin.com/qw5Neq4U
// - Mattias Aronsson - https://gist.github.com/maronsson/073840bf94e4d6df94c5f294a6e96e03
//
// Use Chrome browser to view the results of the JSON file using the following URI: chrome://tracing
#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <limits>
#include <mutex>
#include <string>
#include <thread>

#ifdef LOG_PERFORMANCE
#define LOG_PERFORMANCE_BEGIN(name) PerformanceLogger::getInstance().beginSession(name)
#define LOG_PERFORMANCE_END() PerformanceLogger::getInstance().endSession()
#define LOG_PERFORMANCE_OF_SCOPE(name) PerformanceLogTimer timer##__LINE__(name)
#else
#define LOG_PERFORMANCE_BEGIN(name)
#define LOG_PERFORMANCE_END()
#define LOG_PERFORMANCE_OF_SCOPE(name)
#endif

struct PerformanceLogEntry {
    std::string name;
    long long start;
    long long end;
    std::size_t threadID;
};

class PerformanceLogger {
private:
    std::string sessionName = "None";
    std::ofstream outputStream;
    int profileCount = 0;
    std::mutex mutex;
    bool activeSession = false;

    PerformanceLogger() = default;
    PerformanceLogger(const PerformanceLogger &) = delete;           // No copy constructor
    PerformanceLogger(PerformanceLogger &&) = delete;                // No move constructor
    PerformanceLogger &operator=(const PerformanceLogger &) = delete;// No copy assignment
    PerformanceLogger &operator=(PerformanceLogger &&) = delete;     // No move assignment

public:
    ~PerformanceLogger() {
        endSession();
    }

    void beginSession(const std::string &name, const std::string &filepath = "performancelog.json") {
        if (activeSession) {
            endSession();
        }
        activeSession = true;
        outputStream.open(filepath);
        writeHeader();
        sessionName = name;
    }

    void endSession() {
        if (!activeSession) {
            return;
        }
        activeSession = false;
        writeFooter();
        outputStream.close();
        profileCount = 0;
    }

    void writeProfile(const PerformanceLogEntry &entry) {
        std::lock_guard<std::mutex> lock(mutex);

        if (profileCount++ > 0) {
            outputStream << ",";
        }
        std::string name = entry.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        outputStream << "{";
        outputStream << "\"cat\":\"function\",";
        outputStream << "\"dur\":" << (entry.end - entry.start) << ',';
        outputStream << "\"name\":\"" << name << "\",";
        outputStream << "\"ph\":\"X\",";
        outputStream << "\"pid\":0,";
        outputStream << "\"tid\":" << entry.threadID << ",";
        outputStream << "\"ts\":" << entry.start;
        outputStream << "}";
        //outputStream.flush();
    }

    void writeHeader() {
        outputStream << "{\"otherData\": {},\"traceEvents\":[";
        //outputStream.flush();
    }

    void writeFooter() {
        outputStream << "]}";
        //outputStream.flush();
    }

    static PerformanceLogger &getInstance() {
        static PerformanceLogger instance;
        return instance;
    }
};

class PerformanceLogTimer {
private:
    const char *name;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimestamp;
    bool stopped = false;

public:
    explicit PerformanceLogTimer(const char *newName)
        : name(newName) {
        startTimestamp = std::chrono::high_resolution_clock::now();
    }

    ~PerformanceLogTimer() {
        if (!stopped) {
            stop();
        }
    }

    void stop() {
        auto endTimestamp = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimestamp).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimestamp).time_since_epoch().count();
        auto threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

        PerformanceLogger::getInstance().writeProfile({name, start, end, threadID});

        stopped = true;
    }
};