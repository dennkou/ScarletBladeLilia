#pragma once
#include <numbers>
#include <math.h>

namespace Crown
{
	//	‰ñ“]‚ğ+-180ˆÈ“à‚É•ÏŠ·‚·‚é‚æ™(ƒ‰ƒWƒAƒ“)
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