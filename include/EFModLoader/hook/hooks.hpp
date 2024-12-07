/*******************************************************************************
 * 文件名称: hooks
 * 项目名称: EFModLoader
 * 创建时间: 2024/12/7
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

#include <vector>
#include <map>
#include <error.h>
#include <cstdint>
#include <stdexcept>
#include "../Log.hpp"

namespace EFModLoader::Hooks {

    /**
     * @brief 结构体 HOOKS 用于Hook一个指定函数。
     *
     * 包含了命名空间、类名、函数名、返回类型描述符以及参数个数等信息，
     * 以便在运行时动态查找并hook对应的函数。
     */
    struct HOOKS {
        std::string Namespace; ///< 函数所在的命名空间
        std::string Class;     ///< 函数所属的类名
        std::string Name;      ///< 函数名称
        std::string Return;    ///< 函数返回类型的描述符
        int arg;               ///< 参数个数或特定参数值，用于区分重载函数

        /**
         * @brief 创建唯一的键字符串，用于生成哈希值。
         * @return 返回由命名空间、类名、函数名、参数个数及返回类型组成的字符串。
         */
        std::string getKey() const {
            return Namespace + Class + Name + std::to_string(arg) + Return;
        }
    };

    struct HookInfo {
        void* ptr;
        void* new_ptr;
        void** old_ptr;
    };


    inline std::vector<std::map<uint64_t, std::vector<void *>>> hookVector_Mod;
    inline std::map<uint64_t, HookInfo> hookMap;

    std::vector<void *> getHooksFunc(HOOKS hook);
    void registerHook(const HOOKS& hook, void* ptr, void* new_ptr, void** old_ptr);
    void registerHooks();


    /**
     * @callFunction 调用函数指针
     *
     * @tparam R 返回类型
     * @tparam Args 参数类型列表
     * @param funcPtr 函数指针
     * @param args 函数参数
     * @return 函数调用的结果
     */
    template <typename R, typename... Args>
    R callFunction(void *funcPtr, Args &&...args) {
        if (!funcPtr) {
            EFLOG(LogLevel::ERROR, "EFModLoader", "Hooks", "callFunction", "传入了空函数指针！");
            throw std::invalid_argument("函数指针不能为NULL");
        }

        // 从 void* 转换为函数指针
        using FuncPtr = R (*)(Args...);
        auto f = reinterpret_cast<FuncPtr>(funcPtr);

        // 调用函数
        EFLOG(LogLevel::INFO, "EFModLoader", "Hooks", "callFunction", "调用函数指针：" + std::to_string(reinterpret_cast<uintptr_t>(f)));
        return f(std::forward<Args>(args)...);
    }
}