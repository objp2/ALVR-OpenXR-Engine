# Copyright (c) Facebook Technologies, LLC and its affiliates. All rights reserved.

if(NOT TARGET OpenXR::openxr_loader)
    find_package(OpenXR REQUIRED)
endif()

add_library(
    samplexrframework STATIC
    Src/XrApp.cpp
    Src/Input/HandMaskRenderer.cpp
    Src/Input/HandRenderer.cpp
    Src/Render/Framebuffer.cpp
)

# Add include directories
target_include_directories(
    samplexrframework
    PUBLIC
        Src
        # Extra vendor-provided headers
        ../OpenXR/Include
)

target_link_libraries(
    samplexrframework
    PUBLIC
        minizip
        stb
        OpenXR::openxr_loader
        samplecommon
)

if(ANDROID)
    set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
    add_library(app_glue STATIC ${APP_GLUE_DIR}/android_native_app_glue.c)

    target_include_directories(samplexrframework PUBLIC ${APP_GLUE_DIR})
    target_compile_definitions(
        samplexrframework
        PUBLIC
            XR_OS_ANDROID=1
            XR_USE_PLATFORM_ANDROID=1
            XR_USE_GRAPHICS_API_OPENGL_ES=1
            ANDROID_NDK
            $<IF:$<CONFIG:Debug>,OVR_BUILD_DEBUG=1,>
    )
    target_compile_options(
        samplexrframework
        PUBLIC
            -Werror
            -Wall
            -Wextra
            -Wno-unused-parameter
            -Wno-missing-field-initializers
    )
    target_link_libraries(
        samplexrframework
        PRIVATE
            android
            EGL
            GLESv3
            log
            app_glue
    )
    if(USE_ASAN)
        target_compile_options(
            samplexrframework
            PUBLIC $<IF:$<CONFIG:Debug>,-fsanitize=address,>
                    $<IF:$<CONFIG:Debug>,-fno-omit-frame-pointer,>
        )
        target_link_options(
            samplexrframework PUBLIC
            $<IF:$<CONFIG:Debug>,-fsanitize=address,>
        )
    endif()
elseif(WIN32)
    target_compile_definitions(
        samplexrframework PUBLIC XR_OS_WINDOWS=1 XR_USE_PLATFORM_WIN32=1
                                    XR_USE_GRAPHICS_API_OPENGL=1
    )
endif()
