#pragma once
#ifndef ALXR_FOVEATION_H
#define ALXR_FOVEATION_H
#include <type_traits>
#include <cmath>
#include "alxr_ctypes.h"

namespace ALXR {
    struct alignas(16) FoveatedDecodeParams {
        XrVector2f eyeSizeRatio;
        XrVector2f edgeRatio;
        XrVector2f c1;
        XrVector2f c2;
        XrVector2f loBound;
        XrVector2f hiBound;
        XrVector2f aLeft;
        XrVector2f bLeft;
        XrVector2f aRight;
        XrVector2f bRight;
        XrVector2f cRight;
    };
    static_assert(std::is_standard_layout<FoveatedDecodeParams>());

    struct alignas(16) FoveatedDecodeBaseParams {
        XrVector2f eyeSizeRatio;
        XrVector2f centerSize;
        XrVector2f centerShift;
        XrVector2f edgeRatio;
    };
    static_assert(std::is_standard_layout<FoveatedDecodeBaseParams>());

    inline FoveatedDecodeBaseParams MakeFoveatedDecodeBaseParams
    (
        const XrVector2f& targetEyeSize,
        const XrVector2f& centerSize,
        const XrVector2f& centerShift,
        const XrVector2f& edgeRatio
    )
    {
        const float edgeSizeX = targetEyeSize.x - centerSize.x * targetEyeSize.x;
        const float edgeSizeY = targetEyeSize.y - centerSize.y * targetEyeSize.y;

        const float centerSizeXAligned = static_cast<const float>(1. - std::ceil(edgeSizeX / (edgeRatio.x * 2.)) * (edgeRatio.x * 2.) / targetEyeSize.x);
        const float centerSizeYAligned = static_cast<const float>(1. - std::ceil(edgeSizeY / (edgeRatio.y * 2.)) * (edgeRatio.y * 2.) / targetEyeSize.y);

        const float edgeSizeXAligned = targetEyeSize.x - centerSizeXAligned * targetEyeSize.x;
        const float edgeSizeYAligned = targetEyeSize.y - centerSizeYAligned * targetEyeSize.y;

        const float centerShiftXAligned = static_cast<const float>(std::ceil(centerShift.x * edgeSizeXAligned / (edgeRatio.x * 2.)) * (edgeRatio.x * 2.) / edgeSizeXAligned);
        const float centerShiftYAligned = static_cast<const float>(std::ceil(centerShift.y * edgeSizeYAligned / (edgeRatio.y * 2.)) * (edgeRatio.y * 2.) / edgeSizeYAligned);

        const float foveationScaleX = (centerSizeXAligned + (1.0f - centerSizeXAligned) / edgeRatio.x);
        const float foveationScaleY = (centerSizeYAligned + (1.0f - centerSizeYAligned) / edgeRatio.y);

        const float optimizedEyeWidth  = foveationScaleX * targetEyeSize.x;
        const float optimizedEyeHeight = foveationScaleY * targetEyeSize.y;

        // round the frame dimensions to a number of pixel multiple of 32 for the encoder
        const auto optimizedEyeWidthAligned  = (uint32_t)std::ceil(optimizedEyeWidth / 32.f) * 32;
        const auto optimizedEyeHeightAligned = (uint32_t)std::ceil(optimizedEyeHeight / 32.f) * 32;

        const float eyeWidthRatioAligned  = optimizedEyeWidth / optimizedEyeWidthAligned;
        const float eyeHeightRatioAligned = optimizedEyeHeight / optimizedEyeHeightAligned;

        return {
            .eyeSizeRatio { eyeWidthRatioAligned, eyeHeightRatioAligned },
            .centerSize   { centerSizeXAligned,   centerSizeYAligned    },
            .centerShift  { centerShiftXAligned,  centerShiftYAligned   },
            .edgeRatio    { edgeRatio.x, edgeRatio.y }
        };
    }

    inline FoveatedDecodeParams MakeFoveatedDecodeParams
    (
        const XrVector2f& targetEyeSize,
        const XrVector2f& centerSize,
        const XrVector2f& centerShift,
        const XrVector2f& edgeRatio
    )
    {
        const auto fp = MakeFoveatedDecodeBaseParams(targetEyeSize, centerSize, centerShift, edgeRatio);

        const float c0_x = (1.f - fp.centerSize.x) * 0.5f;
        const float c0_y = (1.f - fp.centerSize.y) * 0.5f;

        const float c1_x = (edgeRatio.x - 1.f) * c0_x * (fp.centerShift.x + 1.f) / edgeRatio.x;
        const float c1_y = (edgeRatio.y - 1.f) * c0_y * (fp.centerShift.y + 1.f) / edgeRatio.y;

        const float c2_x = (edgeRatio.x - 1.f) * fp.centerSize.x + 1.f;
        const float c2_y = (edgeRatio.y - 1.f) * fp.centerSize.y + 1.f;

        // Lower bound bellow which "left" edge begins
        const float loBoundX = c0_x * (fp.centerShift.x + 1.f);
        const float loBoundY = c0_y * (fp.centerShift.y + 1.f);

        // Upper bound above which "right" edge begins
        const float hiBoundX = c0_x * (fp.centerShift.x - 1.f) + 1.f;
        const float hiBoundY = c0_y * (fp.centerShift.y - 1.f) + 1.f;

        // Same as loBoundX but rescaled for distorted image
        const float loBoundXC = c0_x * (fp.centerShift.x + 1.f) / c2_x;
        const float loBoundYC = c0_y * (fp.centerShift.y + 1.f) / c2_y;

        // Same as hiBoundX but rescaled for distorted image
        const float hiBoundXC = c0_x * (fp.centerShift.x - 1.f) / c2_x + 1.f;
        const float hiBoundYC = c0_y * (fp.centerShift.y - 1.f) / c2_y + 1.f;

        // Constants for function:
        //   leftEdge(x) = (-bleftX + sqrt(bleftX^2 + 4 * aleftX * x)) / (2 * aleftX)
        const float aleftX = c2_x * (1.f - edgeRatio.x) / (edgeRatio.x * loBoundXC);
        const float aleftY = c2_y * (1.f - edgeRatio.y) / (edgeRatio.y * loBoundYC);

        const float bleftX = (c1_x + c2_x * loBoundXC) / loBoundXC;
        const float bleftY = (c1_y + c2_y * loBoundYC) / loBoundYC;

        // Constants for function:
        //   rightEdge(x) = (-brightX + sqrt(brightX^2 + 4 * (crightX - arightX * x)) / (2 * arightX)
        const float arightX = c2_x * (edgeRatio.x - 1.f) / (edgeRatio.x * (1.f - hiBoundXC));
        const float arightY = c2_y * (edgeRatio.y - 1.f) / (edgeRatio.y * (1.f - hiBoundYC));

        const float brightX = (c2_x - edgeRatio.x * c1_x - 2.f * edgeRatio.x * c2_x + c2_x * edgeRatio.x * (1.f - hiBoundXC) + edgeRatio.x) / (edgeRatio.x * (1.f - hiBoundXC));
        const float brightY = (c2_y - edgeRatio.y * c1_y - 2.f * edgeRatio.y * c2_y + c2_y * edgeRatio.y * (1.f - hiBoundYC) + edgeRatio.y) / (edgeRatio.y * (1.f - hiBoundYC));

        const float crightX = ((c2_x * edgeRatio.x - c2_x) * (c1_x - hiBoundXC + c2_x * hiBoundXC)) / (edgeRatio.x * (1.f - hiBoundXC) * (1.f - hiBoundXC));
        const float crightY = ((c2_y * edgeRatio.y - c2_y) * (c1_y - hiBoundYC + c2_y * hiBoundYC)) / (edgeRatio.y * (1.f - hiBoundYC) * (1.f - hiBoundYC));

        return {
            .eyeSizeRatio = fp.eyeSizeRatio,
            .edgeRatio    = fp.edgeRatio,
            .c1           { c1_x,     c1_y     },
            .c2           { c2_x,     c2_y     },
            .loBound      { loBoundX, loBoundY },
            .hiBound      { hiBoundX, hiBoundY },
            .aLeft        { aleftX,   aleftY   },
            .bLeft        { bleftX,   bleftY   },
            .aRight       { arightX,  arightY  },
            .bRight       { brightX,  brightY  },
            .cRight       { crightX,  crightY  },
        };
    }

    inline FoveatedDecodeParams MakeFoveatedDecodeParams(const ALXRRenderConfig& rc) {
        return MakeFoveatedDecodeParams
        (
            XrVector2f{ float(rc.eyeWidth), float(rc.eyeHeight) },
            XrVector2f{ rc.foveationCenterSizeX,  rc.foveationCenterSizeY  },
            XrVector2f{ rc.foveationCenterShiftX, rc.foveationCenterShiftY },
            XrVector2f{ rc.foveationEdgeRatioX,   rc.foveationEdgeRatioY   }
        );
    }
}
#endif
