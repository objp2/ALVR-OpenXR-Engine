#pragma once
#ifndef ALXR_ONE_EURO_FILTER_H
#define ALXR_ONE_EURO_FILTER_H
//
// @file
// @brief Header file that completely implements a direction and
// 	orientation filter on tracking reports; it does not implement
// 	an actual server, it is intended to be built into a server.
// @date 2012
// @author
// Jan Ciger
// <jan.ciger@reviatech.com>
// 
//           Copyright Reviatech 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)
//
// "One Euro" filter for reducing jitter
// http://hal.inria.fr/hal-00670496/
// 
// ADAPTED from: https ://github.com/vrpn/vrpn/blob/master/vrpn_OneEuroFilter.h
//
#include <type_traits>
#include <cstddef>
#include <cmath> // for sqrt() and acos()
#include <cstring> // for memcpy

#include "xr_eigen.h"

namespace ALXR {

struct alignas (EIGEN_MAX_ALIGN_BYTES) Vector3LowPassFilter final {
	static constexpr const std::size_t dimension = 3;
	using value_type = Eigen::Vector3f;
	using scalar_type = float;
	using reference_type = value_type&;
	using return_type = value_type;

	return_type filter(const value_type& x, const scalar_type alpha) {
		if (_firstTime) {
			_firstTime = false;
			_hatxprev = x;
		}
		const value_type hatx = alpha * x + (1.0f - alpha) * _hatxprev;
		return _hatxprev = hatx;
	}

	void reset() { _firstTime = true; }

	const return_type& hatxprev() const {
		return _hatxprev;
	}

private:
	value_type _hatxprev = Eigen::Vector3f::Zero();
	bool _firstTime = true;
};


struct alignas (EIGEN_MAX_ALIGN_BYTES) QuaternionLowPassFilter final {
	using float64 = float;
	using QuatType = Eigen::Quaternionf;
	using return_type = QuatType;

	return_type filter(const QuatType& x, const float64 alpha) {
		if (_firstTime) {
			_firstTime = false;
			_hatxprev = x;
		}
		return _hatxprev = _hatxprev.slerp(alpha, x);
	}

	const return_type& hatxprev() const {
		return _hatxprev;
	}

	void reset() { _firstTime = true; }

private:
	QuatType _hatxprev = QuatType::Identity();
	bool _firstTime = true;
};

struct Vector3Filterable {

	using value_filter_type = Vector3LowPassFilter;
	using derivative_filter_type = Vector3LowPassFilter;
	using value_type = Vector3LowPassFilter::value_type;
	using scalar_type = float;
	using reference_type = value_type&;
	using derivative_value_type = value_type;
	using value_ptr_type = scalar_type*;
	using value_filter_return_type = /*typename*/ value_filter_type::return_type;
	static constexpr const std::size_t dimension = Vector3LowPassFilter::dimension;

	static inline /*constexpr*/ value_type dxIdentity() {
		return value_type::Zero();
	}

	static inline /*constexpr*/ derivative_value_type computeDerivative(const value_filter_return_type& prev, const value_type& current, const scalar_type dt) {
		return (current - prev) * (1.0f / dt);
	}

	static inline /*constexpr*/ scalar_type computeDerivativeMagnitude(const derivative_value_type& dx) {
		return dx.stableNorm();
	}
};

struct QuaternionFilterable {

	using QuatType = QuaternionLowPassFilter::QuatType;
	using scalar_type = QuaternionLowPassFilter::float64;
	using value_type = QuatType;
	using derivative_value_type = QuatType;
	using value_ptr_type = QuatType*;
	using value_filter_type = QuaternionLowPassFilter;
	using derivative_filter_type = QuaternionLowPassFilter;
	using value_filter_return_type = value_filter_type::return_type;
	using reference_type = value_type&;

	static inline /*constexpr*/ value_type dxIdentity() {
		return value_type::Identity();
	}

	static inline /*constexpr*/ derivative_value_type computeDerivative(const value_filter_return_type& prev, const value_type& current, const scalar_type dt) {

		const scalar_type rate = 1.0f / dt;
		const QuatType inverse_prev = prev.inverse();
		derivative_value_type dx = current * inverse_prev;
		// nlerp instead of slerp
		dx.x() *= rate;
		dx.y() *= rate;
		dx.z() *= rate;
		dx.w() = dx.w() * rate + (1.0f - rate);
		return dx.normalized();
	}

	static inline /*constexpr*/ scalar_type computeDerivativeMagnitude(const derivative_value_type& dx) {
		/// Should be safe since the quaternion we're given has been normalized.
		return static_cast<scalar_type>(2.0 * std::acos(dx.w()));
	}
};

template<typename Filterable>
struct alignas (EIGEN_MAX_ALIGN_BYTES) OneEuroFilter final {

	typedef Filterable contents;
	using reference_type = typename Filterable::reference_type;
	typedef typename Filterable::scalar_type scalar_type;
	typedef typename Filterable::value_type value_type;
	typedef typename Filterable::derivative_value_type derivative_value_type;
	typedef typename Filterable::value_ptr_type value_ptr_type;
	typedef typename Filterable::derivative_filter_type derivative_filter_type;
	typedef typename Filterable::value_filter_type value_filter_type;
	typedef typename value_filter_type::return_type value_filter_return_type;

	struct Params final {
		scalar_type mincutoff = 1.0;
		scalar_type beta = 0.5;
		scalar_type dcutoff = 1.0; //DerivativeCutoff
	};

	inline constexpr OneEuroFilter(const Params& paramsRef = Params{})
	: _firstTime(true), _params(paramsRef) {}

	inline constexpr const Params& getParams() const { return _params; }

	void reset() {
		_dxfilt.reset();
		_xfilt.reset();
		_firstTime = true;
	}

	value_filter_return_type filter(const scalar_type dt, const value_type& x) {
		derivative_value_type dx;
		if (_firstTime) {
			_firstTime = false;
			dx = Filterable::dxIdentity();
		} else {
			dx = Filterable::computeDerivative(_xfilt.hatxprev(), x, dt);
		}

		const scalar_type derivative_magnitude = Filterable::computeDerivativeMagnitude(_dxfilt.filter(dx, alpha(dt, _params.dcutoff)));
		const scalar_type cutoff = _params.mincutoff + _params.beta * derivative_magnitude;

		return _xfilt.filter(x, alpha(dt, cutoff));
	}

private:
	static inline constexpr scalar_type alpha(const scalar_type dt, const scalar_type cutoff) {
		const scalar_type tau = static_cast<scalar_type>(1.0 / (2.0 * M_PI * cutoff));
		return scalar_type(1) / (scalar_type(1) + tau / dt);
	}

	bool _firstTime;
	value_filter_type _xfilt;
	derivative_filter_type _dxfilt;
	Params _params;
};

using Vector3OneEuroFilter = OneEuroFilter<Vector3Filterable>;
using QuatOneEuroFilter = OneEuroFilter<QuaternionFilterable>;

class alignas (EIGEN_MAX_ALIGN_BYTES) XrPosefOneEuroFilter final {
	QuatOneEuroFilter    _rotFilter;
	Vector3OneEuroFilter _posFilter;

public:
	struct Params final {
		QuatOneEuroFilter::Params    rotParams;
		Vector3OneEuroFilter::Params posParams;
	};

	inline constexpr XrPosefOneEuroFilter(const Params& params = Params{})
	: _rotFilter{ params.rotParams }, _posFilter{ params.posParams } {}

	void reset() {
		_rotFilter.reset();
		_posFilter.reset();
	}

	XrPosef filter(const float dt, const XrPosef& x) {
		const auto newRot = _rotFilter.filter(dt, ToQuaternionf(x.orientation));
		const auto newPos = _posFilter.filter(dt, ToVector3f(x.position));
		return {
			.orientation { newRot.x(), newRot.y(), newRot.z(), newRot.w() },
			.position    { newPos.x(), newPos.y(), newPos.z() },
		};
	}
};
}
#endif
