#pragma once

#include <initializer_list>
#include <vector>
#include "./../../../Enemy.h"

//	�w����W�����ɉ���Ă����p�g���[���A���S���Y�����恙
class PointPatrol : public NavigationAI
{
public:
	PointPatrol(std::initializer_list<DirectX::XMFLOAT3> pointList);
	~PointPatrol();

	virtual void SetPosition(DirectX::XMFLOAT3 position) override;
	virtual DirectX::XMFLOAT3 MovingPosition() override;
private:
	static constexpr float RANGE = 1.0f;		//	�w��ʒu�ɓ��B�����Ƃ݂Ȃ����a���恙
	std::vector<DirectX::XMFLOAT3> m_pointList;	//	�����W�̃��X�g���恙
	int m_index;								//	���̈ʒu�̃C���f�b�N�X���恙
};