precision highp float;

#define MAKE_VEC2_SP_CONST(name, start_id, xval, yval)              \
    layout(constant_id = start_id)     const float name##X = xval;  \
    layout(constant_id = (start_id+1)) const float name##Y = yval;  \
    const vec2 name = vec2(name##X, name##Y);

MAKE_VEC2_SP_CONST(EyeSizeRatio, 0,  0.967592597, 0.988636374)
MAKE_VEC2_SP_CONST(EdgeRatio,    2,  7.0, 7.0)
MAKE_VEC2_SP_CONST(C1,           4,  0.0, 0.0)
MAKE_VEC2_SP_CONST(C2,           6,  0.0, 0.0)
MAKE_VEC2_SP_CONST(LoBound,      8,  0.0, 0.0)
MAKE_VEC2_SP_CONST(HiBound,      10, 0.0, 0.0)
MAKE_VEC2_SP_CONST(ALeft,        12, 0.0, 0.0)
MAKE_VEC2_SP_CONST(BLeft,        14, 0.0, 0.0)
MAKE_VEC2_SP_CONST(ARight,       16, 0.0, 0.0)
MAKE_VEC2_SP_CONST(BRight,       18, 0.0, 0.0)
MAKE_VEC2_SP_CONST(CRight,       20, 0.0, 0.0)

vec2 TextureToEyeUV(const vec2 textureUV, const float isRightEye) {
    // flip distortion horizontally for right eye
    // left: x * 2; right: (1 - x) * 2
    return vec2((textureUV.x + isRightEye * (1. - 2. * textureUV.x)) * 2., textureUV.y);
}

vec2 EyeToTextureUV(const vec2 eyeUV, const float isRightEye) {
    // left: x / 2; right 1 - (x / 2)
    return vec2(eyeUV.x * 0.5 + isRightEye * (1. - eyeUV.x), eyeUV.y);
}

vec2 DecodeFoveationUV(const vec2 uv, const float isRightEye) {
    const vec2 eyeUV = TextureToEyeUV(uv, isRightEye);

    const vec2 center    = (eyeUV - C1) * EdgeRatio / C2;
    const vec2 leftEdge  = (-BLeft + sqrt(BLeft * BLeft + 4. * ALeft * eyeUV)) / (2. * ALeft);
    const vec2 rightEdge = (-BRight + sqrt(BRight * BRight - 4. * (CRight - ARight * eyeUV))) / (2. * ARight);
    
    const vec2 uncompressedUV = mix(
        mix(center, leftEdge, lessThan(eyeUV, LoBound)),
        rightEdge, greaterThan(eyeUV, HiBound)
    );
    return EyeToTextureUV(uncompressedUV * EyeSizeRatio, isRightEye);
}
