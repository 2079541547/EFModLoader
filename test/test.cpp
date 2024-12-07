/*******************************************************************************
 * 文件名称: test
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

#include <EFModLoader/loader/load.hpp>
#include <EFModLoader/Log.hpp>
#include <EFModLoader/hook/hooks.hpp>
#include <jni.h>
#include <dlfcn.h>
#include <unistd.h>
#include <EFModLoader/memory/tool.hpp>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);


    EFModLoader::Loader::vm = vm;
    EFModLoader::Loader::reserved = reserved;

    EFModLoader::Loader::loadModx("libtestMod.so", false);

    /*
    EFModLoader::Loader::loadMod(
            "libtestMod.so",
            "/sdcard/Android/data/eternal.future.efmodloader/files/libtestMod.so",
            "MyMod",
            "I",
            {}
            );
    */

    /*
    EFModLoader::Loader::uninstallMod("MyMod", "I");

    for (auto a: EFModLoader::Hooks::getHooksFunc({"A", "A", "A", "int", 1})) {
        EFLOG(EFModLoader::LogLevel::INFO, std::to_string((uintptr_t)a));
        EFModLoader::Hooks::callFunction<void>(a, std::string("Hello, World"));
    }
     */



    return JNI_VERSION_1_6;
}
