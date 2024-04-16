// Copyright (c) Facebook Technologies, LLC and its affiliates. All rights reserved.
rootProject.name = ("MetaOpenXRSDK")

val XrBodyFaceEyeSocialDir = File("XrSamples/XrBodyFaceEyeSocial/Projects/Android")

if (XrBodyFaceEyeSocialDir.exists()) {
  include(":XrBodyFaceEyeSocial")
  project(":XrBodyFaceEyeSocial").projectDir = XrBodyFaceEyeSocialDir
}

val XrColorSpaceFBDir = File("XrSamples/XrColorSpaceFB/Projects/Android")

if (XrColorSpaceFBDir.exists()) {
  include(":XrColorSpaceFB")
  project(":XrColorSpaceFB").projectDir = XrColorSpaceFBDir
}

val XrCompositor_NativeActivityDir = File("XrSamples/XrCompositor_NativeActivity/Projects/Android")

if (XrCompositor_NativeActivityDir.exists()) {
  include(":XrCompositor_NativeActivity")
  project(":XrCompositor_NativeActivity").projectDir = XrCompositor_NativeActivityDir
}

val XrControllersDir = File("XrSamples/XrControllers/Projects/Android")

if (XrControllersDir.exists()) {
  include(":XrControllers")
  project(":XrControllers").projectDir = XrControllersDir
}

val XrHandDataSourceDir = File("XrSamples/XrHandDataSource/Projects/Android")

if (XrHandDataSourceDir.exists()) {
  include(":XrHandDataSource")
  project(":XrHandDataSource").projectDir = XrHandDataSourceDir
}

val XrHandsAndControllersDir = File("XrSamples/XrHandsAndControllers/Projects/Android")

if (XrHandsAndControllersDir.exists()) {
  include(":XrHandsAndControllers")
  project(":XrHandsAndControllers").projectDir = XrHandsAndControllersDir
}

val XrHandsFBDir = File("XrSamples/XrHandsFB/Projects/Android")

if (XrHandsFBDir.exists()) {
  include(":XrHandsFB")
  project(":XrHandsFB").projectDir = XrHandsFBDir
}

val XrHandTrackingWideMotionModeDir =
    File("XrSamples/XrHandTrackingWideMotionMode/Projects/Android")

if (XrHandTrackingWideMotionModeDir.exists()) {
  include(":XrHandTrackingWideMotionMode")
  project(":XrHandTrackingWideMotionMode").projectDir = XrHandTrackingWideMotionModeDir
}

val XrInputDir = File("XrSamples/XrInput/Projects/Android")

if (XrInputDir.exists()) {
  include(":XrInput")
  project(":XrInput").projectDir = XrInputDir
}

val XrKeyboardDir = File("XrSamples/XrKeyboard/Projects/Android")

if (XrKeyboardDir.exists()) {
  include(":XrKeyboard")
  project(":XrKeyboard").projectDir = XrKeyboardDir
}

val XrPassthroughDir = File("XrSamples/XrPassthrough/Projects/Android")

if (XrPassthroughDir.exists()) {
  include(":XrPassthrough")
  project(":XrPassthrough").projectDir = XrPassthroughDir
}

val XrPassthroughOcclusionDir = File("XrSamples/XrPassthroughOcclusion/Projects/Android")

if (XrPassthroughOcclusionDir.exists()) {
  include(":XrPassthroughOcclusion")
  project(":XrPassthroughOcclusion").projectDir = XrPassthroughOcclusionDir
}

val XrSceneModelDir = File("XrSamples/XrSceneModel/Projects/Android")

if (XrSceneModelDir.exists()) {
  include(":XrSceneModel")
  project(":XrSceneModel").projectDir = XrSceneModelDir
}

val XrSpaceWarpDir = File("XrSamples/XrSpaceWarp/Projects/Android")

if (XrSpaceWarpDir.exists()) {
  include(":XrSpaceWarp")
  project(":XrSpaceWarp").projectDir = XrSpaceWarpDir
}

val XrSpatialAnchorDir = File("XrSamples/XrSpatialAnchor/Projects/Android")

if (XrSpatialAnchorDir.exists()) {
  include(":XrSpatialAnchor")
  project(":XrSpatialAnchor").projectDir = XrSpatialAnchorDir
}

val XrVirtualKeyboardDir = File("XrSamples/XrVirtualKeyboard/Projects/Android")

if (XrVirtualKeyboardDir.exists()) {
  include(":XrVirtualKeyboard")
  project(":XrVirtualKeyboard").projectDir = XrVirtualKeyboardDir
}
