/*******************************************************************************
 * 文件名称: Func
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

#include <map>
#include <string>
#include <cstdint>

namespace EFModLoader::API::Func {
    /**
     * @brief 结构体 EFModAPI_Func 用于唯一标识一个函数。
     *
     * 包含了命名空间、类名、函数名、返回类型描述符以及参数个数等信息，
     * 以便在运行时动态查找并调用对应的函数。
     */
    struct EFModAPI_Func {
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

    /**
     * @var FuncMap
     * @brief 全局函数映射表，用于存储函数标识符到函数指针的映射。
     *
     * 这是一个全局的std::map容器，用于保存所有已注册函数的信息。键是64位无符号整数，代表函数的唯一标识符；
     * 值是指向实际函数的void*指针。通过这种方式，可以方便地根据函数ID查找对应的函数实例。
     */
    inline std::map<uint64_t, void*> FuncMap = {};

    /**
     * @function registerFunc
     * @brief 注册一个新的函数到全局函数映射表中。
     *
     * 此函数用于将给定的函数信息添加到全局的`FuncMap`中。它接受一个包含函数元数据的`EFModAPI_Func`结构体，
     * 以及指向函数的指针。然后，它会计算函数的唯一标识符，并将其与指针一起存储在`FuncMap`中。
     *
     * @param FUNC 包含函数元数据的EFModAPI_Func结构体。
     * @param PTR 指向函数的指针。
     */
    void registerFunc(EFModAPI_Func FUNC, void* PTR);

    /**
     * @function Register
     * @brief 初始化并注册所有必要的函数。
     *
     * 此函数负责初始化模组所需的函数，并调用`registerFunc`函数将它们注册到全局`FuncMap`中。
     * 它应该在模组加载时被调用，以确保所有函数在模组运行之前已经被正确注册。
     */
    void Register();
}