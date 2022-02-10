//
// Created by Flo on 17/01/2022.
//

#ifndef NETPONG_PROJECT_LOGGER_HPP

#include <vector>
#include <map>
#include <string>
#include <thread>
#include <mutex>

enum LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry {
    LogType type;
    std::thread::id threadId;
    std::string message;
};

class Logger {
public:
    static std::vector<LogEntry> messages;
    static void Log(const std::string& message);
    static void Warn(const std::string& message);
    static void Err(const std::string& message);

    static void SetThreadLabel(const std::string& label);
    static void SetThreadLabel(const std::string& label, std::thread::id threadID);
private:
    static std::mutex _mutex;

    static std::map<std::thread::id, std::string> _threadsLabels;
};

#define NETPONG_PROJECT_LOGGER_HPP
#endif //NETPONG_PROJECT_LOGGER_HPP
