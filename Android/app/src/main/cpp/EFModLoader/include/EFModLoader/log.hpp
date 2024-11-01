//
// Created by eternalfuture on 2024/9/28.
//

#ifndef EFMODLOADER_LOG_HPP
#define EFMODLOADER_LOG_HPP

#include <android/log.h>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h> // For getpid

namespace EFModLoader {

    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    class Log {
    public:
        // 获取当前时间的字符串表示
        static std::string getCurrentTime();

        // 内部日志记录方法
        static void logInternal(const std::string &fullMessage);

        // 日志记录方法
        static void LOG(LogLevel level, const std::string &message, const char* file, int line);
        static void LOG(LogLevel level, const std::string &function, const std::string &message, const char* file, int line);
        static void LOG(LogLevel level, const std::string &Class, const std::string &function, const std::string &message, const char* file, int line);
        static void LOG(LogLevel level, const std::string &Namespace, const std::string &Class, const std::string &function, const std::string &message, const char* file, int line);

    private:
        // 将日志级别转换为字符串
        static std::string logLevelToString(LogLevel level);
    };

} // namespace EFModLoader

// 宏定义，用于捕获文件名和行号
#define EFLOG(level, ...) EFModLoader::Log::LOG(level, ##__VA_ARGS__, __FILE__, __LINE__)

#endif // EFMODLOADER_LOG_HPP