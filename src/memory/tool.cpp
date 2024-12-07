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

#include <memory/tool.hpp>
#include <fstream>
#include <sys/mman.h>
#include <Log.hpp>
#include <cstring>
#ifdef WINDOWS_ENABLE
#include <windows.h>
#else
#include <sys/sysinfo.h>
#endif

namespace EFModLoader::Memory::Tool {
// 指针重定向
    void redirectPtr(void*& old_ptr, void* new_ptr) {
        // 直接将old_ptr指向new_ptr所指的新地址。
        old_ptr = new_ptr;
    }

// 内存导出到文件
    void dumpMemoryToFile(const std::filesystem::path &path,
                          const void *start,
                          size_t length) {

        // 检查输入参数的有效性
        if (start == nullptr || length == 0) {
            throw std::invalid_argument("Invalid memory range or length.");
        }

        // 打开文件以二进制写模式
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing.");
        }

        // 写入指定长度的内存数据到文件，并检查写操作是否成功
        file.write(static_cast<const char*>(start), length);
        if (!file) {
            throw std::runtime_error("Failed to write to file.");
        }
    }

// 分配内存
    void *allocateMemory(size_t size) {
        // 使用malloc分配内存并返回指针
        return malloc(size);
    }

// 释放内存
    void freeMemory(void *ptr) {
        // 使用free释放之前分配的内存块
        free(ptr);
    }

// 内存复制
    void *copyMemory(void *dest, const void *src, size_t num) {
        // 使用memcpy进行内存复制并返回目标地址
        return memcpy(dest, src, num);
    }

// 内存移动
    void *moveMemory(void *dest, const void *src, size_t num) {
        // 使用memmove进行内存移动（处理重叠区域）并返回目标地址
        return memmove(dest, src, num);
    }

// 内存填充
    void fillMemory(void *ptr, int value, size_t num) {
        // 使用memset用给定值填充指定数量的内存
        memset(ptr, value, num);
    }

// 内存比较
    int compareMemory(const void *ptr1, const void *ptr2, size_t num) {
        // 使用memcmp逐字节比较两个内存区域的内容
        return memcmp(ptr1, ptr2, num);
    }

// 设置内存保护
    bool setMemoryProtection(void *addr, size_t len, MemoryProtection protection) {
#ifdef WINDOWS_ENABLE
        DWORD oldProtect;
    DWORD newProtect = 0;

    // 根据枚举值设置Windows平台上的内存保护标志
    switch (protection) {
        case MemoryProtection::NoAccess:     newProtect = PAGE_NOACCESS; break;
        case MemoryProtection::ReadOnly:     newProtect = PAGE_READONLY; break;
        case MemoryProtection::ReadWrite:   newProtect = PAGE_READWRITE; break;
        case MemoryProtection::Execute:     newProtect = PAGE_EXECUTE; break;
        case MemoryProtection::ExecuteReadWrite: newProtect = PAGE_EXECUTE_READWRITE; break;
    }

    // 调用VirtualProtect更改内存保护属性
    return VirtualProtect(addr, len, newProtect, &oldProtect) != 0;
#else
        int prot = 0;

        // 根据枚举值设置POSIX平台上的内存保护标志
        switch (protection) {
            case MemoryProtection::NoAccess:     prot = PROT_NONE; break;
            case MemoryProtection::ReadOnly:     prot = PROT_READ; break;
            case MemoryProtection::ReadWrite:   prot = PROT_READ | PROT_WRITE; break;
            case MemoryProtection::Execute:     prot = PROT_EXEC; break;
            case MemoryProtection::ExecuteReadWrite: prot = PROT_READ | PROT_WRITE | PROT_EXEC; break;
        }

        // 调用mprotect更改内存保护属性
        return mprotect(addr, len, prot) == 0;
#endif
    }

// 内存查找
    void *findPattern(const void *start, size_t size, const void *pattern,
                      size_t patternSize) {
        // 检查输入参数的有效性
        if (patternSize == 0 || patternSize > size) {
            return nullptr;
        }

        // 将输入转换为unsigned char指针以便于操作
        const unsigned char* data = static_cast<const unsigned char*>(start);
        const unsigned char* pat = static_cast<const unsigned char*>(pattern);

        // 遍历内存区域，尝试找到匹配的模式
        for (size_t i = 0; i <= size - patternSize; ++i) {
            if (memcmp(&data[i], pat, patternSize) == 0) {
                // 如果找到匹配项，则返回其在内存区域中的位置
                return const_cast<void*>(static_cast<const void*>(&data[i]));
            }
        }

        // 如果没有找到匹配项，则返回nullptr
        return nullptr;
    }

// 打印内存信息
    void printMemoryInfo() {
#ifdef WINDOWS_ENABLE
        MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    // 输出全局内存信息
    EFLOG(LogLevel::INFO, "总内存: " + std::to_string(memInfo.ullTotalPhys / (1024 * 1024)) + " MB");
    EFLOG(LogLevel::INFO, "空闲内存: " + std::to_string(memInfo.ullAvailPhys / (1024 * 1024)) + " MB");

    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // 输出当前进程的内存使用信息
        EFLOG(LogLevel::INFO, "进程使用的页文件: " + std::to_string(pmc.PrivateUsage / (1024 * 1024)) + " MB");
        EFLOG(LogLevel::INFO, "工作集大小: " + std::to_string(pmc.WorkingSetSize / (1024 * 1024)) + " MB");
        EFLOG(LogLevel::INFO, "峰值工作集大小: " + std::to_string(pmc.PeakWorkingSetSize / (1024 * 1024)) + " MB");
    }
#else
        struct sysinfo memInfo;
        sysinfo(&memInfo);

        long long totalRAM = memInfo.totalram;
        totalRAM += memInfo.totalswap;
        totalRAM *= memInfo.mem_unit;

        // 输出系统内存信息
        EFLOG(LogLevel::INFO, "总内存: " + std::to_string(totalRAM / (1024 * 1024)) + " MB");
        EFLOG(LogLevel::INFO, "空闲内存: " + std::to_string(memInfo.freeram * memInfo.mem_unit / (1024 * 1024)) + " MB");
        EFLOG(LogLevel::INFO, "共享内存: " + std::to_string(memInfo.sharedram * memInfo.mem_unit / (1024 * 1024)) + " MB");
        EFLOG(LogLevel::INFO, "缓冲内存: " + std::to_string(memInfo.bufferram * memInfo.mem_unit / (1024 * 1024)) + " MB");
        EFLOG(LogLevel::INFO, "总交换分区: " + std::to_string(memInfo.totalswap * memInfo.mem_unit / (1024 * 1024)) + " MB");
        EFLOG(LogLevel::INFO, "空闲交换分区: " + std::to_string(memInfo.freeswap * memInfo.mem_unit / (1024 * 1024)) + " MB");

        // 读取/proc/self/status文件以获取更详细的内存使用信息
        std::ifstream status("/proc/self/status");
        std::string line;
        while (std::getline(status, line)) {
            if (line.find("VmSize:") != std::string::npos ||
                line.find("VmRSS:") != std::string::npos ||
                line.find("VmData:") != std::string::npos ||
                line.find("VmStk:") != std::string::npos ||
                line.find("VmExe:") != std::string::npos ||
                line.find("VmLib:") != std::string::npos ||
                line.find("VmPTE:") != std::string::npos) {
                EFLOG(LogLevel::INFO, line.c_str());
            }
        }
#endif
    }
}