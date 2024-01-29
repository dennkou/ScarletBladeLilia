#pragma once
#include <numbers>
#include <math.h>

namespace Crown
{
	//	回転を+-180以内に変換するよ☆(ラジアン)
	static float RotateNormalize(float rotate) noexcept
	{
		double ret = fmod(rotate, std::numbers::pi * 2);
		if (ret > std::numbers::pi)
		{
			ret -= std::numbers::pi * 2;
		}
		if (ret < -std::numbers::pi)
		{
			ret += std::numbers::pi * 2;
		}
		return static_cast<float>(ret);
	}
}