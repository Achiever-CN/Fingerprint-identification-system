#include "Logger.h"


Logger::Logger(const std::string &filename) : filename_(filename)
{
        file_.open(filename_, std::ios_base::app);
}

Logger::~Logger()
{
        file_.close();
}

void Logger::log(LogLevel level, const std::string &message, bool printf_flag)
{
        std::lock_guard<std::mutex> lock(mutex_);
        std::string timestamp = getCurrentTime();
        std::string levelStr = getLogLevelString(level);
        std::string output = timestamp + " [" + levelStr + "] " + message + "\n";

        // 输出到终端
        if (printf_flag)
                std::cout << getColoredOutput(level) << output << Color::Reset;

        // 输出到文件
        file_ << output;
        file_.flush(); // 确保数据被写入文件
}

std::string Logger::getCurrentTime()
{
        std::time_t now = std::time(nullptr);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return std::string(buffer);
}

std::string Logger::getLogLevelString(LogLevel level)
{
        switch (level)
        {
        case LogLevel::TRACE:
                return "TRACE";
        case LogLevel::DEBUG:
                return "DEBUG";
        case LogLevel::INFO:
                return "INFO";
        case LogLevel::WARNING:
                return "WARNING";
        case LogLevel::ERROR_:
                return "ERROR";
        default:
                return "UNKNOWN";
        }
}

std::string Logger::getColoredOutput(LogLevel level)
{
        switch (level)
        {
        case LogLevel::TRACE:
                return "";
        case LogLevel::DEBUG:
                return "";
        case LogLevel::INFO:
                return Color::Green;
        case LogLevel::WARNING:
                return Color::Yellow;
        case LogLevel::ERROR_:
                return Color::Red;
        default:
                return "";
        }
}

void Logger::log_format(LogLevel level, bool printf_flag, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    log(level, std::string(buffer), printf_flag);
}

