#include "ColliderAlgorithm.h"
#include <math.h>
#include "./../../MathLibrary.h"

bool ColliderAlgorithm::CheckHitCapsuleTriangle(const Capsule& capsule, const Triangle& triangle)
{
	float minLength;
	minLength = SegmentTriangleMinLengthSquare(capsule.line, triangle);
	return  minLength <= (capsule.radius * capsule.radius);
}

bool ColliderAlgorithm::HitCheckLineTriangle(const LineSegment& lineSegment, const Triangle& triangle)
{
	DirectX::XMFLOAT3 LineL1L2, LineL1T1, LineL1T2, LineL1T3, OP;
	DirectX::XMFLOAT3 LineL1C, LineL2C;
	float u, v, w, totalr;

	LineL1L2 = Crown::Math::VectorSub(lineSegment.end.point, lineSegment.start.point);
	LineL1T1 = Crown::Math::VectorSub(triangle.point[0].point, lineSegment.start.point);
	LineL1T2 = Crown::Math::VectorSub(triangle.point[1].point, lineSegment.start.point);
	LineL1T3 = Crown::Math::VectorSub(triangle.point[2].point, lineSegment.start.point);

	OP = Crown::Math::VectorOuterProduct(LineL1L2, LineL1T2);
	u = Crown::Math::VectorInnerProduct(OP, LineL1T3);

	OP = Crown::Math::VectorOuterProduct(LineL1L2, LineL1T2);
	u = Crown::Math::VectorInnerProduct(OP, LineL1T3);

	OP = Crown::Math::VectorOuterProduct(LineL1L2, LineL1T3);
	v = Crown::Math::VectorInnerProduct(OP, LineL1T1);

	OP = Crown::Math::VectorOuterProduct(LineL1L2, LineL1T1);
	w = Crown::Math::VectorInnerProduct(OP, LineL1T2);

	if (!(((u < 0.0f || v < 0.0f || w < 0.0f) &&
		(u > 0.0f || v > 0.0f || w > 0.0f)) ||
		(u < 0.0000001f && u > -0.0000001f &&
			v < 0.0000001f && v > -0.0000001f &&
			w < 0.0000001f && w > -0.0000001f)))
	{
		totalr = 1.0f / (u + v + w);
		u *= totalr;
		v *= totalr;
		w *= totalr;

		DirectX::XMFLOAT3 pos;
		pos.x = triangle.point[0].point.x * u + triangle.point[1].point.x * v + triangle.point[2].point.x * w;
		pos.y = triangle.point[0].point.y * u + triangle.point[1].point.y * v + triangle.point[2].point.y * w;
		pos.z = triangle.point[0].point.z * u + triangle.point[1].point.z * v + triangle.point[2].point.z * w;

		LineL1C = Crown::Math::VectorSub(lineSegment.start.point, pos);
		LineL2C = Crown::Math::VectorSub(lineSegment.end.point, pos);
		if (Crown::Math::VectorInnerProduct(LineL1C, LineL2C) <= 0.0f)
		{
			return true;
		}
	}

	bool Project1, Project2;
	DirectX::XMFLOAT3 Seg1_TriPro;
	DirectX::XMFLOAT3 Seg2_TriPro;
	float Seg1_TriPro_w1;
	float Seg1_TriPro_w2;
	float Seg1_TriPro_w3;
	float Seg2_TriPro_w1;
	float Seg2_TriPro_w2;
	float Seg2_TriPro_w3;
	DirectX::XMFLOAT3 Seg1_2;
	DirectX::XMFLOAT3 Tri1_Seg1;
	DirectX::XMFLOAT3 Tri1_Seg2;
	DirectX::XMFLOAT3 TriNorm;
	DirectX::XMFLOAT3 tv;
	DirectX::XMFLOAT3 Tri1_2, Tri1_3;
	float Seg1_TriProSeg1_Dist = 0.0f;
	float Seg2_TriProSeg2_Dist = 0.0f;
	float Seg1_TriProSeg1_DistBase = 0.0f;
	float Seg2_TriProSeg2_DistBase = 0.0f;

	Seg1_2 = Crown::Math::VectorSub(lineSegment.end.point, lineSegment.start.point);
	Tri1_2 = Crown::Math::VectorSub(triangle.point[1].point, triangle.point[0].point);
	Tri1_3 = Crown::Math::VectorSub(triangle.point[2].point, triangle.point[0].point);
	TriNorm = Crown::Math::VectorOuterProduct(Tri1_2, Tri1_3);
	TriNorm = Crown::Math::VectorNormalize(TriNorm);

	// 三角形が示す平面状に頂点を投影する
	Tri1_Seg1 = Crown::Math::VectorSub(lineSegment.start.point, triangle.point[0].point);
	Seg1_TriProSeg1_DistBase = Crown::Math::VectorInnerProduct(Tri1_Seg1, TriNorm);
	tv = Crown::Math::VectorScale(TriNorm, Seg1_TriProSeg1_DistBase);
	Seg1_TriPro = Crown::Math::VectorSub(lineSegment.start.point, tv);
	if (Seg1_TriProSeg1_DistBase < 0.0f)
	{
		Seg1_TriProSeg1_Dist = -Seg1_TriProSeg1_DistBase;
	}
	else
	{
		Seg1_TriProSeg1_Dist = Seg1_TriProSeg1_DistBase;
	}
	// 三角形内に頂点が存在するかどうかを調べる
	{
		Point tmp;
		tmp.point = Seg1_TriPro;
		TriangleBarycenter_Base(triangle, tmp, &Seg1_TriPro_w1, &Seg1_TriPro_w2, &Seg1_TriPro_w3);
	}
	Project1 = Seg1_TriPro_w1 >= 0.0f && Seg1_TriPro_w1 <= 1.0f && Seg1_TriPro_w2 >= 0.0f && Seg1_TriPro_w2 <= 1.0f && Seg1_TriPro_w3 >= 0.0f && Seg1_TriPro_w3 <= 1.0f;

	// 三角形が示す平面状に頂点を投影する
	Tri1_Seg2 = Crown::Math::VectorSub(lineSegment.end.point, triangle.point[0].point);
	Seg2_TriProSeg2_DistBase = Crown::Math::VectorInnerProduct(Tri1_Seg2, TriNorm);
	tv = Crown::Math::VectorScale(TriNorm, Seg2_TriProSeg2_DistBase);
	Seg2_TriPro = Crown::Math::VectorSub(lineSegment.end.point, tv);
	if(Seg2_TriProSeg2_DistBase < 0.0f)
	{
		Seg2_TriProSeg2_Dist = -Seg2_TriProSeg2_DistBase;
	}
	else
	{
		Seg2_TriProSeg2_Dist = Seg2_TriProSeg2_DistBase;
	}
	// 三角形内に頂点が存在するかどうかを調べる
	{
		Point tmp;
		tmp.point = Seg2_TriPro;
		TriangleBarycenter_Base(triangle, tmp, &Seg2_TriPro_w1, &Seg2_TriPro_w2, &Seg2_TriPro_w3);
	}
	Project2 = Seg2_TriPro_w1 >= 0.0f && Seg2_TriPro_w1 <= 1.0f && Seg2_TriPro_w2 >= 0.0f && Seg2_TriPro_w2 <= 1.0f && Seg2_TriPro_w3 >= 0.0f && Seg2_TriPro_w3 <= 1.0f;


	// 三角形と線分が交差又は接している可能性がある場合は分岐
	if ((Seg1_TriProSeg1_Dist >= 0.00000001f || Seg2_TriProSeg2_Dist >= 0.00000001f) &&
		(Seg1_TriProSeg1_Dist < 0.00000001f || Seg2_TriProSeg2_Dist < 0.00000001f ||
			(Seg1_TriProSeg1_DistBase > 0.0f && Seg2_TriProSeg2_DistBase < 0.0f) ||
			(Seg1_TriProSeg1_DistBase < 0.0f && Seg2_TriProSeg2_DistBase > 0.0f)))
	{
		float Seg_TriPos_TriPos1_w;
		float Seg_TriPos_TriPos2_w;
		float Seg_TriPos_TriPos3_w;
		DirectX::XMFLOAT3 Seg_TriPos;
		float TriPos_SegPos1_2_t;

		// 三角形が表す平面と線分との接点と重心を算出する
		TriPos_SegPos1_2_t = Seg1_TriProSeg1_Dist / (Seg1_TriProSeg1_Dist + Seg2_TriProSeg2_Dist);
		Seg_TriPos = Crown::Math::VectorScale(Seg1_2, TriPos_SegPos1_2_t);
		Seg_TriPos = Crown::Math::VectorAdd(Seg_TriPos, lineSegment.start.point);

		{
			Point tmp;
			tmp.point = Seg_TriPos;
			TriangleBarycenter_Base(triangle, tmp, &Seg_TriPos_TriPos1_w, &Seg_TriPos_TriPos2_w, &Seg_TriPos_TriPos3_w);
		}

		// 線分と三角形が接しているかチェック
		if (Seg_TriPos_TriPos1_w >= 0.0f && Seg_TriPos_TriPos1_w <= 1.0f &&
			Seg_TriPos_TriPos2_w >= 0.0f && Seg_TriPos_TriPos2_w <= 1.0f &&
			Seg_TriPos_TriPos3_w >= 0.0f && Seg_TriPos_TriPos3_w <= 1.0f)
		{
			return true;
		}
	}

	// どちらも三角形の範囲にあった場合
	if (Project1 && Project2)
	{
		// 片方の側にどちらの頂点もある場合は、より近いほうの頂点と平面との距離を結果にする
		if (Seg1_TriProSeg1_Dist < Seg2_TriProSeg2_Dist ||
			Seg1_TriProSeg1_Dist == Seg2_TriProSeg2_Dist)
		{
			return Seg1_TriProSeg1_Dist * Seg1_TriProSeg1_Dist < 0.0000001f;
		}
		else
		{
			return Seg2_TriProSeg2_Dist * Seg2_TriProSeg2_Dist < 0.0000001f;
		}
	}

	// 線分が三角形と同一平面上にある場合のみ三角形に横から当たっているかどうかの判定を行う
	if (Seg1_TriProSeg1_Dist < 0.00000001f && Seg2_TriProSeg2_Dist < 0.00000001f)
	{
		// 線分の始点が三角形の範囲にある場合は接触とする
		if (Project1 || Project2)
		{
			return true;
		}
	}

	return false;
}

bool ColliderAlgorithm::CheckHitSphereTriangle(const Sphere& capsule, const Triangle& triangle)
{
	return (capsule.radius * capsule.radius) >= Crown::Math::VectorSquareSize(Crown::Math::VectorSub(capsule.point.point, GetTrianglePointMinPosition(capsule.point, triangle)));
}

float ColliderAlgorithm::SegmentTriangleMinLengthSquare(const LineSegment& lineSegment, const Triangle& triangle)
{
	bool Touei1, Touei2;
	DirectX::XMFLOAT3 st1, st2;
	DirectX::XMFLOAT3 stw1, stw2;
	DirectX::XMFLOAT3 tv, tnorm;
	DirectX::XMFLOAT3 v12, v13;
	float st1u, st1v, st1w;
	float st2u, st2v, st2w;
	float l1 = 0.0f, l2 = 0.0f;
	float l12, l23, l31;
	float MinLen;

	// 線分が三角形を貫いていたら距離は０
	if (HitCheckLineTriangle(lineSegment, triangle))
	{
		return 0.0f;
	}

	v12 = Crown::Math::VectorSub(triangle.point[1].point, triangle.point[0].point);
	v13 = Crown::Math::VectorSub(triangle.point[2].point, triangle.point[0].point);
	tnorm = Crown::Math::VectorOuterProduct(v12, v13);
	tnorm = Crown::Math::VectorNormalize(tnorm);

	// 三角形が示す平面状に頂点を投影する
	tv = Crown::Math::VectorSub(lineSegment.start.point, triangle.point[0].point);
	stw1 = tv;
	tv = Crown::Math::VectorScale(tnorm, Crown::Math::VectorInnerProduct(tv, tnorm));
	st1 = Crown::Math::VectorSub(lineSegment.start.point, tv);

	// 三角形内に頂点が存在するかどうかを調べる
	{
		Point tmp;
		tmp.point = st1;
		TriangleBarycenter_Base(triangle, tmp, &st1u, &st1v, &st1w);
	}
	Touei1 = !(st1u <= 0.0f || st1u >= 1.0f || st1v <= 0.0f || st1v >= 1.0f || st1w <= 0.0f || st1w >= 1.0f);
	if (Touei1)
	{
		tv = Crown::Math::VectorSub(lineSegment.start.point, st1);
		l1 = Crown::Math::VectorInnerProduct(tv, tv);
	}

	// 三角形が示す平面状に頂点を投影する
	tv = Crown::Math::VectorSub(lineSegment.end.point, lineSegment.start.point);
	stw2 = tv;
	tv = Crown::Math::VectorScale(tnorm, Crown::Math::VectorInnerProduct(tv, tnorm));
	st2 = Crown::Math::VectorSub(lineSegment.end.point, tv);

	// 三角形内に頂点が存在するかどうかを調べる
	{
		Point tmp;
		tmp.point = st2;
		TriangleBarycenter_Base(triangle, tmp, &st2u, &st2v, &st2w);
	}
	Touei2 = !(st2u <= 0.0f || st2u >= 1.0f || st2v <= 0.0f || st2v >= 1.0f || st2w <= 0.0f || st2w >= 1.0f);
	if (Touei2)
	{
		tv = Crown::Math::VectorSub(lineSegment.end.point, st2);
		l2 = Crown::Math::VectorInnerProduct(tv, tv);
	}

	// どちらも三角形の範囲にあった場合
	if (Touei1 && Touei2)
	{
		float i1, i2;

		// 線分の各頂点が三角形の前後にあるか判定
		i1 = Crown::Math::VectorInnerProduct(tnorm, stw1);
		i2 = Crown::Math::VectorInnerProduct(tnorm, stw2);
		if ((i1 < 0.0f && i2 < 0.0f) || (i1 >= 0.0f && i2 >= 0.0f))
		{
			// 片方の側にどちらの頂点もある場合は、より近いほうの頂点と平面との距離を結果にする
			if (l1 < l2)
			{
				return l1;
			}
			else
			{
				return l2;
			}
		}
		else
		{
			return 0.0f;
		}
	}

	// そうではない場合は三角形の３辺と線分の距離を測り、一番距離が短い辺を探す
	l12 = SegmentSegmentMinLengthSquare(lineSegment, LineSegment(triangle.point[0], triangle.point[1]));
	l23 = SegmentSegmentMinLengthSquare(lineSegment, LineSegment(triangle.point[1], triangle.point[2]));
	l31 = SegmentSegmentMinLengthSquare(lineSegment, LineSegment(triangle.point[2], triangle.point[0]));

	MinLen = l12;
	if (l23 < MinLen)
	{
		MinLen = l23;
	}
	if (l31 < MinLen)
	{
		MinLen = l31;
	}
	if (Touei1 && l1 < MinLen)
	{
		MinLen = l1;
	}
	if (Touei2 && l2 < MinLen)
	{
		MinLen = l2;
	}

	return MinLen;
}

DirectX::XMFLOAT3 ColliderAlgorithm::GetLinePointMinPosition(DirectX::XMFLOAT3 Point, const LineSegment& lineSegment)
{
	float Length12, t;
	float Dot12_1P;
	DirectX::XMFLOAT3 Line12, Line1P;
	DirectX::XMFLOAT3 Result;

	Line12.x =lineSegment.end.point.x - lineSegment.start.point.x;
	Line12.y =lineSegment.end.point.y - lineSegment.start.point.y;
	Line12.z =lineSegment.end.point.z - lineSegment.start.point.z;
	Line1P.x = Point.x - lineSegment.start.point.x;
	Line1P.y = Point.y - lineSegment.start.point.y;
	Line1P.z = Point.z - lineSegment.start.point.z;
	Dot12_1P = Line12.x * Line1P.x + Line12.y * Line1P.y + Line12.z * Line1P.z;
	if (Dot12_1P <= 0.0f)
	{
		return lineSegment.start.point;
	}

	Length12 = Line12.x * Line12.x + Line12.y * Line12.y + Line12.z * Line12.z;
	if (Length12 == 0.0f)
	{
		return lineSegment.start.point;
	}

	if (Length12 < Dot12_1P)
	{
		return lineSegment.end.point;
	}

	t = Dot12_1P / Length12;
	Result.x = lineSegment.start.point.x + Line12.x * t;
	Result.y = lineSegment.start.point.y + Line12.y * t;
	Result.z = lineSegment.start.point.z + Line12.z * t;

	return Result;
}

float ColliderAlgorithm::SegmentPointMinLengthSquare(const LineSegment& lineSegment, DirectX::XMFLOAT3 PointPos)
{
	DirectX::XMFLOAT3 MinPosition, Sa;

	// 線と点の最近点を取得する
	MinPosition = GetLinePointMinPosition(PointPos, lineSegment);

	// 最近点と点の距離を返す
	Sa.x = MinPosition.x - PointPos.x;
	Sa.y = MinPosition.y - PointPos.y;
	Sa.z = MinPosition.z - PointPos.z;
	return Sa.x * Sa.x + Sa.y * Sa.y + Sa.z * Sa.z;
}

float ColliderAlgorithm::SegmentSegmentMinLengthSquare(const LineSegment& lineSegmentA, const LineSegment& lineSegmentB)
{
	DirectX::XMFLOAT3 da, db, p, tp;
	DirectX::XMFLOAT3 e1, e2;
	float a, b, c, d, e, f, w, s, t = 0.0f;
	float tmpA, tmpB;

	da = Crown::Math::VectorSub(lineSegmentA.end.point, lineSegmentA.start.point);
	db = Crown::Math::VectorSub(lineSegmentB.end.point, lineSegmentB.start.point);
	p = Crown::Math::VectorSub(lineSegmentA.start.point, lineSegmentB.start.point);
	a = Crown::Math::VectorInnerProduct(da, da);
	b = -Crown::Math::VectorInnerProduct(da, db);
	c = -b;
	d = -Crown::Math::VectorInnerProduct(db, db);
	e = -Crown::Math::VectorInnerProduct(da, p);
	f = -Crown::Math::VectorInnerProduct(db, p);

	// SA1 - SA2 または SB1 - SB2 の距離が限りなくゼロに近いかどうかのチェック
	tmpA = a < 0.0f ? -a : a;
	tmpB = d < 0.0f ? -d : d;
	if (tmpA < 0.00000001f)
	{
		if (tmpB < 0.00000001f)
		{
			return Crown::Math::VectorSquareSize(Crown::Math::VectorSub(lineSegmentA.start.point, lineSegmentB.start.point));
		}
		else
		{
			return SegmentPointMinLengthSquare(lineSegmentB, lineSegmentA.start.point);
		}
	}
	else
	{
		if (tmpB < 0.00000001f)
		{
			return SegmentPointMinLengthSquare(lineSegmentA, lineSegmentB.start.point);
		}
	}

	s = -0.1f;
	w = a * d - b * c;
	if (w <= -0.00000001f)
	{
		// クラーメルの公式
		s = (e * d - b * f) / w;
		t = (a * f - e * c) / w;
	}

	// 二つの線分が縮退していたら点と見なして点同士の距離を返す
	if (a <= 0.0f && -d <= 0.0f)
	{
		tp = Crown::Math::VectorSub(lineSegmentA.start.point, lineSegmentB.start.point);
		return static_cast<float>(sqrt(Crown::Math::VectorInnerProduct(tp, tp)));
	}

	if (a <= 0.0f)
	{
		s = 0.0f;
		if (t > 1.0f || t < 0.0f)
			t = t > 1.0f ? 1.0f : 0.0f;
	}
	else
	{
		if (-d <= 0.0f)
		{
			t = 0.0f;
			if (s > 1.0f || s < 0.0f)
				s = s > 1.0f ? 1.0f : 0.0f;
		}
		else
		{
			if (s > 1.0f || s < 0.0f)
			{
				s = s > 1.0f ? 1.0f : 0.0f;

				t = (-b * s + -f) / -d;		// ガウス消去法
				if (t > 1.0f || t < 0.0f)
				{
					t = t > 1.0f ? 1.0f : 0.0f;
					s = (-b * t - -e) / a;	// ガウス消去法
					if (s > 1.0f)
					{
						s = 1.0f;
					}
					else if (s < 0.0f)
					{
						s = 0.0f;
					}
				}
			}
			else
			{
				if (t > 1.0f || t < 0.0f)
				{
					t = t > 1.0f ? 1.0f : 0.0f;
					s = (-b * t - -e) / a;		// ガウス消去法

					if (s > 1.0f || s < 0.0f)
					{
						s = s > 1.0f ? 1.0f : 0.0f;
						t = (-b * s + -f) / -d;	// ガウス消去法
						if (t > 1.0f)
						{
							t = 1.0f;
						}
						else if (t < 0.0f)
						{
							t = 0.0f;
						}
					}
				}
			}
		}
	}

	e1 = Crown::Math::VectorScale(da, s);
	e1 = Crown::Math::VectorAdd(e1, lineSegmentA.start.point);

	e2 = Crown::Math::VectorScale(db, t);
	e2 = Crown::Math::VectorAdd(e2, lineSegmentB.start.point);

	tp = Crown::Math::VectorSub(e1, e2);
	return Crown::Math::VectorInnerProduct(tp, tp);
}

void ColliderAlgorithm::TriangleBarycenter_Base(const Triangle& triangle, const Point& position, float* TrianglePos1Weight, float* TrianglePos2Weight, float* TrianglePos3Weight) noexcept
{
	DirectX::XMFLOAT3 v1, v2, v3;
	float d11, d12, d22, d13, d23, divn;

	v1 = Crown::Math::VectorSub(triangle.point[1].point, triangle.point[0].point);
	v2 = Crown::Math::VectorSub(triangle.point[2].point, triangle.point[0].point);
	v3 = Crown::Math::VectorSub(position.point, triangle.point[0].point);

	d11 = Crown::Math::VectorInnerProduct(v1, v1);
	d12 = Crown::Math::VectorInnerProduct(v2, v1);
	d22 = Crown::Math::VectorInnerProduct(v2, v2);
	d13 = Crown::Math::VectorInnerProduct(v1, v3);
	d23 = Crown::Math::VectorInnerProduct(v2, v3);
	divn = d11 * d22 - d12 * d12;
	*TrianglePos2Weight = (d13 * d22 - d12 * d23) / divn;
	*TrianglePos3Weight = (d11 * d23 - d13 * d12) / divn;
	*TrianglePos1Weight = 1.0f - *TrianglePos2Weight - *TrianglePos3Weight;
}

DirectX::XMFLOAT3 ColliderAlgorithm::GetTrianglePointMinPosition(Point point, Triangle triangle)
{
	DirectX::XMFLOAT3 Line12, Line23, Line31, Line1P, Line2P, Line3P, Result;
	float Dot1P2, Dot1P3, Dot2P1, Dot2P3, Dot2PH, Dot3P1, Dot3P2, Dot3PH, OPA, OPB, OPC, Div, t, v, w;

	Line12 = Crown::Math::VectorSub(triangle.point[1].point, triangle.point[0].point);
	Line31 = Crown::Math::VectorSub(triangle.point[0].point, triangle.point[2].point);
	Line1P = Crown::Math::VectorSub(point.point, triangle.point[0].point);
	Dot1P2 = Crown::Math::VectorInnerProduct(Line12, Line1P);
	Dot1P3 = Crown::Math::VectorInnerProduct(Line31, Line1P);
	if (Dot1P2 <= 0.0f && Dot1P3 >= 0.0f)
	{
		return triangle.point[0].point;
	}

	Line23 = Crown::Math::VectorSub(triangle.point[2].point, triangle.point[1].point);
	Line2P = Crown::Math::VectorSub(point.point, triangle.point[1].point);
	Dot2P1 = Crown::Math::VectorInnerProduct(Line12, Line2P);
	Dot2P3 = Crown::Math::VectorInnerProduct(Line23, Line2P);
	if (Dot2P1 >= 0.0f && Dot2P3 <= 0.0f)
	{
		return triangle.point[1].point;
	}

	Dot2PH = Crown::Math::VectorInnerProduct(Line31, Line2P);
	OPC = Dot1P2 * -Dot2PH - Dot2P1 * -Dot1P3;	// ←ラグランジュ恒等式
	if (OPC <= 0.0f && Dot1P2 >= 0.0f && Dot2P1 <= 0.0f)
	{
		t = Dot1P2 / (Dot1P2 - Dot2P1);
		Result.x = triangle.point[0].point.x + Line12.x * t;
		Result.y = triangle.point[0].point.y + Line12.y * t;
		Result.z = triangle.point[0].point.z + Line12.z * t;
		return Result;
	}

	Line3P = Crown::Math::VectorSub(point.point, triangle.point[2].point);
	Dot3P1 = Crown::Math::VectorInnerProduct(Line31, Line3P);
	Dot3P2 = Crown::Math::VectorInnerProduct(Line23, Line3P);
	if (Dot3P1 <= 0.0f && Dot3P2 >= 0.0f)
	{
		return triangle.point[2].point;
	}

	Dot3PH = Crown::Math::VectorInnerProduct(Line12, Line3P);
	OPB = Dot3PH * -Dot1P3 - Dot1P2 * -Dot3P1;	// ←ラグランジュ恒等式
	if (OPB <= 0.0f && Dot1P3 <= 0.0f && Dot3P1 >= 0.0f)
	{
		t = Dot3P1 / (Dot3P1 - Dot1P3);
		Result.x = triangle.point[2].point.x + Line31.x * t;
		Result.y = triangle.point[2].point.y + Line31.y * t;
		Result.z = triangle.point[2].point.z + Line31.z * t;
		return Result;
	}

	OPA = Dot2P1 * -Dot3P1 - Dot3PH * -Dot2PH;	// ←ラグランジュ恒等式
	if (OPA <= 0.0f && (-Dot2PH - Dot2P1) >= 0.0f && (Dot3PH + Dot3P1) >= 0.0f)
	{
		t = (-Dot2PH - Dot2P1) / ((-Dot2PH - Dot2P1) + (Dot3PH + Dot3P1));
		Result.x = triangle.point[1].point.x + Line23.x * t;
		Result.y = triangle.point[1].point.y + Line23.y * t;
		Result.z = triangle.point[1].point.z + Line23.z * t;
		return Result;
	}

	Div = 1.0f / (OPA + OPB + OPC);
	v = OPB * Div;
	w = OPC * Div;
	Result.x = triangle.point[0].point.x + Line12.x * v - Line31.x * w;
	Result.y = triangle.point[0].point.y + Line12.y * v - Line31.y * w;
	Result.z = triangle.point[0].point.z + Line12.z * v - Line31.z * w;
	return Result;
}

bool ColliderAlgorithm::GetCalcRaySphere(LineSegment lineSegment, Sphere sphere, Point& point1, Point& point2)
{
	DirectX::XMFLOAT3 lineVector = Crown::Math::VectorSub(lineSegment.start.point, lineSegment.end.point);

	sphere.point.point.x = sphere.point.point.x - lineSegment.start.point.x; 
	sphere.point.point.y = sphere.point.point.y - lineSegment.start.point.y;
	sphere.point.point.z = sphere.point.point.z - lineSegment.start.point.z;

	float A = Crown::Math::VectorInnerProduct(lineVector, lineVector);
	float B = Crown::Math::VectorInnerProduct(lineVector, sphere.point.point);
	float C = Crown::Math::VectorInnerProduct(sphere.point.point, sphere.point.point) - sphere.radius * sphere.radius;

	if (A == 0.0f)
	{
		return false; // レイの長さが0
	}

	float s = B * B - A * C;
	if (s < 0.0f)
	{
		return false; // 衝突していない
	}

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
	{
		return false; // レイの反対で衝突
	}

	point1.point.x = lineSegment.start.point.x + a1 * lineVector.x;
	point1.point.y = lineSegment.start.point.y + a1 * lineVector.y;
	point1.point.z = lineSegment.start.point.z + a1 * lineVector.z;
	point2.point.x = lineSegment.start.point.x + a2 * lineVector.x;
	point2.point.y = lineSegment.start.point.y + a2 * lineVector.y;
	point2.point.z = lineSegment.start.point.z + a2 * lineVector.z;

	return true;
}