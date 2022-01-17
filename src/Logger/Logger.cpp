//
// Created by Flo on 17/01/2022.
//

#include "Logger.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include "ConsoleColor.hpp"
#include "../Utils/StringUtils.hpp"

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
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.threadId = std::this_thread::get_id();
    logEntry.message = "[" + CurrentDateTimeToString() + "]: " + message;

    std::cout << ConsoleColor::blue
              << '[' << logEntry.threadId << ']'
              << logEntry.message
              << ConsoleColor::white << std::endl;

    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR: [" + CurrentDateTimeToString() + "]: " + message;

    std::cerr << ConsoleColor::red << logEntry.message
              << ConsoleColor::white << std::endl;

    messages.push_back(logEntry);
}