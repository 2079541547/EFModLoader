/*******************************************************************************
 * 文件名称: mod
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

#include <iostream>
#include "../../include/EFModLoader/EFMod/EFMod.hpp"
#include <android/log.h>
#include <jni.h>

bool EFMod::initialization() {
    return true;
}

void Hello(std::string name) {
    __android_log_print(ANDROID_LOG_INFO, "SilkCasket", "%s", name.c_str());
}

extern "C" std::map<uint64_t, std::vector<void*>> get_hooks() {
    EFMod::registerHook({"A", "A", "A", "int", 1},
                        (void*)Hello); //注册一个Hook，不过推荐定义为函数一键注册
    return EFMod::hooks; //返回hook表
}

extern "C" void CreateMod() {
    __android_log_print(ANDROID_LOG_INFO, "SilkCasket", "%s", private_path.c_str());
    EFMod::initialization();
}








JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    __android_log_print(ANDROID_LOG_INFO, "SilkCasket", "独立Mod加载测试");
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
