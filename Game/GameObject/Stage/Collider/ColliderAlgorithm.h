#pragma once
#include "DirectXMath.h"
//	当たり判定で使用するアルゴリズムをまとめたものだよ☆
class ColliderAlgorithm
{
public:
	//	3D空間上のある一点だよ☆
	struct Point
	{
		Point() = default;
		Point(DirectX::XMFLOAT3 inPoint) : point(inPoint) {}
		DirectX::XMFLOAT3 point;
	};
	//	3D空間上の三角形だよ☆
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
	//	3D空間上の線分だよ☆
	struct LineSegment
	{
		LineSegment() = default;
		LineSegment(Point inStart, Point inEnd) : start(inStart),end(inEnd) {}
		Point start;
		Point end;
	};
	//	3D空間上のカプセルだよ☆
	struct Capsule
	{
		LineSegment line;
		float radius;
	};


	//	三角形とカプセルの当たり判定だよ☆
	static bool CheckHitCapsuleTriangle(const Capsule& capsule, const Triangle& triangle);

	// 線分と三角形の当たり判定だよ☆
	static bool HitCheckLineTriangle(const LineSegment& lineSegment, const Triangle& triangle);

	//	 線分と三角形の最近点間の距離の二乗を返すよ☆
	static float SegmentTriangleMinLengthSquare(const LineSegment& lineSegment, const Triangle& triangle);

	// 点に一番近い線上の座標を得る
	static DirectX::XMFLOAT3 GetLinePointMinPosition(DirectX::XMFLOAT3 Point, const LineSegment& lineSegment);

	// 線分と点の一番近い距離の二乗を得る
	static float SegmentPointMinLengthSquare(const LineSegment& lineSegment, DirectX::XMFLOAT3 PointPos);

	// 二つの線分がもっとも近づく点の距離の二乗を得る
	static float SegmentSegmentMinLengthSquare(const LineSegment& lineSegmentA, const LineSegment& lineSegmentB);

	//	以下算術系☆

	//	DirectX::XMFLOAT3の足し算だよ☆
	static DirectX::XMFLOAT3 VectorAdd(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept;

	//	DirectX::XMFLOAT3の引き算だよ☆
	static DirectX::XMFLOAT3 VectorSub(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept;

	//	DirextX::XMFLOAT3の外積を計算するよ☆
	static DirectX::XMFLOAT3 VectorOuterProduct(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept;

	//	DirextX::XMFLOAT3の内積を計算するよ☆
	static float VectorInnerProduct(const DirectX::XMFLOAT3& in1, const DirectX::XMFLOAT3& in2) noexcept;

	//	DirextX::XMFLOAT3の正規化をするよ☆
	static DirectX::XMFLOAT3 VectorNormalize(const DirectX::XMFLOAT3& in) noexcept;

	//	ベクトルのサイズの二乗を取得するよ☆
	static float VSquareSize(const DirectX::XMFLOAT3& in)noexcept;

	//	DirextX::XMFLOAT3のスケーリングを行うよ☆
	static DirectX::XMFLOAT3 VectorScale(DirectX::XMFLOAT3 in, float scale) noexcept;

	// 指定の座標から三角形の重心を求める
	static void TriangleBarycenter_Base(const Triangle& triangle, const Point& position, float* TrianglePos1Weight, float* TrianglePos2Weight, float* TrianglePos3Weight) noexcept;
private:
	//	インスタンスの作成は想定しないよ☆
	ColliderAlgorithm() = delete;
	~ColliderAlgorithm() = delete;
};