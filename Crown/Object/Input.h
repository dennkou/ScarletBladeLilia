#pragma once
#ifndef CROWN_INPUT
#define CROWN_INPUT
//================================================
// �쐬�� : 2023/6/7
// �쐬�� : �����D��
// 
// ���͂��i��N���X���恙
// 
//================================================
#include <DirectXMath.h>
#include <Windows.h>
#include <vector>
#include <chrono>
#include <memory>
namespace Crown
{
	enum class KeyCode : int
	{
		BACKSPACE,	//	�o�b�N�X�y�[�X�L�[��
		Tab,		//	�^�u�L�[��
		Enter,		//	�G���^�[�L�[��
		Shift,		//	�V�t�g�L�[��
		Ctrl,		//	�R���g���[���L�[��
		ALT,		//	�I���g�L�[��
		CAPS_LOCK,	//	�L���v�X���b�N�L�[��
		ESC,		//	�G�X�P�[�v�L�[��
		Space,		//	�X�y�[�X�L�[��
		PAGE_UP,	//	
		PAGE_DOWN,	//	
		END,		//	
		HOME,		//	
		LEFT,		//	���L�[��
		UP,			//	���L�[��
		RIGHT,		//	���L�[��
		DOWN,		//	���L�[��
		key0,		//	0�L�[��
		key1,		//	1�L�[��
		key2,		//	2�L�[��
		key3,		//	3�L�[��
		key4,		//	4�L�[��
		key5,		//	5�L�[��
		key6,		//	6�L�[��
		key7,		//	7�L�[��
		key8,		//	8�L�[��
		key9,		//	9�L�[��
		A,			//
		B,			//
		C,			//
		D,			//
		E,			//
		F,			//
		G,			//
		H,			//
		I,			//
		J,			//
		K,			//
		L,			//
		M,			//
		N,			//
		O,			//
		P,			//
		Q,			//
		R,			//
		S,			//
		T,			//
		U,			//
		V,			//
		W,			//
		X,			//
		Y,			//
		Z,			//
		OEM_1,		//	:�L�[��
		OEM_2,		//	?�L�[��
		OEM_3,		//	@�L�[��
		OEM_4,		//	[�L�[��
		OEM_5,		//	|�L�[��
		OEM_6,		//	]�L�[��
		OEM_7,		//	~�L�[��
		PLUS,		//	+�L�[��
		COMMA,		//	,�L�[��
		MINUS,		//	-�L�[��
		PERIOD,		//	.�L�[��

		keyNum		//	�L�[�̐����恙
	};
	class Input
	{
	public:
		interface IMouse
		{
			virtual DirectX::XMFLOAT2 GetMousePosint() const = 0;
			virtual float GetWheel() const = 0;

			virtual bool GetLButton() const = 0;
			virtual bool GetLButtonUp() const = 0;
			virtual bool GetLButtonDown() const = 0;
			virtual int GetLButtonDownTime() const = 0;

			virtual bool GetRButton() const = 0;
			virtual bool GetRButtonUp() const = 0;
			virtual bool GetRButtonDown() const = 0;
			virtual int GetRButtonDownTime() const = 0;

			virtual bool GetCenterButton() const = 0;
			virtual bool GetCenterButtonUp() const = 0;
			virtual bool GetCenterButtonDown() const = 0;
			virtual int GetCenterButtonDownTime() const = 0;

			virtual void SetPoint(int x, int y, HWND hwnd) = 0;
		};
		interface IMouseMessage
		{
			virtual void LButtonUpMessage() = 0;
			virtual void LButtonDownMessage() = 0;
			virtual void RButtonUpMessage() = 0;
			virtual void RButtonDownMessage() = 0;
			virtual void MButtonUpMessage() = 0;
			virtual void MButtonDownMessage() = 0;
			virtual void MoveMessage(LPARAM lparam) = 0;
			virtual void WheelMessage(WPARAM wparam) = 0;
		};
		interface IKeyboard
		{
			virtual bool key(KeyCode keyCode) const = 0;			//	�L�[�������Ă��遙
			virtual bool keyUp(KeyCode keyCode) const = 0;			//	�L�[�𗣂����u�ԁ�
			virtual bool keyDown(KeyCode keyCode) const = 0;		//	�L�[���������u�ԁ�
			virtual int keyDownTime(KeyCode keyCode) const = 0;		//	�L�[�������Ă��鎞�Ԃ�Ԃ��恙
		};
		interface IKeyboardMessage
		{
			virtual void UpMessage(WPARAM wparam) = 0;
			virtual void DownMessage(WPARAM wparam) = 0;
		};
		~Input();

		static IMouse* const GetMouse();
		static const IKeyboard* const GetKeyboard();
		static IMouseMessage* const GetMouseMessage();
		static IKeyboardMessage* const GetKeyboardMessage();

		static void Update();
	private:
		class Mouse : public IMouse, public IMouseMessage
		{
		public:
			Mouse();
			virtual ~Mouse();

			virtual DirectX::XMFLOAT2 GetMousePosint() const;
			virtual float GetWheel() const;

			virtual bool GetLButton() const override;
			virtual bool GetLButtonUp() const override;
			virtual bool GetLButtonDown() const override;
			virtual int GetLButtonDownTime() const override;

			virtual bool GetRButton() const override;
			virtual bool GetRButtonUp() const override;
			virtual bool GetRButtonDown() const override;
			virtual int GetRButtonDownTime() const override;

			virtual bool GetCenterButton() const override;
			virtual bool GetCenterButtonUp() const override;
			virtual bool GetCenterButtonDown() const override;
			virtual int GetCenterButtonDownTime() const override;

			virtual void SetPoint(int x, int y, HWND hwnd) override;

			void Update();

			virtual void LButtonUpMessage() override;
			virtual void LButtonDownMessage() override;
			virtual void RButtonUpMessage() override;
			virtual void RButtonDownMessage() override;
			virtual void MButtonUpMessage() override;
			virtual void MButtonDownMessage() override;
			virtual void MoveMessage(LPARAM lparam) override;
			virtual void WheelMessage(WPARAM wparam) override;
		private:
			int m_x, m_y;
			float m_wheel;
			bool m_l, m_r, m_center;
			bool m_oldL, m_oldR, m_oldCenter;
			std::chrono::system_clock::time_point m_lTimer, m_rTimer, m_centerTimer;
		};
		class Keyboard : public IKeyboard, public IKeyboardMessage
		{
		public:
			Keyboard();
			virtual ~Keyboard();

			virtual bool key(KeyCode keyCode) const override;			//	�L�[�������Ă��遙
			virtual bool keyUp(KeyCode keyCode) const override;			//	�L�[�𗣂����u�ԁ�
			virtual bool keyDown(KeyCode keyCode) const override;		//	�L�[���������u�ԁ�
			virtual int keyDownTime(KeyCode keyCode) const override;	//	�L�[�������Ă��鎞�Ԃ�Ԃ��恙

			void Update();

			virtual void UpMessage(WPARAM wparam) override;
			virtual void DownMessage(WPARAM wparam) override;
		private:
			KeyCode GetKeyCode(WPARAM wparam);

			bool m_keyInput[static_cast<int>(KeyCode::keyNum)];
			bool m_oldKeyInput[static_cast<int>(KeyCode::keyNum)];
			std::chrono::system_clock::time_point m_keyTimer[static_cast<int>(KeyCode::keyNum)];
			
		};
		Input();
		

		static std::unique_ptr<Input> m_me;
		Mouse m_mouse;
		Keyboard m_keyboard;
	};
}
#endif // !CROWN_INPUT
