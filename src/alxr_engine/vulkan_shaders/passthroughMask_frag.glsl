#version 460
#ifdef ENABLE_ARB_INCLUDE_EXT
    #extension GL_ARB_shading_language_include : require
#else
    // required by glslangValidator
    #extension GL_GOOGLE_include_directive : require
#endif
#pragma fragment

#include "common/baseVideoFrag.glsl"
#include "common/color-functions.glsl" //added from Rachmanin0xF github

vec3 RGB_TO_LAB(vec3 rgb) {
    return XYZ_TO_LAB(RGB_TO_XYZ(rgb));
}

float when_lt(float x, float y) {
    return max(sign(y - x), 0.0);
}

layout(location = 0) out vec4 FragColor;
const vec3 KeyColor_LAB = vec3(21.0486,-5.2067,21.8668); //Modify as you wish in LAB format this code is between brown/green/yellow

void main()
{
  
    vec4 sampleRGB = SampleVideoTexture();
    vec3 sampleLAB = RGB_TO_LAB(sampleRGB.rgb);
    float deltaE = LAB_DELTA_E_CIE2000(sampleLAB, KeyColor_LAB);
    sampleRGB.a -= when_lt(deltaE, 10.0);   //tolerance, try 10 for instance
    FragColor = sampleRGB;
}
