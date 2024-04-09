#pragma once

#include <initializer_list>
#include <vector>
#include "./../../../Enemy.h"

//	指定座標を順に廻っていくパトロールアルゴリズムだよ☆
class PointPatrol : public NavigationAI
{
public:
	PointPatrol(std::initializer_list<DirectX::XMFLOAT3> pointList);
	~PointPatrol();

	virtual void SetPosition(DirectX::XMFLOAT3 position) override;
	virtual DirectX::XMFLOAT3 MovingPosition() override;
private:
	static constexpr float RANGE = 1.0f;		//	指定位置に到達したとみなす半径だよ☆
	std::vector<DirectX::XMFLOAT3> m_pointList;	//	廻る座標のリストだよ☆
	int m_index;								//	次の位置のインデックスだよ☆
};