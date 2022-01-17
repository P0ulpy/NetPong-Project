//
// Created by Flo on 17/01/2022.
//

#ifndef NETPONG_PROJECT_LOGGER_HPP

#include <vector>
#include <string>
#include <thread>

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
    static void Err(const std::string& message);
};

#define NETPONG_PROJECT_LOGGER_HPP
#endif //NETPONG_PROJECT_LOGGER_HPP
