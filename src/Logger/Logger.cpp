//
// Created by Flo on 17/01/2022.
//

#include "Logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

#include "ConsoleColor.hpp"
#include "../Utils/StringUtils.hpp"

std::mutex Logger::_mutex;
std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    //std::string output(30, '\0');
    //std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));
    return StringUtils::trim_copy(std::asctime(std::localtime(&now)));
}

void Logger::Log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.threadId = std::this_thread::get_id();
    logEntry.message = "[" + CurrentDateTimeToString() + "]: " + message;

    std::cout << ConsoleColor::blue
              << '[' << logEntry.threadId << ']'
              << logEntry.message
              << ConsoleColor::white
              << std::endl;

    messages.push_back(logEntry);
}

void Logger::Warn(const std::string& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    LogEntry logEntry;
    logEntry.type = LOG_WARNING;
    logEntry.threadId = std::this_thread::get_id();
    logEntry.message = "WARN: [" + CurrentDateTimeToString() + "]: " + message;

    std::cout << ConsoleColor::yellow
              << '[' << logEntry.threadId << ']'
              << logEntry.message
              << ConsoleColor::white
              << std::endl;

    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.threadId = std::this_thread::get_id();
    logEntry.message = "ERR: [" + CurrentDateTimeToString() + "]: " + message;

    std::cerr << ConsoleColor::red
              << '[' << logEntry.threadId << ']'
              << logEntry.message
              << ConsoleColor::white
              << std::endl;

    messages.push_back(logEntry);
}
