/*******************************************************************************
 * 文件名称: Log
 * 项目名称: EFModLoader
 * 创建时间: 2024/12/1
 * 作者: EternalFuture゙
 * Github: https://github.com/2079541547 
 * 版权声明: Copyright © 2024 EternalFuture. All rights reserved.
 * 许可证: This program is free software: you can redistribute it and/or modify
 *         it under the terms of the GNU Affero General Public License as published
 *         by the Free Software Foundation, either version 3 of the License, or
 *         (at your option) any later version.
 *
 *         This program is distributed in the hope that it will be useful,
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *         GNU Affero General Public License for more details.
 *
 *         You should have received a copy of the GNU Affero General Public License
 *         along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * 描述信息: 本文件为EFModLoader项目中的一部分。
 * 注意事项: 请严格遵守GNU AGPL v3.0协议使用本代码，任何未经授权的商业用途均属侵权行为。
 *******************************************************************************/

#include <Log.hpp>
#include <chrono>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <iostream>

#ifdef ANDROID_ENABLE
#include <android/log.h>
#endif


std::string EFModLoader::Log::getCurrentTime() {
    // 使用 std::chrono::system_clock 来获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 将当前时间点转换为 time_t 类型，它表示自 epoch 以来的秒数
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // 使用 localtime_r 函数将 time_t 转换成本地时间，并存储在一个 tm 结构体中
    // 注意：localtime_r 是线程安全的版本，而 localtime 不是
    std::tm buffer{};
    localtime_r(&in_time_t, &buffer);

    // 定义一个足够大的字符数组来存放格式化后的时间字符串
    char timeStr[20];

    // 使用 strftime 函数根据指定格式将 tm 结构体中的信息格式化成字符串
    // "%Y-%m-%d %X" 格式意味着年-月-日 时:分:秒
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %X", &buffer);

    // 返回格式化好的时间字符串
    return {timeStr};
}

std::string EFModLoader::Log::logLevelToString(EFModLoader::LogLevel level) {
    // 使用 switch 语句来匹配 LogLevel 枚举值
    switch (level) {
        case LogLevel::ERROR:  // 错误级别
            return "ERROR";  // 返回错误级别的字符串表示
        case LogLevel::WARN:   // 警告级别
            return "WARN";   // 返回警告级别的字符串表示
        case LogLevel::INFO:   // 信息级别
            return "INFO";   // 返回信息级别的字符串表示
        case LogLevel::DEBUG:  // 调试级别
            return "DEBUG";  // 返回调试级别的字符串表示
        case LogLevel::TRACE:  // 跟踪级别
            return "TRACE";  // 返回跟踪级别的字符串表示
        default:  // 如果 LogLevel 不是上述任何一种情况
            // 返回未知级别的字符串表示
            return "UNKNOWN";
    }
}

void EFModLoader::Log::LOG(EFModLoader::LogLevel level, const std::string &message,
                           const char *file, int line) {
    // 构建完整的日志消息字符串
    // 使用当前时间、进程ID、转换后的日志级别名称、源代码文件名及行号，以及实际的日志消息内容
    std::string fullMessage = "[" + getCurrentTime() + "] "          // 添加当前的时间戳
                              + "[" + std::to_string(getpid()) + "] " // 添加当前进程ID
                              + "[" + logLevelToString(level) + "] "  // 将日志级别从枚举转换为字符串形式并添加
                              + "[" + file + ":" + std::to_string(line) + "] " // 添加产生日志的文件名和行号
                              + message; // 最后加上具体的日志消息

    // 调用内部的日志处理函数，传入日志级别和构建好的完整日志消息
    logInternal(level, fullMessage);
}

void EFModLoader::Log::LOG(EFModLoader::LogLevel level, const std::string &function,
                           const std::string &message, const char *file, int line) {
    // 构建函数位置信息
    std::string position = "[" + function + "]";

    // 组装完整的日志消息字符串
    std::string fullMessage =
            "[" + getCurrentTime() + "] "          // 当前时间戳
            + "[" + std::to_string(getpid()) + "] " // 当前进程ID
            + "[" + logLevelToString(level) + "] "  // 日志级别（转换为可读形式）
            + position + " "                       // 调用该日志函数的具体函数名
            + "[" + file + ":" + std::to_string(line) + "] " // 文件名及行号
            + message;                             // 具体的日志内容

    // 将组装好的完整日志消息传递给内部处理函数
    logInternal(level, fullMessage);
}

void EFModLoader::Log::LOG(EFModLoader::LogLevel level, const std::string &Class,
                           const std::string &function, const std::string &message,
                           const char *file, int line) {
    // 构建完整的调用位置信息，包括类名和函数名
    std::string position = "[" + Class + "::" + function + "]";

    // 组装完整的日志消息字符串
    std::string fullMessage =
            "[" + getCurrentTime() + "] "          // 当前时间戳
            + "[" + std::to_string(getpid()) + "] " // 当前进程ID
            + "[" + logLevelToString(level) + "] "  // 日志级别（转换为可读形式）
            + position + " "                       // 类名与函数名
            + "[" + file + ":" + std::to_string(line) + "] " // 文件名及行号
            + message;                             // 具体的日志内容

    // 将组装好的完整日志消息传递给内部处理函数
    logInternal(level, fullMessage);
}

void EFModLoader::Log::LOG(EFModLoader::LogLevel level, const std::string &Namespace,
                           const std::string &Class, const std::string &function,
                           const std::string &message, const char *file, int line) {
    // 构建完整的调用位置信息，包括命名空间、类名和函数名
    std::string position = "[" + Namespace + "::" + Class + "::" + function + "]";

    // 组装完整的日志消息字符串
    std::string fullMessage =
            "[" + getCurrentTime() + "] "          // 当前时间戳
            + "[" + std::to_string(getpid()) + "] " // 当前进程ID
            + "[" + logLevelToString(level) + "] "  // 日志级别（转换为可读形式）
            + position + " "                       // 命名空间、类名与函数名
            + "[" + file + ":" + std::to_string(line) + "] " // 文件名及行号
            + message;                             // 具体的日志内容

    // 将组装好的完整日志消息传递给内部处理函数
    logInternal(level, fullMessage);
}


void EFModLoader::Log::logInternal(EFModLoader::LogLevel level, const std::string &fullMessage) {
#ifdef ANDROID_ENABLE
    // Android平台下的日志输出
    switch (level) {
        case LogLevel::ERROR:
            // 错误信息输出到安卓错误流
            __android_log_print(ANDROID_LOG_ERROR, "SilkCasket", "%s", fullMessage.c_str());
            break;
        case LogLevel::WARN:
            // 警告信息输出到安卓错误流
            __android_log_print(ANDROID_LOG_WARN, "SilkCasket", "%s", fullMessage.c_str());
            break;
        case LogLevel::INFO:
            // 信息输出到安卓输出流
            __android_log_print(ANDROID_LOG_INFO, "SilkCasket", "%s", fullMessage.c_str());
            break;
        case LogLevel::DEBUG:
            // 调试信息输出到安卓输出流
            __android_log_print(ANDROID_LOG_DEBUG, "SilkCasket", "%s", fullMessage.c_str());
            break;
        case LogLevel::TRACE:
            // 追踪信息输出到安卓输出流
            __android_log_print(ANDROID_LOG_DEFAULT, "SilkCasket", "%s", fullMessage.c_str());
            break;
    }
#else
    // 非Android平台下的日志输出
    switch (level) {
        case LogLevel::ERROR:
            // 错误信息输出到标准错误流
            std::cerr << fullMessage << std::endl;
            break;
        case LogLevel::WARN:
            // 警告信息输出到标准错误流
            std::cerr << fullMessage << std::endl;
            break;
        case LogLevel::INFO:
            // 信息输出到标准输出流
            std::cout << fullMessage << std::endl;
            break;
        case LogLevel::DEBUG:
            // 调试信息输出到标准输出流
            std::cout << fullMessage << std::endl;
            break;
        case LogLevel::TRACE:
            // 追踪信息输出到标准输出流
            std::cout << fullMessage << std::endl;
            break;
    }
#endif
}