#pragma once
#include "./../UIState.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"
#include "./../../../../DesignPatterns/FiniteStateMachine.h"


class GameUI::UITitle : public GameUI::UIState
{
public:
	UITitle(GameUI* gameUI);
	virtual ~UITitle() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;

	virtual void InputMouse(DirectX::XMFLOAT2 input) override;
	virtual void InputCursorUp() override;
	virtual void InputCursorDown() override;
	virtual void InputSelect() override;
private:
	static constexpr float SELECT_ITEM_START_POS_Y = -2.0f;			//	��ԏ��UI�̏����ʒu���恙
	static constexpr float SELECT_ITEM_POSITION_Y = -0.3f;			//	��ԏ��UI��Y�ʒu���恙
	static constexpr float SELECT_ITEM_SPEED = 0.0015f;				//	���j���[�A�C�R������ʂɓ����Ă���Ƃ��̃X�s�[�h���恙
	static constexpr float SELECT_ITEM_INTERVAL = 0.35f;			//	���̃��j���[�A�C�R������ʂɓ����Ă���Ƃ��̎��ԍ����恙
	static constexpr float SELECT_ITEM_POSITION_GAP_Y = 0.2f;		//	UI���Ƃ̂��炵�����恙
	static constexpr float SELECT_ITEM_SIZE_X = 0.12f;				//	UI��X�T�C�Y���恙
	static constexpr float SELECT_ITEM_SIZE_Y = 0.06f;				//	UI��Y�T�C�Y���恙
	static constexpr float SELECT_SIZE_X = 0.18f;					//	UI�I������X�T�C�Y���恙
	static constexpr float SELECT_SIZE_Y = 0.09f;					//	UI�I������Y�T�C�Y���恙
	static constexpr float SIZE_DOWN_SPEED = 0.01f;					//	UI�I������UI�̊g�呬�x���恙
	static constexpr float SIZE_UP_SPEED = 0.01f;					//	UI��I����UI�̏k�����x���恙

	static constexpr float CURSOR_START_POS_X = 2.0f;				//	�J�[�\���̏����ʒu���恙
	static constexpr float CURSOR_POSITION_X = 0.25f;				//	�J�[�\����X�ʒu���恙
	static constexpr float CURSOR_SIZE_X = 0.032f;					//	�J�[�\����X�T�C�Y���恙
	static constexpr float CURSOR_SIZE_Y = 0.02f;					//	�J�[�\����Y�T�C�Y���恙
	static constexpr float CURSOR_SPEED = 0.005f;					//	�J�[�\���̈ړ����x���恙
	//static constexpr float CURSOR_SHAKING_TIME = 1000.0f;			//	
	//static constexpr float CURSOR_SHAKING_GAP = 0.02f;				//	
	//static constexpr float CURSOR_SHAKING_SPEED = 0.005f;			//	
	static constexpr float BLINK_SPEED = 0.05f;						//	
	static constexpr float CHANGE_TIME = 1000.0f;					//	
	static constexpr float FADE_OUT_SPEED = 0.001f;					//	

	GameUI* m_owner;

	Crown::RenderObject::UiGraphic m_titleLogo;	
	Crown::RenderObject::UiGraphic m_start;		
	Crown::RenderObject::UiGraphic m_end;		
	Crown::RenderObject::UiGraphic m_option;	
	Crown::RenderObject::UiGraphic m_cursor;	

	enum class IState
	{
		FadeIn,
		PleaseSpaceKey,
		Menu,
		FadeOut,
	};
	class UITitleState;
	class UITitleFadeIn;
	class UITitlePleaseSpaceKey;
	class UITitleMenu;
	class UITitleFadeOut;

	Crown::FiniteStateMachine<IState, UITitleState> m_titleState;
};