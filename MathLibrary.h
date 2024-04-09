#pragma once
#include <DirectXMath.h>

//	DirectX::XMFLOAT3�̑����Z���恙
static DirectX::XMFLOAT3 VectorAdd(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept
{
	DirectX::XMFLOAT3 ret;
	ret.x = in1.x + in2.x;
	ret.y = in1.y + in2.y;
	ret.z = in1.z + in2.z;
	return ret;
}

//	DirectX::XMFLOAT3�̈����Z���恙
static DirectX::XMFLOAT3 VectorSub(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept
{
	DirectX::XMFLOAT3 ret;
	ret.x = in1.x - in2.x;
	ret.y = in1.y - in2.y;
	ret.z = in1.z - in2.z;
	return ret;
}

//	DirextX::XMFLOAT3�̊O�ς��v�Z����恙
static DirectX::XMFLOAT3 VectorOuterProduct(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept
{
	DirectX::XMFLOAT3 ret;

	ret.x = in1.y * in2.z - in1.z * in2.y;
	ret.y = in1.z * in2.x - in1.x * in2.z;
	ret.z = in1.x * in2.y - in1.y * in2.x;
	return ret;
}

//	DirextX::XMFLOAT3�̓��ς��v�Z����恙
static float VectorInnerProduct(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept
{
	return in1.x * in2.x + in1.y * in2.y + in1.z * in2.z;
}

//	DirextX::XMFLOAT3�̐��K��������恙
static DirectX::XMFLOAT3 VectorNormalize(const DirectX::XMFLOAT3& in) noexcept
{
	float tmp = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
	DirectX::XMFLOAT3 ret;
	ret.x = in.x / tmp;
	ret.y = in.y / tmp;
	ret.z = in.z / tmp;
	return ret;
}

//	�x�N�g���̃T�C�Y�̓����擾����恙
static float VectorSquareSize(const DirectX::XMFLOAT3& in)noexcept
{
	return in.x * in.x + in.y * in.y + in.z * in.z;
}

//	DirextX::XMFLOAT3�̃X�P�[�����O���s���恙
static DirectX::XMFLOAT3 VectorScale(DirectX::XMFLOAT3 in, float scale) noexcept
{
	DirectX::XMFLOAT3 ret;
	ret.x = in.x * scale;
	ret.y = in.y * scale;
	ret.z = in.z * scale;
	return ret;
}
