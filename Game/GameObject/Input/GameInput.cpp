#include "GameInput.h"
#include "./../../Crown/Object/Input.h"
#include "./../../Crown/System.h"

GameInput::GameInput(Game* game)
	:
	GameObject(game),
	m_input(0,0),
	m_mouseMove(true),
	m_mouse(0,0)
{
}

GameInput::~GameInput()
{
}

void GameInput::OnGameUpdate(Timer& timer)
{
	timer;

	//	キーボードの入力受け取りだよ☆(メニュー時)
	const Crown::Input::IKeyboard* keyboard = Crown::Input::GetKeyboard();
	if (keyboard->keyDown(Crown::KeyCode::Space))
	{
		EventTrigger(&GameObject::OnInputSelect);
	}
	if (keyboard->keyDown(Crown::KeyCode::W) || keyboard->keyDown(Crown::KeyCode::UP))
	{
		EventTrigger(&GameObject::OnInputCursorUp);
	}
	if (keyboard->keyDown(Crown::KeyCode::S) || keyboard->keyDown(Crown::KeyCode::DOWN))
	{
		EventTrigger(&GameObject::OnInputCursorDown);
	}
	if (keyboard->keyDown(Crown::KeyCode::A) || keyboard->keyDown(Crown::KeyCode::LEFT))
	{
		EventTrigger(&GameObject::OnInputCursorLeft);
	}
	if (keyboard->keyDown(Crown::KeyCode::D) || keyboard->keyDown(Crown::KeyCode::RIGHT))
	{
		EventTrigger(&GameObject::OnInputCursorRight);
	}
	if (keyboard->keyDown(Crown::KeyCode::ESC))
	{
		EventTrigger(&GameObject::OnInputMenu);
	}
	//	キーボードの入力受け取りだよ☆(プレイ中)
	DirectX::XMFLOAT2 input = DirectX::XMFLOAT2(0.0f, 0.0f);
	if (Crown::Input::GetKeyboard()->key(Crown::KeyCode::A))
	{
		input.x = -1;
	}
	if (Crown::Input::GetKeyboard()->key(Crown::KeyCode::D))
	{
		input.x = 1;
	}
	if (Crown::Input::GetKeyboard()->key(Crown::KeyCode::W))
	{
		input.y = 1;
	}
	if (Crown::Input::GetKeyboard()->key(Crown::KeyCode::S))
	{
		input.y = -1;
	}
	if (m_input.x != input.x || m_input.y != input.y)
	{
		m_input = input;
		EventTrigger(&GameObject::OnInputMove, input);
	}

	//	マウス入力受け取りだよ☆(プレイ中)
	if (!m_mouseMove)
	{
		int windowHalfSizeY = Crown::System::GetInstance().GetWindowSizeY() / 2;
		int windowHalfSizeX = Crown::System::GetInstance().GetWindowSizeX() / 2;

		DirectX::XMFLOAT2 mouse = Crown::Input::GetMouse()->GetMousePosint();
		mouse.x -= windowHalfSizeX;
		mouse.y -= windowHalfSizeY;
		//	どちらかから入力があればマウスポインターを原点に移動させるよ☆
		if ((static_cast<int>(Crown::Input::GetMouse()->GetMousePosint().y - windowHalfSizeY) != 0) ||
			(static_cast<int>(Crown::Input::GetMouse()->GetMousePosint().x - windowHalfSizeX) != 0))
		{
			Crown::Input::GetMouse()->SetPoint(windowHalfSizeX, windowHalfSizeY, Crown::System::GetInstance().GetWindow());
		}

		if (mouse.x != m_mouse.x || mouse.y != m_mouse.y)
		{
			EventTrigger(&GameObject::OnInputCamera, mouse);
			m_mouse = mouse;
		}
	}
	if (Crown::Input::GetMouse()->GetLButtonDown())
	{
		EventTrigger(&GameObject::OnInputAttackDown);
	}
}

void GameInput::OnInputMenu()
{
	m_mouseMove = !m_mouseMove;

	ShowCursor(m_mouseMove);

	int windowHalfSizeY = Crown::System::GetInstance().GetWindowSizeY() / 2;
	int windowHalfSizeX = Crown::System::GetInstance().GetWindowSizeX() / 2;
	Crown::Input::GetMouse()->SetPoint(windowHalfSizeX, windowHalfSizeY, Crown::System::GetInstance().GetWindow());
	m_mouse = DirectX::XMFLOAT2(0, 0);
	EventTrigger(&GameObject::OnInputCamera, m_mouse);
}

void GameInput::OnPlayStart()
{
	EventTrigger(&GameObject::OnInputMenu);
}