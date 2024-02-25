#pragma once
#ifndef ALXR_INPUT_H
#define ALXR_INPUT_H

#include <cstdint>
#include <atomic>
#include <chrono>
#include <thread>

#include "alxr_ctypes.h"

struct IOpenXrProgram;

namespace ALXR {

constexpr inline const ALXREyeInfo EyeInfoZero{
	.eyeFov = { {0,0,0,0}, {0,0,0,0} },
	.ipd = 0.0f
};

struct XrInputThread final {

	struct StartCtx final {
		using IOpenXrProgramPtr = std::shared_ptr<IOpenXrProgram>;
		using ALXRClientCtxPtr = std::shared_ptr<const ALXRClientCtx>;
		IOpenXrProgramPtr programPtr;
		ALXRClientCtxPtr  clientCtx;
	};

private:

	std::thread	m_inputThread;
	ALXREyeInfo m_lastEyeInfo = EyeInfoZero;
	std::atomic<std::int64_t> m_targetDurationUS{ 0 };
	std::atomic_bool m_isConnected{ false };
	std::atomic_bool m_clientPrediction{ false };
	std::atomic_bool m_isRunning{ false };

	void Update(const StartCtx& ctx);
	void Run(const StartCtx& ctx);

	auto GetTargetFrameDuration() const noexcept {
		return std::chrono::microseconds{ m_targetDurationUS.load() };
	}

public:
	XrInputThread() noexcept {
		SetTargetFrameRate(90.0f);
	}
	~XrInputThread() {
		Stop();
	}

	XrInputThread(XrInputThread&&) noexcept = delete;
	XrInputThread(const XrInputThread&) noexcept = delete;
	XrInputThread& operator=(XrInputThread&&) noexcept = delete;
	XrInputThread& operator=(const XrInputThread&) noexcept = delete;

	XrInputThread& SetConnected(const bool connected) noexcept {
		m_lastEyeInfo = EyeInfoZero;
		m_isConnected = connected;
		return *this;
	}

	XrInputThread& SetClientPrediction(const bool clientPrediction) noexcept {
		m_clientPrediction.store(clientPrediction);
		return *this;
	}

	XrInputThread& SetTargetFrameRate(const float frameRate) noexcept {
		const auto newTarget = static_cast<std::int64_t>((1.f / (frameRate * 3.f)) * 1e+6f);
		m_targetDurationUS.store(newTarget);
		return *this;
	}

	void Stop() {
		m_isConnected.store(false);
		m_isRunning.store(false);
		if (m_inputThread.joinable()) {
			m_inputThread.join();
		}
	}

	void Start(const StartCtx& ctx) {
		Stop();
		m_isRunning.store(true);
		m_inputThread = std::thread([this, ctxCpy = ctx] {
			Run(ctxCpy);
		});
	}
};
}
#endif
