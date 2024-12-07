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

#pragma once

#include <string>

namespace EFModLoader {

    /**
    * @enum class LogLevel
    * @brief 定义日志级别。
    *
    * 这个枚举类定义了不同的日志级别，从最详细的 TRACE 到最严重的 ERROR。
    * 日志级别用于控制哪些信息会被记录下来，从而帮助开发者根据需要过滤日志输出。
    */
    enum class LogLevel {
        /**
         * @brief 最详细的日志级别，通常用于跟踪程序的内部执行流程。
         *
         * 在开发和调试过程中非常有用，但在生产环境中可能会产生大量日志信息。
         */
        TRACE,

        /**
         * @brief 用于调试目的的日志级别。
         *
         * 包含比 INFO 更详细的信息，但比 TRACE 级别更少。
         * 适用于开发阶段，可以帮助开发者诊断问题。
         */
        DEBUG,

        /**
         * @brief 通用信息级别的日志。
         *
         * 用于记录应用程序运行时的一般信息，比如启动、关闭等事件。
         * 适合在生产环境中开启，不会产生过多的日志量。
         */
        INFO,

        /**
         * @brief 警告级别的日志。
         *
         * 用于记录可能影响应用正常工作的潜在问题，但当前不影响程序运行的情况。
         * 应当引起注意，并考虑在未来进行修复或优化。
         */
        WARN,

        /**
         * @brief 错误级别的日志。
         *
         * 用于记录阻止应用正常运行的错误情况。
         * 表示出现了严重的问题，需要立即采取行动。
         */
        ERROR
    };

    /**
     * @class Log
     * @brief 日志相关的辅助类。
     *
     * 这个内部类提供了与日志记录相关的一些静态方法。
     */
    class Log {
    public:

        /**
         * @brief 记录一条日志消息。
         *
         * 该函数根据给定的日志级别、消息内容、文件名和行号生成完整的日志条目，并调用内部的日志记录函数来处理。
         *
         * @param level 日志级别枚举值。
         * @param message 要记录的日志消息。
         * @param file 产生日志的源文件名称。
         * @param line 产生日志的源代码行号。
         */
        static void LOG(LogLevel level, const std::string &message, const char* file, int line);

        /**
        * @brief 记录一条日志消息。
        *
        * 该函数用于记录包含时间戳、进程ID、日志级别、调用位置（包括函数名和文件名及行号）以及具体日志内容的消息。
        * 日志消息将被格式化并传递给内部的日志处理函数进行进一步处理。
        *
        * @param level 日志级别，指示日志的重要性。例如：DEBUG, INFO, WARNING, ERROR等。
        * @param function 调用此日志函数的具体函数名称。
        * @param message 实际要记录的日志文本。
        * @param file 产生日志的源文件路径。
        * @param line 产生日志的源代码行号。
        */
        static void LOG(LogLevel level, const std::string &function, const std::string &message, const char* file, int line);

        /**
        * @brief 记录一条包含丰富上下文信息的日志消息。
        *
        * 该函数用于记录包含当前时间戳、进程ID、日志级别、调用位置（包括类名、函数名以及文件名及行号）以及具体日志内容的消息。
        * 日志消息将被格式化并传递给内部的日志处理函数进行进一步处理。
        *
        * @param level 日志级别，指示日志的重要性。例如：DEBUG, INFO, WARNING, ERROR等。
        * @param Class 调用此日志函数的具体类名称。
        * @param function 调用此日志函数的具体函数名称。
        * @param message 实际要记录的日志文本。
        * @param file 产生日志的源文件路径。
        * @param line 产生日志的源代码行号。
        */
        static void LOG(LogLevel level, const std::string &Class, const std::string &function, const std::string &message, const char* file, int line);

        /**
         * @brief 记录一条包含丰富上下文信息的日志消息。
         *
         * 该函数用于记录包含当前时间戳、进程ID、日志级别、调用位置（包括命名空间、类名、函数名以及文件名及行号）以及具体日志内容的消息。
         * 日志消息将被格式化并传递给内部的日志处理函数进行进一步处理。
         *
         * @param level 日志级别，指示日志的重要性。例如：DEBUG, INFO, WARNING, ERROR等。
         * @param Namespace 调用此日志函数的具体命名空间名称。
         * @param Class 调用此日志函数的具体类名称。
         * @param function 调用此日志函数的具体函数名称。
         * @param message 实际要记录的日志文本。
         * @param file 产生日志的源文件路径。
         * @param line 产生日志的源代码行号。
         */
        static void LOG(LogLevel level, const std::string &Namespace, const std::string &Class, const std::string &function, const std::string &message, const char* file, int line);

    private:

        /**
         * @brief 获取当前系统时间并以字符串形式返回。
         *
         * 该函数利用C++标准库中的<chrono>和<ctime>来获取当前的本地时间，并将其格式化为"YYYY-MM-DD HH:MM:SS"的形式。
         *
         * @return 返回一个表示当前时间的字符串。
         */
        static std::string getCurrentTime();

        /**
         * @brief 将LogLevel枚举转换为对应的字符串表示形式。
         *
         * 该函数接受一个LogLevel枚举值作为输入，并返回一个描述该日志级别的字符串。
         * 如果提供的LogLevel不在预定义的范围内，则返回"UNKNOWN"。
         *
         * @param level 日志级别枚举值。
         * @return 返回一个表示日志级别的字符串。
         */
        static std::string logLevelToString(LogLevel level);

        /**
         * @brief 内部日志处理函数。
         *
         * 根据提供的日志级别，将完整的日志消息输出到适当的日志流中。
         * 在Android平台上，使用__android_log_print输出到Android日志系统。
         * 在非Android平台上，错误和警告信息输出到标准错误流（stderr），其他信息输出到标准输出流（stdout）。
         *
         * @param level 日志级别，指示日志的重要性。例如：ERROR, WARN, INFO, DEBUG, TRACE等。
         * @param fullMessage 完整的日志消息字符串，已经包含了时间戳、进程ID、日志级别、调用位置以及具体日志内容。
         */
        static void logInternal(LogLevel level, const std::string &fullMessage);

    };
}

#define EFLOG(level, ...) EFModLoader::Log::LOG(level, ##__VA_ARGS__, __FILE__, __LINE__)