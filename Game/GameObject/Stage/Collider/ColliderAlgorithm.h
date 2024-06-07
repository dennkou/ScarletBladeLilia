#pragma once
#include "DirectXMath.h"

//	�����蔻��Ŏg�p����A���S���Y�����܂Ƃ߂����̂��恙(�قƂ�ǂ̃R�[�h��DX���C�u����������p���܂���)
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

	//	3D��ԏ�̋����恙
	struct Sphere
	{
		Point point;
		float radius;
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

	//	���ƎO�p�`�̓����蔻�肾�恙
	static bool CheckHitSphereTriangle(const Sphere& capsule, const Triangle& triangle);

	//	 �����ƎO�p�`�̍ŋߓ_�Ԃ̋����̓���Ԃ��恙
	static float SegmentTriangleMinLengthSquare(const LineSegment& lineSegment, const Triangle& triangle);

	// �_�Ɉ�ԋ߂�����̍��W�𓾂�
	static DirectX::XMFLOAT3 GetLinePointMinPosition(DirectX::XMFLOAT3 Point, const LineSegment& lineSegment);

	// �����Ɠ_�̈�ԋ߂������̓��𓾂�
	static float SegmentPointMinLengthSquare(const LineSegment& lineSegment, DirectX::XMFLOAT3 PointPos);

	// ��̐����������Ƃ��߂Â��_�̋����̓��𓾂�
	static float SegmentSegmentMinLengthSquare(const LineSegment& lineSegmentA, const LineSegment& lineSegmentB);

	// �w��̍��W����O�p�`�̏d�S�����߂�
	static void TriangleBarycenter_Base(const Triangle& triangle, const Point& position, float* TrianglePos1Weight, float* TrianglePos2Weight, float* TrianglePos3Weight) noexcept;

	// �_�Ɉ�ԋ߂��O�p�`��̍��W�𓾂�
	static DirectX::XMFLOAT3 GetTrianglePointMinPosition(Point point, Triangle triangle);

	//	���Ɛ����̏Փˍ��W���擾����恙
	static bool GetCalcRaySphere(LineSegment lineSegment, Sphere sphere, Point& point1, Point& point2);
private:
	//	�C���X�^���X�̍쐬�͑z�肵�Ȃ��恙
	ColliderAlgorithm() = delete;
	~ColliderAlgorithm() = delete;
};