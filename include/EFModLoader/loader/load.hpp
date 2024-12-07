/*******************************************************************************
 * 文件名称: load
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
#include <vector>
#include <map>
#include <cstdint>

#if ANDROID_ENABLE
#include <jni.h>
#endif

namespace EFModLoader::Loader {

#if ANDROID_ENABLE
    /**
     * @var vm
     * @brief 全局Java虚拟机实例指针。
     *
     * 这个变量用于存储指向当前应用程序使用的JavaVM实例的指针。
     * 在Android平台上，JNI接口需要访问JavaVM实例来调用Java方法或处理回调。
     * 注意：此变量仅在定义了ANDROID_ENABLE宏时有效。
     */
    inline JavaVM *vm;

    /**
     * @var reserved
     * @brief 保留参数，通常用于JNI接口中。
     *
     * 在JNI接口中，`reserved`参数通常作为占位符提供，但目前并未被使用。
     * 它是JNI_OnLoad函数签名的一部分，但在大多数情况下可以传入nullptr。
     * 注意：此变量仅在定义了ANDROID_ENABLE宏时有效。
     */
    inline void *reserved;
#endif

    /**
     * @struct Mod
     * @brief 表示一个已加载模组的数据结构。
     *
     * 此结构体保存了关于单个模组的关键信息，包括其唯一标识符、钩子向量索引以及动态库句柄。
     * 模组管理器使用这些信息来跟踪和管理所有已加载的模组。
     */
    struct Mod {
        /**
         * @var id
         * @brief 模组的唯一标识符。
         *
         * 这是一个64位无符号整数，用于唯一标识一个模组。它通常是通过哈希模组名称和作者信息生成的。
         */
        uint64_t id;

        /**
         * @var hookV
         * @brief 钩子向量中的索引位置。
         *
         * 这个值表示模组在其关联的钩子向量中的索引位置，以便能够快速定位并操作模组相关的钩子。
         */
        size_t hookV;

        /**
         * @var dlopen
         * @brief 动态库句柄。
         *
         * 这个指针保存了加载模组时返回的动态库句柄，允许稍后释放该库资源。
         * 在Windows上，它是HMODULE类型；在其他平台上，它是void*类型。
         */
        void* dlopen;
    };

    /**
     * @var runtime
     * @brief 存储当前运行时环境中所有已加载模组的容器。
     *
     * 这是一个全局的vector容器，用于保存所有已加载模组的信息。
     * 每个元素都是一个Mod结构体实例，包含了有关该模组的所有必要信息。
     */
    inline std::vector<Mod> runtime;

    /**
     * @brief 卸载指定名称和作者的模组。
     *
     * 该方法用于从当前运行时环境中卸载一个特定的模组（mod），通过给定的模组名称和作者信息来识别要卸载的模组。
     * 模组的唯一标识符是通过组合模组名称和作者，并对其进行哈希运算得到的64位整数。
     * 如果找到对应的模组，将会移除与之关联的钩子，并释放动态加载的库资源。
     * 如果没有找到对应的模组，则会记录警告信息。
     *
     * @param ModName 模组的名称字符串。
     * @param Author 模组作者的名称字符串。
     */
    void uninstallMod(const std::string& ModName,
                      const std::string& Author);


    /**
     * @brief 加载指定路径下的模组。
     *
     * 该方法用于加载一个模组到当前运行时环境中，通过给定的模组文件路径、私有路径、模组名称、作者信息和哈希表来初始化模组。
     * 模组的唯一标识符是通过组合模组名称和作者，并对其进行哈希运算得到的64位整数。
     * 如果成功加载了模组，则会执行模组提供的初始化函数，并添加其钩子到全局钩子向量中。
     * 如果加载失败或过程中发生错误，则记录相应的错误日志并清理资源。
     *
     * @param path 模组文件的完整路径。
     * @param private_path 模组所需的私有数据路径。
     * @param ModName 模组的名称字符串。
     * @param Author 模组作者的名称字符串。
     * @param Hash 包含模组相关信息的键值对映射。
     */
    void loadMod(const std::string& path,
                 const std::string& private_path,
                 const std::string& ModName,
                 const std::string& Author,
                 const std::map<std::string, std::string>& Hash);

    /**
     * @brief 加载指定路径下的模组，并根据参数决定是否自动关闭。
     *
     * 该方法用于加载一个模组到当前运行时环境中，通过给定的模组文件路径。
     * 如果成功加载了模组，则会尝试调用模组提供的`OnLoad`初始化函数。
     * 如果设置了`autoClose`为true，在加载完成后会立即关闭模组。
     * 此外，该函数还处理Android平台上的JNI接口加载。
     *
     * @param path 模组文件的完整路径。
     * @param autoClose 是否在加载后自动关闭模组。
     */
    void loadModx(const std::string& path, bool autoClose);
}