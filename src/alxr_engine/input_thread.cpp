#include "pch.h"
#include "common.h"
#include "logger.h"
#include "input_thread.h"
#include "timing.h"
#include "graphicsplugin.h"
#include "openxr_program.h"

namespace ALXR {
namespace {
inline void LogViewConfig(const ALXREyeInfo& newEyeInfo) {
    constexpr const auto FmtEyeFov = [](const EyeFov& eye) {
        constexpr const float deg = 180.0f / 3.14159265358979323846f;
        return Fmt("{ .left=%f, .right=%f, .top=%f, .bottom=%f }",
            eye.left * deg, eye.right * deg, eye.top * deg, eye.bottom * deg);
    };
    const auto lEyeFovStr = FmtEyeFov(newEyeInfo.eyeFov[0]);
    const auto rEyeFovStr = FmtEyeFov(newEyeInfo.eyeFov[1]);
    Log::Write(Log::Level::Info, Fmt("New view config sent:\n"
        "\tViewConfig {\n"
        "\t  .ipd = %f,\n"
        "\t  .eyeFov {\n"
        "\t    .leftEye  = %s,\n"
        "\t    .rightEye = %s\n"
        "\t  }\n"
        "\t}",
        newEyeInfo.ipd * 1000.0f, lEyeFovStr.c_str(), rEyeFovStr.c_str()));
}
}

void XrInputThread::Update(const XrInputThread::StartCtx& ctx) {

    const auto isConnected = m_isConnected.load();

    ALXREyeInfo newEyeInfo{};
    if (isConnected && ctx.programPtr->GetEyeInfo(newEyeInfo)) {
        if (std::abs(newEyeInfo.ipd - m_lastEyeInfo.ipd) > 0.01f ||
            std::abs(newEyeInfo.eyeFov[0].left - m_lastEyeInfo.eyeFov[0].left) > 0.01f ||
            std::abs(newEyeInfo.eyeFov[1].left - m_lastEyeInfo.eyeFov[1].left) > 0.01f)
        {
            m_lastEyeInfo = newEyeInfo;
            ctx.clientCtx->viewsConfigSend(&newEyeInfo);
            LogViewConfig(newEyeInfo);
        }
    }

    ctx.programPtr->PollActions();
    if (!isConnected)
        return;

    TrackingInfo newInfo;
    if (!ctx.programPtr->GetTrackingInfo(newInfo, m_clientPrediction))
        return;
    ctx.clientCtx->inputSend(&newInfo);
}

void XrInputThread::Run(const XrInputThread::StartCtx& ctx) {    
    CHECK(ctx.clientCtx);
    CHECK(ctx.programPtr);

    using namespace std::chrono;
    using namespace std::chrono_literals;
    static_assert(XrSteadyClock::is_steady);

    ctx.programPtr->SetAndroidAppThread(AndroidThreadType::AppWorker);

    auto nextWakeTime = XrSteadyClock::now();
	while (m_isRunning.load()) {        

        Update(ctx);

        const auto targetFrameDuration = GetTargetFrameDuration();

        nextWakeTime += targetFrameDuration;
        // Adjust nextWakeTime if it's behind the current time
        const auto currentTime = XrSteadyClock::now();
        if (nextWakeTime <= currentTime) {
            // Adjust nextWakeTime to the next future interval
            const auto missedIntervals = ((currentTime - nextWakeTime) / targetFrameDuration) + 1;
            nextWakeTime = currentTime + missedIntervals * targetFrameDuration;
        }
        std::this_thread::sleep_until(nextWakeTime);
	}
}

}
