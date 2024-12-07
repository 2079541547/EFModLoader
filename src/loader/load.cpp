/*******************************************************************************
 * 文件名称: load
 * 项目名称: EFModLoader
 * 创建时间: 2024/12/6
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

#include <loader/load.hpp>
#include <filesystem>
#include <Log.hpp>
#include <map>
#include <api/Field.hpp>
#include <api/Func.hpp>
#include <hook/hooks.hpp>
#include <SilkHash64.hpp>

#if WINDOWS_ENABLE
#include <windows.h>
#else
#include <dlfcn.h>
#endif

/**
 * @brief 定义一个函数指针类型，该类型指向一个不带参数且返回void（无返回值）的函数。
 *
 * 这个函数指针可以用来指向任何满足以下条件的函数：
 * - 函数没有参数。
 * - 函数执行某些操作但不返回任何结果。
 *
 * 例如，这种类型的函数可能被用来初始化模块、创建对象等，
 * 并且当它被调用时，它会执行其内部定义的操作。
 */
typedef void (*CreateMod)();

/**
 * @brief 定义一个函数指针类型，该类型指向一个不带参数并返回std::map<uint64_t, std::vector<void *>>类型的函数。
 *
 * 这个函数指针可以用来指向任何满足以下条件的函数：
 * - 函数没有参数。
 * - 函数返回一个映射表，其中键是uint64_t类型的整数，而值是一个void指针的向量。
 *
 * uint64_t 键可能代表唯一的标识符或者地址，而void* 向量则可能表示与这些标识符相关的钩子函数指针集合。
 * GetHooks 类型的函数可能是用来获取一组预先注册的钩子函数，它们可以在特定事件发生时被调用。
 *
 * 注意：使用void*指针意味着失去类型安全性，程序员需要确保在转换回原始类型时保持正确的类型信息。
 */
typedef std::map<uint64_t, std::vector<void *>> (*GetHooks)();


void EFModLoader::Loader::uninstallMod(const std::string &ModName, const std::string &Author) {
    // 创建模组的唯一标识符，通过将模组名称和作者名连接起来并进行哈希计算。
    auto modId = SilkHash64::hashString(ModName + Author);

    // 在runtime容器中查找具有匹配ID的模组。
    auto it = std::find_if(runtime.begin(), runtime.end(), [modId](const Mod& a) {
        return a.id == modId;
    });

    if (it != runtime.end()) {
        // 如果找到了模组，先移除它关联的钩子。
        // 假设 hookVector_Mod 是一个全局或类成员变量，存储了所有模组的钩子。
        // 这里我们根据模组的hookV索引在钩子向量中定位并移除对应的钩子。
        EFModLoader::Hooks::hookVector_Mod.erase(EFModLoader::Hooks::hookVector_Mod.begin() + it->hookV);

        // 根据平台不同，释放动态加载的库资源。
#ifdef WINDOWS_ENABLE
        // Windows平台上，如果模组库已经被加载，则调用FreeLibrary释放库。
        if (it->dlopen != NULL) {
            FreeLibrary(static_cast<HMODULE>(it->dlopen));
        }
#else
        // 非Windows平台（如Linux、macOS等），使用dlclose释放库。
        dlclose(it->dlopen);
#endif

        // 从运行时列表中移除模组。
        runtime.erase(it);

        // 记录成功卸载模组的信息日志。
        EFLOG(LogLevel::INFO, "EFModLoader::Loader", "uninstallMod", "成功卸载Mod：" + std::to_string(modId));
    } else {
        // 如果未找到要卸载的模组，记录一条警告日志。
        EFLOG(LogLevel::WARN, "EFModLoader::Loader", "uninstallMod", "未找到要卸载的Mod：" + std::to_string(modId));
    }
}


void EFModLoader::Loader::loadMod(const std::string& path,
                                  const std::string& private_path,
                                  const std::string& ModName,
                                  const std::string& Author,
                                  const std::map<std::string, std::string>& Hash) {

    // 创建模组的唯一标识符，通过将模组名称和作者名连接起来并进行哈希计算。
    uint64_t modId = SilkHash64::hashString(ModName + Author);

#ifdef WINDOWS_ENABLE
    // 在Windows平台上加载动态链接库。
    HMODULE Mod = LoadLibraryA(path.c_str());
    if (!Mod) {
        // 如果加载失败，记录错误日志并返回。
        EFLOG(LogLevel::ERROR, "EFModLoader::Loader", "loadMod",
              "加载Mod失败：" + path + " 错误：" + std::to_string(GetLastError()));
        return;
    }

#else
    // 在非Windows平台（如Linux、macOS等）上加载共享库。
    void *Mod = dlopen(path.c_str(), RTLD_LAZY);
    if (!Mod) {
        // 如果加载失败，记录错误日志并返回。
        EFLOG(LogLevel::ERROR, "EFModLoader::Loader", "loadMod",
              "加载Mod失败：" + path + " 错误：" + std::string(dlerror()));
        return;
    }
#endif

    try {
        // 设置符号地址为全局变量的引用，以供模组使用。
        // 注意：这种方式直接修改外部模块的内存布局，存在潜在的风险和不确定性。
        // 这里假设FieldMap, FuncMap, private_path, ModName, Author, Hash都是模组期望接收的全局变量。
        // 使用GetProcAddress或dlsym获取这些变量的地址，并设置它们指向正确的实例。
#ifdef WINDOWS_ENABLE
        *(std::map<uint64_t, void*> **) GetProcAddress(Mod, "FieldMap") = &EFModLoader::API::Field::FieldMap;
        *(std::map<uint64_t, void*> **) GetProcAddress(Mod, "FuncMap") = &EFModLoader::API::Func::FuncMap;
        *(std::filesystem::path *) GetProcAddress(Mod, "private_path") = std::filesystem::path(private_path);
        *(std::string *) GetProcAddress(Mod, "ModName") = ModName;
        *(std::string *) GetProcAddress(Mod, "Author") = Author;
        *(std::map<std::string, std::string> *) GetProcAddress(Mod, "Hash") = Hash;

        // 获取模组提供的入口点函数指针。
        auto createMod = reinterpret_cast<CreateMod>(GetProcAddress(Mod, "CreateMod"));
        auto get_hooks = reinterpret_cast<GetHooks>(GetProcAddress(Mod, "get_hooks"));
#else
        *(std::map<uint64_t, void*> **) dlsym(Mod, "FieldMap") = &EFModLoader::API::Field::FieldMap;
        *(std::map<uint64_t, void*> **) dlsym(Mod, "FuncMap") = &EFModLoader::API::Func::FuncMap;
        *(std::filesystem::path *) dlsym(Mod, "private_path") = std::filesystem::path(private_path);
        *(std::string *) dlsym(Mod, "ModName") = ModName;
        *(std::string *) dlsym(Mod, "Author") = Author;
        *(std::map<std::string, std::string> *) dlsym(Mod, "Hash") = Hash;

        // 获取模组提供的入口点函数指针。
        auto createMod = reinterpret_cast<CreateMod>(dlsym(Mod, "CreateMod"));
        auto get_hooks = reinterpret_cast<GetHooks>(dlsym(Mod, "get_hooks"));
#endif

        // 检查是否找到了必要的入口点函数。
        if (!createMod || !get_hooks) {
            throw std::runtime_error("未能找到必要的入口点");
        }

        // 调用模组的初始化函数。
        createMod();

        // 将模组提供的钩子添加到全局钩子向量中。
        EFModLoader::Hooks::hookVector_Mod.push_back(get_hooks());

        // 将模组信息添加到运行时列表中。
        runtime.push_back({modId, EFModLoader::Hooks::hookVector_Mod.size() - 1, Mod});

        // 记录成功加载模组的信息日志。
        EFLOG(LogLevel::INFO, "EFModLoader::Loader", "loadMod", "成功加载Mod：" + path);
    } catch (const std::exception& e) {
        // 如果加载过程中发生异常，记录错误日志并清理资源。
        EFLOG(LogLevel::ERROR, "EFModLoader::Loader", "loadMod",
              "加载Mod：" + path + "时发生错误：" + e.what());

        // 清理加载的动态库资源。
#ifdef WINDOWS_ENABLE
        FreeLibrary(static_cast<HMODULE>(Mod));
#else
        dlclose(Mod);
#endif
    }
}



void EFModLoader::Loader::loadModx(const std::string& path, bool autoClose) {

#ifdef WINDOWS_ENABLE
    // 在Windows平台上加载动态链接库。
    HMODULE Mod = LoadLibraryA(path.c_str());
    if (!Mod) {
        // 如果加载失败，记录错误日志并返回。
        EFLOG(LogLevel::ERROR, "EFModLoader::Loader", "loadModx",
              "加载Mod失败：" + path + " 错误：" + std::to_string(GetLastError()));
        return;
    }

    // 定义OnLoad函数指针类型，并尝试获取模组的入口点。
    typedef int (*OnLoad)();
    OnLoad onLoad;

#if ANDROID_ENABLE
    // 编译时错误：ANDROID_ENABLE和WINDOWS_ENABLE不能同时启用。
    #error "ANDROID_ENABLE is not compatible with WINDOWS_ENABLE"
#else
    // 对于非Android平台（包括Windows），使用GetProcAddress获取入口点。
    onLoad = (OnLoad) GetProcAddress(Mod, "OnLoad");
#endif

#else
    // 在非Windows平台（如Linux、macOS等）上加载共享库。
    void *Mod = dlopen(path.c_str(), RTLD_LAZY);
    if (!Mod) {
        // 如果加载失败，记录错误日志并返回。
        EFLOG(LogLevel::ERROR, "EFModLoader::Loader", "loadModx",
              "加载Mod失败：" + path + " 错误：" + dlerror());
        return;
    }

    // 定义OnLoad函数指针类型，并初始化为nullptr。
    typedef int (*OnLoad)();
    OnLoad onLoad = nullptr;

#if ANDROID_ENABLE
    // 对于Android平台，尝试获取JNI_OnLoad入口点。
    typedef jint (*JNI_OnLoad)(JavaVM *vm, void *reserved);
    JNI_OnLoad onLoadJNI = (JNI_OnLoad) dlsym(Mod, "JNI_OnLoad");

    // 调用JNI_OnLoad函数，传递必要的JNI环境参数。
    // 注意：这里假设vm和reserved已经在其他地方定义或初始化。
    int a = (int)onLoadJNI(vm, reserved);

#else
    // 对于非Android平台，尝试获取OnLoad入口点。
    onLoad = (OnLoad) dlsym(Mod, "OnLoad");
    int a = onLoad();
#endif

#endif

    try {
        // 检查是否找到了必要的入口点函数。
        if (!onLoad) {
            throw std::runtime_error("未能找到必要的入口点");
        }

        // 记录加载结果。
        EFLOG(LogLevel::INFO, "EFModLoader", "Loader", "loadModx", "加载结果：" + std::to_string(a));
    } catch (const std::exception& e) {
        // 如果加载过程中发生异常，记录错误日志并清理资源。
        EFLOG(LogLevel::ERROR, "EFModLoader::Loader", "loadModx",
              "加载Mod：" + path + "时发生错误：" + e.what());

        // 清理加载的动态库资源。
#ifdef WINDOWS_ENABLE
        FreeLibrary(static_cast<HMODULE>(Mod));
#else
        dlclose(Mod);
#endif
        return;
    }

    // 如果设置了autoClose为true，在加载完成后立即关闭模组。
    if (autoClose) {
        EFLOG(LogLevel::INFO, "EFModLoader::Loader", "loadModx", "正在关闭Mod：" + path);

#ifdef WINDOWS_ENABLE
        FreeLibrary(static_cast<HMODULE>(Mod));
#else
        dlclose(Mod);
#endif
    }
}