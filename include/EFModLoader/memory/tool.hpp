/*******************************************************************************
 * 文件名称: tool
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

#include <iostream>
#include <filesystem>

namespace EFModLoader::Memory::Tool {

    /**
     * @enum MemoryProtection
     * @brief 表示不同类型的内存保护模式。
     */
    enum class MemoryProtection {
        NoAccess,           ///< 禁止所有访问
        ReadOnly,           ///< 只读访问
        ReadWrite,          ///< 读写访问
        Execute,            ///< 可执行访问（仅执行）
        ExecuteReadWrite    ///< 可执行且可读写访问
    };

    /**
     * @function redirectPtr
     * @brief 将旧指针重定向到新地址。
     *
     * 此函数用于更新给定指针变量的值，使其指向新的内存位置。
     *
     * @param old_ptr 指向要被重定向的指针的引用。
     * @param new_ptr 新的目标地址。
     */
    void redirectPtr(void*& old_ptr, void* new_ptr);

    /**
     * @function dumpMemoryToFile
     * @brief 将指定内存区域的内容导出到文件。
     *
     * 此函数用于将从`start`开始的`size`字节内存内容保存到指定路径的文件中。
     *
     * @param path 文件路径。
     * @param start 内存区域的起始地址。
     * @param end 内存区域的结束偏移量（相对于起始地址）。
     */
    void dumpMemoryToFile(const std::filesystem::path& path, const void* start, size_t end);

    /**
     * @function printMemoryInfo
     * @brief 打印当前进程的内存信息。
     *
     * 此函数用于输出有关当前进程中内存使用的调试信息。
     */
    void printMemoryInfo();

    /**
     * @function allocateMemory
     * @brief 分配指定大小的内存块。
     *
     * 此函数用于分配一块未初始化的内存，并返回指向这块内存的指针。
     *
     * @param size 要分配的内存量（以字节为单位）。
     * @return 分配的内存块的指针，如果分配失败则返回nullptr。
     */
    void* allocateMemory(size_t size);

    /**
     * @function freeMemory
     * @brief 释放之前分配的内存块。
     *
     * 此函数用于释放由`allocateMemory`分配的内存块。
     *
     * @param ptr 要释放的内存块的指针。
     */
    void freeMemory(void* ptr);

    /**
     * @function copyMemory
     * @brief 复制内存块。
     *
     * 此函数用于将`num`字节的数据从`src`复制到`dest`。
     *
     * @param dest 目标内存区域的起始地址。
     * @param src 源内存区域的起始地址。
     * @param num 要复制的字节数。
     * @return 返回目标内存区域的起始地址。
     */
    void* copyMemory(void* dest, const void* src, size_t num);

    /**
     * @function moveMemory
     * @brief 移动内存块。
     *
     * 此函数用于将`num`字节的数据从`src`移动到`dest`。它处理重叠区域的情况。
     *
     * @param dest 目标内存区域的起始地址。
     * @param src 源内存区域的起始地址。
     * @param num 要移动的字节数。
     * @return 返回目标内存区域的起始地址。
     */
    void* moveMemory(void* dest, const void* src, size_t num);

    /**
     * @function fillMemory
     * @brief 填充内存区域。
     *
     * 此函数用于用给定的值填充`num`字节的内存区域。
     *
     * @param ptr 要填充的内存区域的起始地址。
     * @param value 用于填充的值。
     * @param num 要填充的字节数。
     */
    void fillMemory(void* ptr, int value, size_t num);

    /**
     * @function compareMemory
     * @brief 比较两个内存区域。
     *
     * 此函数用于逐字节比较两个内存区域的内容。
     *
     * @param ptr1 第一个内存区域的起始地址。
     * @param ptr2 第二个内存区域的起始地址。
     * @param num 要比较的字节数。
     * @return 如果两个内存区域相等，则返回0；否则返回第一个不匹配字节的位置差。
     */
    int compareMemory(const void* ptr1, const void* ptr2, size_t num);

    /**
     * @function setMemoryProtection
     * @brief 设置指定内存区域的保护属性。
     *
     * 此函数用于更改指定内存区域的访问权限。
     *
     * @param addr 内存区域的起始地址。
     * @param len 内存区域的长度（以字节为单位）。
     * @param protection 欲设置的内存保护模式。
     * @return 成功时返回true，失败时返回false。
     */
    bool setMemoryProtection(void* addr, size_t len, MemoryProtection protection);

    /**
     * @function findPattern
     * @brief 在指定内存区域内查找模式。
     *
     * 此函数用于在一个较大的内存区域内搜索特定的字节序列（模式）。
     *
     * @param start 内存区域的起始地址。
     * @param size 内存区域的大小（以字节为单位）。
     * @param pattern 要查找的模式的起始地址。
     * @param patternSize 模式的大小（以字节为单位）。
     * @return 如果找到模式，则返回其在内存区域中的起始地址；否则返回nullptr。
     */
    void* findPattern(const void* start, size_t size, const void* pattern, size_t patternSize);
}