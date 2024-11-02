# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("efmodloader")


set(DOBBY_LIB ${CMAKE_CURRENT_SOURCE_DIR}/EFModLoader/DependencyLibrary/Dobby/${CMAKE_ANDROID_ARCH_ABI}/libdobby.a)

set(BNM_INCLUDE_DIRS
        EFModLoader/DependencyLibrary/BNM/include
        EFModLoader/DependencyLibrary/BNM/external/include
        EFModLoader/DependencyLibrary/BNM/external
        EFModLoader/DependencyLibrary/BNM/external/utf8
        EFModLoader/DependencyLibrary/BNM/src/private
        EFModLoader/DependencyLibrary/Dobby
)

add_library(
        BNM
        STATIC
        EFModLoader/DependencyLibrary/BNM/src/Class.cpp
        EFModLoader/DependencyLibrary/BNM/src/ClassesManagement.cpp
        EFModLoader/DependencyLibrary/BNM/src/Coroutine.cpp
        EFModLoader/DependencyLibrary/BNM/src/Defaults.cpp
        EFModLoader/DependencyLibrary/BNM/src/Delegates.cpp
        EFModLoader/DependencyLibrary/BNM/src/EventBase.cpp
        EFModLoader/DependencyLibrary/BNM/src/Exceptions.cpp
        EFModLoader/DependencyLibrary/BNM/src/FieldBase.cpp
        EFModLoader/DependencyLibrary/BNM/src/Hooks.cpp
        EFModLoader/DependencyLibrary/BNM/src/Image.cpp
        EFModLoader/DependencyLibrary/BNM/src/Internals.cpp
        EFModLoader/DependencyLibrary/BNM/src/Loading.cpp
        EFModLoader/DependencyLibrary/BNM/src/MethodBase.cpp
        EFModLoader/DependencyLibrary/BNM/src/MonoStructures.cpp
        EFModLoader/DependencyLibrary/BNM/src/PropertyBase.cpp
        EFModLoader/DependencyLibrary/BNM/src/UnityStructures.cpp
        EFModLoader/DependencyLibrary/BNM/src/Utils.cpp
)

target_include_directories(
        BNM
        PUBLIC
        ${BNM_INCLUDE_DIRS}
)

target_link_libraries(
        BNM
        PUBLIC
        ${DOBBY_LIB}
        log
)



set(EFMODLOADER_INCLUDE_DIRS
        EFModLoader/include
        EFModLoader/src
)

add_library(
        EFModLoader
        STATIC
        EFModLoader/src/log.cpp
        EFModLoader/src/api/RegisterAPI.cpp
        EFModLoader/src/hook/unity/RegisterHook.cpp
        EFModLoader/src/loader/LoadELFMods.cpp
        EFModLoader/src/Android.cpp
        EFModLoader/src/getData.cpp
)

target_include_directories(
        EFModLoader
        PUBLIC
        ${EFMODLOADER_INCLUDE_DIRS}
)

target_link_libraries(
        EFModLoader
        PUBLIC
        log
        android
        BNM
)









add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        EFModLoader
        log)