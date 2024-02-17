# Copyright (c) Facebook Technologies, LLC and its affiliates. All rights reserved.

# create static library
add_library(
    samplecommon STATIC
    Src/GUI/ActionComponents.cpp
    Src/GUI/AnimComponents.cpp
    Src/GUI/CollisionPrimitive.cpp
    Src/GUI/DefaultComponent.cpp
    Src/GUI/Fader.cpp
    Src/GUI/GazeCursor.cpp
    Src/GUI/GuiSys.cpp
    Src/GUI/MetaDataManager.cpp
    Src/GUI/Reflection.cpp
    Src/GUI/ReflectionData.cpp
    Src/GUI/SoundLimiter.cpp
    Src/GUI/VRMenu.cpp
    Src/GUI/VRMenuComponent.cpp
    Src/GUI/VRMenuEvent.cpp
    Src/GUI/VRMenuEventHandler.cpp
    Src/GUI/VRMenuMgr.cpp
    Src/GUI/VRMenuObject.cpp
    Src/Input/ArmModel.cpp
    Src/Input/AxisRenderer.cpp
    Src/Input/ControllerRenderer.cpp
    Src/Input/Skeleton.cpp
    Src/Input/SkeletonRenderer.cpp
    Src/Input/TinyUI.cpp
    Src/Locale/OVR_Locale.cpp
    Src/Locale/tinyxml2.cpp
    Src/Misc/Log.c
    Src/Model/ModelAnimationUtils.cpp
    Src/Model/ModelCollision.cpp
    Src/Model/ModelFile_glTF.cpp
    Src/Model/ModelFile_OvrScene.cpp
    Src/Model/ModelFile.cpp
    Src/Model/ModelRender.cpp
    Src/Model/ModelTrace.cpp
    Src/Model/SceneView.cpp
    Src/OVR_BinaryFile2.cpp
    Src/OVR_FileSys.cpp
    Src/OVR_Lexer2.cpp
    Src/OVR_MappedFile.cpp
    Src/OVR_Stream.cpp
    Src/OVR_Uri.cpp
    Src/OVR_UTF8Util.cpp
    Src/PackageFiles.cpp
    Src/Render/BeamRenderer.cpp
    Src/Render/BitmapFont.cpp
    Src/Render/DebugLines.cpp
    Src/Render/EaseFunctions.cpp
    Src/Render/Egl.c
    Src/Render/GeometryBuilder.cpp
    Src/Render/GeometryRenderer.cpp
    Src/Render/GlBuffer.cpp
    Src/Render/GlGeometry.cpp
    Src/Render/GlProgram.cpp
    Src/Render/GlTexture.cpp
    Src/Render/PanelRenderer.cpp
    Src/Render/ParticleSystem.cpp
    Src/Render/PointList.cpp
    Src/Render/Ribbon.cpp
    Src/Render/SurfaceRender.cpp
    Src/Render/SurfaceTexture.cpp
    Src/Render/TextureAtlas.cpp
    Src/Render/TextureManager.cpp
    Src/System.cpp
)

# include directories
target_include_directories(
    samplecommon
    PUBLIC
        Src
        Src/GUI
        Src/Input
        Src/Locale
        Src/Misc
        Src/Model
        Src/Render
        ../1stParty/OVR/Include
        ../1stParty/utilities/include
        #### 3rd Party Headers from the OpenXR repo ####
        ../3rdParty/khronos/openxr/OpenXR-SDK/src/common
)
target_link_libraries(samplecommon PUBLIC minizip stb ktx)
target_compile_definitions(
    samplecommon PUBLIC $<IF:$<CONFIG:Debug>,OVR_BUILD_DEBUG=1,>
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        samplecommon
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wno-invalid-offsetof>
    )
endif()

if(WIN32)
    add_definitions(-DNOMINMAX)
    if(MSVC)
        target_compile_definitions(samplecommon PUBLIC /wd4267)
    endif()

    add_library(
        samplecommon_win32gl STATIC
        Src/Render/GlWrapperWin32.c
    )

    target_include_directories(
        samplecommon_win32gl
        PUBLIC
            ../3rdParty/glext
            ../1stParty/utilities/include
            ../1stParty/utilities/include/vulkan
            ../1stParty/utilities/include/utils
    )

    target_link_libraries(samplecommon_win32gl PUBLIC opengl32 gdi32 user32)
    target_link_libraries(samplecommon PUBLIC samplecommon_win32gl)
endif()
