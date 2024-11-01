//
// Created by eternalfuture on 2024/9/28.
//

#include "EFModLoader/log.hpp"
#include <ctime>
#include <cstring>

using namespace EFModLoader;

// 获取当前时间的字符串表示
std::string Log::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm buffer;
    localtime_r(&in_time_t, &buffer);

    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %X", &buffer);

    return std::string(timeStr);
}

// 将日志级别转换为字符串
std::string Log::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::WARN: return "WARN";
        case LogLevel::INFO: return "INFO";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::TRACE: return "TRACE";
    }
    return "UNKNOWN";
}

// 内部日志记录方法
void Log::logInternal(const std::string &fullMessage) {
    __android_log_print(ANDROID_LOG_INFO, "EFModLoader", "%s", fullMessage.c_str());
}

// 日志记录方法
void Log::LOG(LogLevel level, const std::string &message, const char* file, int line) {
std::string fullMessage = "[" + getCurrentTime() + "] [" + std::to_string(getpid()) + "] [" + logLevelToString(level) + "] " + " [" + file + ":" + std::to_string(line) + "] " + message;
logInternal(fullMessage);
}

void Log::LOG(LogLevel level, const std::string &function, const std::string &message, const char* file, int line) {
std::string position = "[" + function + "]";
std::string fullMessage = "[" + getCurrentTime() + "] [" + std::to_string(getpid()) + "] [" + logLevelToString(level) + "] " + position + " " + "[" + file + ":" + std::to_string(line) + "] " +  message;
logInternal(fullMessage);
}

void Log::LOG(LogLevel level, const std::string &Class, const std::string &function, const std::string &message, const char* file, int line) {
std::string position = "[" + Class + "::" + function + "]";
std::string fullMessage = "[" + getCurrentTime() + "] [" + std::to_string(getpid()) + "] [" + logLevelToString(level) + "] " + position + " " + "[" + file + ":" + std::to_string(line) + "] " + message;
logInternal(fullMessage);
}

void Log::LOG(LogLevel level, const std::string &Namespace, const std::string &Class, const std::string &function, const std::string &message, const char* file, int line) {
std::string position = "[" + Namespace + "::" + Class + "::" + function + "]";
std::string fullMessage = "[" + getCurrentTime() + "] [" + std::to_string(getpid()) + "] [" + logLevelToString(level) + "] " + position + " " + "[" + file + ":" + std::to_string(line) + "] " + message;
logInternal(fullMessage);
}