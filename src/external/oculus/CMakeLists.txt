# Copyright (c) Facebook Technologies, LLC and its affiliates. All rights reserved.

cmake_minimum_required(VERSION 3.10.2)

project(MetaOpenXRSDK C CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(ANDROID)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(PREBUILT_LIB_PATH ${CMAKE_SOURCE_DIR}/OpenXR/Libs/Android/${ANDROID_ABI}/Debug)
    else()
        set(PREBUILT_LIB_PATH ${CMAKE_SOURCE_DIR}/OpenXR/Libs/Android/${ANDROID_ABI}/Release)
    endif()
    # Add the prebuilt library
    add_library(OpenXR::openxr_loader SHARED IMPORTED)
    # Set the library's properties
    set_target_properties(OpenXR::openxr_loader PROPERTIES
        IMPORTED_LOCATION ${PREBUILT_LIB_PATH}/libopenxr_loader.so
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/3rdParty/khronos/openxr/OpenXR-SDK/include
    )
endif()

add_subdirectory(3rdParty)
add_subdirectory(OpenXR)
add_subdirectory(SampleCommon)
add_subdirectory(SampleXrFramework)
add_subdirectory(XrSamples)
