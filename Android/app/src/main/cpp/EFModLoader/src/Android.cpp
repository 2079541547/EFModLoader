//
// Created by eternalfuture on 2024/11/1.
//

#include <iostream>
#include <EFModLoader/log.hpp>
#include <EFModLoader/Android.hpp>
#include <EFModLoader/getData.hpp>
#include <EFModLoader/EFMod/EFMod.hpp>
#include <EFModLoader/api/RegisterApi.hpp>
#include <EFModLoader/api/Redirect.hpp>
#include <EFModLoader/hook/unity/RegisterHook.hpp>
#include <EFModLoader/loader/LoadELFMods.hpp>
#include <filesystem>

namespace EFModLoader::Android {

    void clearDirectory(const filesystem::path& dirPath) {
        if (filesystem::exists(dirPath)) {
            for (const auto& entry : filesystem::directory_iterator(dirPath)) {
                if (filesystem::is_regular_file(entry.status())) {
                    filesystem::remove(entry.path());
                }
            }
        } else {
            std::cerr << "Directory does not exist: " << dirPath << '\n';
        }
    }

    void copyFilesFromTo(const filesystem::path& sourceDir, const filesystem::path& destDir) {
        if (filesystem::exists(sourceDir)) {
            for (const auto& entry : filesystem::directory_iterator(sourceDir)) {
                if (filesystem::is_regular_file(entry.status())) {
                    filesystem::copy(entry.path(), destDir / entry.path().filename(),
                                     filesystem::copy_options::overwrite_existing);
                }
            }
        } else {
            std::cerr << "Source directory does not exist: " << sourceDir << '\n';
        }
    }

    void Load(JNIEnv *env, string EFModLoader) {

        if (filesystem::exists( "/sdcard/EFModLoader/" + EFModLoader)) {

            if (filesystem::is_directory("/sdcard/EFModLoader/" + EFModLoader)) {

                auto* get_PackageName = new std::string(EFModLoader::getPackageNameAsString(env));
                auto* get_cacheDir = new std::string("data/data/" + *get_PackageName + "/cache/");
                auto* get_ExternalDir = new std::string("/sdcard/EFModLoader/" + EFModLoader + "/EFMod-Private/");

                if (filesystem::exists("/sdcard/EFModLoader/" + EFModLoader + "/kernel/loader")) {
                    EFLOG(LogLevel::INFO, "EFModLoader", "Android", "Load", "尝试更新内核");
                    filesystem::copy_file("/sdcard/EFModLoader/" + EFModLoader + "/kernel/", *get_cacheDir + "EFModLoader/loader", filesystem::copy_options::overwrite_existing);
                    EFLOG(LogLevel::INFO, "EFModLoader", "Android", "Load", "内核更新完成");
                }

                clearDirectory(*get_cacheDir + "EFMod/");
                copyFilesFromTo("/sdcard/EFModLoader/" + EFModLoader + "/EFMods/", *get_cacheDir + "EFMod/");
                clearDirectory("/sdcard/EFModLoader/" + EFModLoader + "/EFMods/");

                EFLOG(LogLevel::INFO, "EFModLoader", "Android", "Load", "使用外部加载");

                EFModLoader::RegisterApi::RegisterAPI("get_PackageName", (long) get_PackageName);
                EFModLoader::RegisterApi::RegisterAPI("get_ExternalDir", (long) get_ExternalDir);
                EFModLoader::RegisterApi::RegisterAPI("get_cacheDir", (long) get_cacheDir);

                EFModLoader::Loader::LoadELFMods::LoadALLMod(*get_cacheDir + "EFMod/");
            } else {
                EFLOG(LogLevel::ERROR, "EFModLoader", "Android", "Load", "加载的目录不是文件夹！！！");
            }
        } else {
            auto* get_PackageName = new std::string(EFModLoader::getPackageNameAsString(env));
            auto* get_cacheDir = new std::string("data/data/" + *get_PackageName + "/cache/");
            auto* get_ExternalDir = new std::string("/sdcard/Android/data/" + *get_PackageName +"/files/EFMod-Private/");

            EFLOG(LogLevel::INFO, "EFModLoader", "Android", "Load", "使用内部加载");

            EFModLoader::RegisterApi::RegisterAPI("get_PackageName", (long) get_PackageName);
            EFModLoader::RegisterApi::RegisterAPI("get_ExternalDir", (long) get_ExternalDir);
            EFModLoader::RegisterApi::RegisterAPI("get_cacheDir", (long) get_cacheDir);

            EFModLoader::Loader::LoadELFMods::LoadALLMod(*get_cacheDir + "EFMod/");
        }

    }
}