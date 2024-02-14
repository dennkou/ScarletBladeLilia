#pragma once
#include "./../../Crown/Object/RenderSystem/UI/Ui.h"

class EnemyUi : public Crown::RenderObject::Ui
{
public:
	EnemyUi(int maxHp);
	virtual ~EnemyUi();

	virtual void Render(ID3D12GraphicsCommandList* commandList) override;

	void SetMaxHp(int maxHp);
	void SetHp(int hp);
private:
	int m_maxHp;
};