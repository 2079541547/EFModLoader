#include <jni.h>
#include <string>
#include <jni.h>
#include <BNM/Loading.hpp>
#include <zconf.h>
#include <EFModLoader/hook/unity/RegisterHook.hpp>
#include <EFModLoader/getData.hpp>
#include <EFModLoader/loader/LoadELFMods.hpp>
#include <EFModLoader/api/Redirect.hpp>
#include <EFModLoader/Android.hpp>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);


    EFModLoader::Android::Load(env, "TEFModLoader-EternalFuture");
    EFModLoader::Loader::LoadELFMods::LoadALLMod(*EFModLoader::Android::get_cacheDir + "EFMod/");


    return JNI_VERSION_1_6;
}
