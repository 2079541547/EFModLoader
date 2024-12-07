/*******************************************************************************
 * 文件名称: Field
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

namespace EFModLoader::API::Field {
    /**
     * @brief 结构体 EFModAPI_Field 用于唯一标识一个字段。
     *
     * 包含了命名空间、类名和字段名信息，以便在运行时动态获取对应的字段值。
     */
    struct EFModAPI_Field {
        std::string Namespace; ///< 字段所在的命名空间
        std::string Class;     ///< 字段所属的类名
        std::string Name;      ///< 字段名称

        /**
         * @brief 创建唯一的键字符串，用于生成哈希值。
         * @return 返回由命名空间、类名和字段名组成的字符串。
         */
        std::string getKey() const {
            return Namespace + Class + Name;
        }
    };

    /**
     * @var FieldMap
     * @brief 全局字段映射表，用于存储字段标识符到字段指针的映射。
     *
     * 这是一个全局的std::map容器，用于保存所有已注册字段的信息。键是64位无符号整数，代表字段的唯一标识符；
     * 值是指向实际字段对象的void*指针。通过这种方式，可以方便地根据字段ID查找对应的字段实例。
     */
    inline std::map<uint64_t, void*> FieldMap = {};

    /**
     * @function registerField
     * @brief 注册一个新的字段到全局字段映射表中。
     *
     * 此函数用于将给定的字段信息添加到全局的`FieldMap`中。它接受一个包含字段元数据的`EFModAPI_Field`结构体，
     * 以及指向字段实例的指针。然后，它会计算字段的唯一标识符，并将其与指针一起存储在`FieldMap`中。
     *
     * @param Field 包含字段元数据的EFModAPI_Field结构体。
     * @param PTR 指向字段实例的指针。
     */
    void registerField(EFModAPI_Field Field, void* PTR);

    /**
     * @function Register
     * @brief 初始化并注册所有必要的字段。
     *
     * 此函数负责初始化模组所需的字段，并调用`registerField`函数将它们注册到全局`FieldMap`中。
     * 它应该在模组加载时被调用，以确保所有字段在模组运行之前已经被正确注册。
     */
    void Register();
}