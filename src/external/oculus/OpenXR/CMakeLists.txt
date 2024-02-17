# Copyright (c) Facebook Technologies, LLC and its affiliates. All rights reserved.

if(NOT TARGET OpenXR::openxr_loader)
    include(FetchContent)

    FetchContent_Declare(
        openxr
        GIT_REPOSITORY  https://github.com/KhronosGroup/OpenXR-SDK.git
        GIT_TAG         release-1.0.32
        SOURCE_DIR      ${CMAKE_CURRENT_LIST_DIR}/openxr
    )

    FetchContent_GetProperties(openxr)
    if(NOT openxr_POPULATED)
        FetchContent_Populate(openxr)
    endif()

    # Common compile definitions
    add_subdirectory(${openxr_SOURCE_DIR} ${openxr_BINARY_DIR})
endif()
