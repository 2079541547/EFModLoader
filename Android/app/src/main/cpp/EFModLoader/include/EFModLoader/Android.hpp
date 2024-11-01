//
// Created by eternalfuture on 2024/11/1.
//

#pragma once

#include <iostream>
#include <jni.h>

namespace EFModLoader::Android {

    using namespace std;

    void Load(JNIEnv *env, string EFModLoader);

}