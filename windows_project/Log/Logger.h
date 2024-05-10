#pragma once

#include <iostream>
#include <mutex>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <cstdio>

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR_
};

namespace Color {
    const std::string Reset = "\033[0m";
    const std::string Red = "\033[31m";
    const std::string Green = "\033[32m";
    const std::string Yellow = "\033[33m";
}


class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();

    void log(LogLevel level, const std::string& message, bool printf_flag = false);
    void log_format(LogLevel level, bool printf_flag, const char* format, ...);

private:
    std::mutex mutex_;
    std::ofstream file_;
    std::string filename_;

    std::string getCurrentTime();
    std::string getLogLevelString(LogLevel level);
    std::string getColoredOutput(LogLevel level);
};

