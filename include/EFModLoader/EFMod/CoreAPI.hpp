/*******************************************************************************
 * 文件名称: CoreAPI
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

#include <filesystem>
#include <map>
#include <string>

/**
 * @var private_path
 * @brief 模组所需的私有路径。
 *
 * 这个变量保存了模组需要访问的私有资源路径。它通常指向一个特定的目录，
 * 其中包含模组运行时所需的文件或配置。确保路径的有效性和安全性对于模组的正确运行至关重要。
 */
std::filesystem::path private_path;

/**
 * @var ModName
 * @brief 模组的名称。
 *
 * 这个字符串变量保存了模组的唯一名称。模组名称应当是独一无二的，以便于识别和管理。
 * 在某些情况下，它可能会被用作生成模组唯一标识符的一部分。
 */
std::string ModName;

/**
 * @var Author
 * @brief 模组作者的名称。
 *
 * 这个字符串变量保存了创建模组的作者的名字。作者信息可以帮助用户了解谁开发了这个模组，
 * 并且在出现兼容性问题或其他问题时，可以联系到正确的开发者。
 */
std::string Author;

/**
 * @var Hash
 * @brief 模组文件的哈希值映射表。
 *
 * 这个映射表保存了模组文件及其对应的哈希值。键是文件名或文件路径，值是文件内容的哈希值。
 * 哈希值通常用于验证文件的完整性，以确保模组文件没有被篡改或损坏。这对于维护模组的安全性和可靠性非常重要。
 */
std::map<std::string, std::string> Hash;