/************************************************************************************************
Filename    :   SimpleGlbRenderer.h
Content     :   A one stop for models from the render model extension
Created     :   April 2021
Authors     :   Federico Schliemann
Copyright   :   Copyright (c) Facebook Technologies, LLC and its affiliates. All rights reserved.
************************************************************************************************/
#pragma once

#include <memory>
#include <string>
#include <vector>

/// Sample Framework
#include "Misc/Log.h"
#include "Model/SceneView.h"
#include "Render/GlProgram.h"
#include "Render/SurfaceRender.h"
#include "OVR_FileSys.h"
#include "OVR_Math.h"

namespace OVRFW {

class SimpleGlbRenderer {
   public:
    SimpleGlbRenderer() = default;
    ~SimpleGlbRenderer() = default;

    enum UpdateOffset { UpdateOffset_None, UpdateOffset_Grip };

    bool Init(std::vector<uint8_t>& modelBuffer);
    void Shutdown();
    void Update(const OVR::Posef& pose, UpdateOffset updateOffset = UpdateOffset_None);
    void Render(std::vector<ovrDrawSurface>& surfaceList);
    bool IsInitialized() const {
        return Initialized;
    }

   public:
    OVR::Vector3f SpecularLightDirection;
    OVR::Vector3f SpecularLightColor;
    OVR::Vector3f AmbientLightColor;
    bool UseSolidTexture = true;
    float Opacity = 1.0f;

   private:
    bool Initialized = false;
    float AlphaBlendFactor = 1.0f;
    GlProgram ProgRenderModel;
    std::unique_ptr<ModelFile> RenderModel{};
    GlTexture RenderModelTextureSolid;
    OVR::Matrix4f Transform;
    OVR::Posef GripPose = OVR::Posef::Identity();
};

} // namespace OVRFW
