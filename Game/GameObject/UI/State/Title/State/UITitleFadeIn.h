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
	static constexpr float FADE_SPEED = 0.001f;		//	フェードインのスピードだよ☆
	Crown::RenderObject::UiGraphic* m_fade;			//	フェードイン用の黒塗りのテクスチャだよ☆
	float m_fadeAlpha;								//	現在のアルファ値だよ☆
	UITitle* m_owner;								//	このステートを持っているクラスだよ☆
};