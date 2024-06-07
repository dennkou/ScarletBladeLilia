#pragma once
#include "DirectXMath.h"

//	当たり判定で使用するアルゴリズムをまとめたものだよ☆(ほとんどのコードはDXライブラリから引用しました)
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

	//	3D空間上の球だよ☆
	struct Sphere
	{
		Point point;
		float radius;
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

	//	球と三角形の当たり判定だよ☆
	static bool CheckHitSphereTriangle(const Sphere& capsule, const Triangle& triangle);

	//	 線分と三角形の最近点間の距離の二乗を返すよ☆
	static float SegmentTriangleMinLengthSquare(const LineSegment& lineSegment, const Triangle& triangle);

	// 点に一番近い線上の座標を得る
	static DirectX::XMFLOAT3 GetLinePointMinPosition(DirectX::XMFLOAT3 Point, const LineSegment& lineSegment);

	// 線分と点の一番近い距離の二乗を得る
	static float SegmentPointMinLengthSquare(const LineSegment& lineSegment, DirectX::XMFLOAT3 PointPos);

	// 二つの線分がもっとも近づく点の距離の二乗を得る
	static float SegmentSegmentMinLengthSquare(const LineSegment& lineSegmentA, const LineSegment& lineSegmentB);

	// 指定の座標から三角形の重心を求める
	static void TriangleBarycenter_Base(const Triangle& triangle, const Point& position, float* TrianglePos1Weight, float* TrianglePos2Weight, float* TrianglePos3Weight) noexcept;

	// 点に一番近い三角形上の座標を得る
	static DirectX::XMFLOAT3 GetTrianglePointMinPosition(Point point, Triangle triangle);

	//	球と線分の衝突座標を取得するよ☆
	static bool GetCalcRaySphere(LineSegment lineSegment, Sphere sphere, Point& point1, Point& point2);
private:
	//	インスタンスの作成は想定しないよ☆
	ColliderAlgorithm() = delete;
	~ColliderAlgorithm() = delete;
};