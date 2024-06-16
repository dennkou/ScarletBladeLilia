#include "PointPatrol.h"
#include "./../../MathLibrary.h"

PointPatrol::PointPatrol(std::initializer_list<DirectX::XMFLOAT3> pointList)
	:
	m_pointList(pointList.begin(), pointList.end()),
	m_index(0)
{

}

PointPatrol::~PointPatrol()
{

}

void PointPatrol::SetPosition(DirectX::XMFLOAT3 position)
{
	DirectX::XMFLOAT3 distance = MovingPosition();
	distance = VectorSub(distance, position);

	if (VectorSquareSize(distance) <= RANGE * RANGE)
	{
		++m_index;
		if (m_index >= m_pointList.size())
		{
			m_index = 0;
		}
	}
}

DirectX::XMFLOAT3 PointPatrol::MovingPosition()
{
	return m_pointList[m_index];
}