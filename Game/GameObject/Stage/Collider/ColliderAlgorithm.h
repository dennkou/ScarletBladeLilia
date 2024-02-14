#pragma once
#include "DirectXMath.h"
//	�����蔻��Ŏg�p����A���S���Y�����܂Ƃ߂����̂��恙
class ColliderAlgorithm
{
public:
	//	3D��ԏ�̂����_���恙
	struct Point
	{
		Point() = default;
		Point(DirectX::XMFLOAT3 inPoint) : point(inPoint) {}
		DirectX::XMFLOAT3 point;
	};
	//	3D��ԏ�̎O�p�`���恙
	struct Triangle
	{
		Triangle() = default;
		constexpr Triangle(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c)
		{
			point[0].point = a;
			point[1].point = b;
			point[2].point = c;
		}
		Point point[3];
	};
	//	3D��ԏ�̐������恙
	struct LineSegment
	{
		LineSegment() = default;
		LineSegment(Point inStart, Point inEnd) : start(inStart),end(inEnd) {}
		Point start;
		Point end;
	};
	//	3D��ԏ�̃J�v�Z�����恙
	struct Capsule
	{
		LineSegment line;
		float radius;
	};


	//	�O�p�`�ƃJ�v�Z���̓����蔻�肾�恙
	static bool CheckHitCapsuleTriangle(const Capsule& capsule, const Triangle& triangle);

	// �����ƎO�p�`�̓����蔻�肾�恙
	static bool HitCheckLineTriangle(const LineSegment& lineSegment, const Triangle& triangle);

	//	 �����ƎO�p�`�̍ŋߓ_�Ԃ̋����̓���Ԃ��恙
	static float SegmentTriangleMinLengthSquare(const LineSegment& lineSegment, const Triangle& triangle);

	// �_�Ɉ�ԋ߂�����̍��W�𓾂�
	static DirectX::XMFLOAT3 GetLinePointMinPosition(DirectX::XMFLOAT3 Point, const LineSegment& lineSegment);

	// �����Ɠ_�̈�ԋ߂������̓��𓾂�
	static float SegmentPointMinLengthSquare(const LineSegment& lineSegment, DirectX::XMFLOAT3 PointPos);

	// ��̐����������Ƃ��߂Â��_�̋����̓��𓾂�
	static float SegmentSegmentMinLengthSquare(const LineSegment& lineSegmentA, const LineSegment& lineSegmentB);

	//	�ȉ��Z�p�n��

	//	DirectX::XMFLOAT3�̑����Z���恙
	static DirectX::XMFLOAT3 VectorAdd(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept;

	//	DirectX::XMFLOAT3�̈����Z���恙
	static DirectX::XMFLOAT3 VectorSub(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept;

	//	DirextX::XMFLOAT3�̊O�ς��v�Z����恙
	static DirectX::XMFLOAT3 VectorOuterProduct(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept;

	//	DirextX::XMFLOAT3�̓��ς��v�Z����恙
	static float VectorInnerProduct(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept;

	//	DirextX::XMFLOAT3�̐��K��������恙
	static DirectX::XMFLOAT3 VectorNormalize(const DirectX::XMFLOAT3& in) noexcept;

	//	�x�N�g���̃T�C�Y�̓����擾����恙
	static float VSquareSize(const DirectX::XMFLOAT3& in)noexcept;

	//	DirextX::XMFLOAT3�̃X�P�[�����O���s���恙
	static DirectX::XMFLOAT3 VectorScale(DirectX::XMFLOAT3 in, float scale) noexcept;

	// �w��̍��W����O�p�`�̏d�S�����߂�
	static void TriangleBarycenter_Base(const Triangle& triangle, const Point& position, float* TrianglePos1Weight, float* TrianglePos2Weight, float* TrianglePos3Weight) noexcept;
private:
	//	�C���X�^���X�̍쐬�͑z�肵�Ȃ��恙
	ColliderAlgorithm() = delete;
	~ColliderAlgorithm() = delete;
};