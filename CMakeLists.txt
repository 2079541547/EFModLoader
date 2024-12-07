cmake_minimum_required(VERSION 3.22.1)

set(EFMOD_WINDOWS OFF)
set(EFMOD_ANDROID ON)
set(BUILD_TEST ON)


if(${EFMOD_WINDOWS})
    add_definitions(-DWINDOWS_ENABLE)
endif()

if(${EFMOD_ANDROID})
    add_definitions(-DANDROID_ENABLE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -flto")

add_library(EFModLoader STATIC
        src/Log.cpp
        src/loader/load.cpp
        src/api/Field.cpp
        src/api/Func.cpp
        src/hook/hooks.cpp
        src/memory/tool.cpp
)

target_include_directories(
        EFModLoader
        PUBLIC
        include

        PRIVATE
        include/EFModLoader
)

if (${EFMOD_ANDROID})
    target_link_libraries(EFModLoader
            PUBLIC
            android
            log
            )
endif ()

if (${BUILD_TEST})

    add_library(test
        SHARED
        test/test.cpp
    )

    target_link_libraries(test
            EFModLoader
    )

    add_library(testMod
            SHARED
            test/mod/mod.cpp
    )

    target_link_libraries(testMod
            android
            log
    )

endif ()