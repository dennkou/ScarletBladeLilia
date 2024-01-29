#pragma once
#include "./UITitleState.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"

class GameUI::UITitle::UITitleFadeIn : public GameUI::UITitle::UITitleState
{
public:
	UITitleFadeIn(UITitle* uiOutputTitle);
	virtual ~UITitleFadeIn() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;

	virtual void InputSelect() override;

private:
	static constexpr float FADE_SPEED = 0.001f;		//	�t�F�[�h�C���̃X�s�[�h���恙
	Crown::RenderObject::UiGraphic* m_fade;			//	�t�F�[�h�C���p�̍��h��̃e�N�X�`�����恙
	float m_fadeAlpha;								//	���݂̃A���t�@�l���恙
	UITitle* m_owner;								//	���̃X�e�[�g�������Ă���N���X���恙
};