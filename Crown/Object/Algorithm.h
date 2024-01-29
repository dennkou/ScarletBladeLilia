#pragma once
#include <numbers>
#include <math.h>

namespace Crown
{
	//	��]��+-180�ȓ��ɕϊ�����恙(���W�A��)
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