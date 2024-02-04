#include "decoderplugin.h"

#if defined(XR_USE_PLATFORM_ANDROID) && !defined(XR_DISABLE_DECODER_THREAD)
std::shared_ptr<IDecoderPlugin> CreateDecoderPlugin_MediaCodec(const IDecoderPlugin::RunCtx&);
#elif !defined(XR_DISABLE_DECODER_THREAD)
std::shared_ptr<IDecoderPlugin> CreateDecoderPlugin_FFMPEG(const IDecoderPlugin::RunCtx&);
#else
std::shared_ptr<IDecoderPlugin> CreateDecoderPlugin_Dummy(const IDecoderPlugin::RunCtx&);
#endif

std::shared_ptr<IDecoderPlugin> CreateDecoderPlugin(const IDecoderPlugin::RunCtx& ctx) {
#if defined(XR_USE_PLATFORM_ANDROID) && !defined(XR_DISABLE_DECODER_THREAD)
	return CreateDecoderPlugin_MediaCodec(ctx);
#elif !defined(XR_DISABLE_DECODER_THREAD)
	return CreateDecoderPlugin_FFMPEG(ctx);
#else
	return CreateDecoderPlugin_Dummy(ctx);
#endif
}
