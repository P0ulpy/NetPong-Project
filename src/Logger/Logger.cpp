//
// Created by Flo on 17/01/2022.
//

#include "Logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "ConsoleColor.hpp"

std::mutex Logger::_mutex;
std::vector<LogEntry> Logger::messages;
std::map<std::thread::id, std::string> Logger::_threadsLabels;

std::string CurrentDateTimeToString()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    //std::string output(30, '\0');
    //std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));

    std::stringstream formatBuffer;
    formatBuffer << std::put_time(std::localtime(&now), "%H:%M:%S");

    return formatBuffer.str();
}

void Logger::Log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.threadId = std::this_thread::get_id();
    logEntry.message = "[" + CurrentDateTimeToString() + "]: " + message;

    std::string threadLabel = (_threadsLabels.contains(logEntry.threadId)) ? _threadsLabels.at(logEntry.threadId) : "";

    std::cout << ConsoleColor::blue
              << '[';

    if(!threadLabel.empty()) std::cout << threadLabel;
    else std::cout << logEntry.threadId;

    std::cout << ']'
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
    logEntry.message = "[" + CurrentDateTimeToString() + "] WARN: " + message;

    std::string threadLabel = (Logger::_threadsLabels.contains(logEntry.threadId)) ? Logger::_threadsLabels.at(logEntry.threadId) : "";

    std::cout << ConsoleColor::yellow
              << '[';

    if(!threadLabel.empty()) std::cout << threadLabel;
    else std::cout << logEntry.threadId;

    std::cout << ']'
              << logEntry.message
              << ConsoleColor::white
              << std::endl;

    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.threadId = std::this_thread::get_id();
    logEntry.message = "[" + CurrentDateTimeToString() + "] ERR: " + message;

    std::string threadLabel = (_threadsLabels.contains(logEntry.threadId)) ? Logger::_threadsLabels[logEntry.threadId] : "";

    std::cerr << ConsoleColor::red
              << '[';

    if(!threadLabel.empty()) std::cerr << threadLabel;
    else std::cerr << logEntry.threadId;

    std::cerr << ']'
              << logEntry.message
              << ConsoleColor::white
              << std::endl;

    messages.push_back(logEntry);
}

void Logger::SetThreadLabel(const std::string &label)
{
    std::lock_guard<std::mutex> lock(_mutex);
    Logger::_threadsLabels[std::this_thread::get_id()] = label;
}

void Logger::SetThreadLabel(const std::string &label, std::thread::id threadID)
{
    std::lock_guard<std::mutex> lock(_mutex);
    Logger::_threadsLabels[threadID] = label;
}