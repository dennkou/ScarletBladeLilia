#pragma once
#ifndef CROWN_WINDOW
#define CROWN_WINDOW

#include <iostream>
#include <Windows.h>

namespace Crown
{
	//============================================================
	// �쐬�� : 2023/4/20
	// �쐬�� : �����D��
	// 
	// �E�B���h�E���Ǘ�����N���X���恙
	// 
	//============================================================
	class Window
	{
	public:
		Window();
		~Window();

		void MakeWindow(std::wstring windowName, WNDPROC windowProcedure, DWORD dwStyle);	//	�E�B���h�E���쐬����恙
		void DisplayWindow();																//	�E�B���h�E�\������恙
		void ProcessMessage();																//	�E�B���h�E���b�Z�[�W��S�ď�������恙
		inline HWND GetWindowHandle()	const { return m_hwnd; };							//	�E�B���h�E�n���h����Ԃ��恙
		inline bool GetEndFlag()		const { return m_endFlag; };						//	�A�v���P�[�V�����̏I���t���O��Ԃ��恙
		void GetWindowPosition(int& x, int& y) const;
		inline int GetWindowWidth()		const { return m_width; };							//	�E�B���h�E�̉�����Ԃ��恙
		inline int GetWindowHeight()	const { return m_height; };							//	�E�B���h�E�̍�����Ԃ��恙
	private:
		HWND m_hwnd;						//	�E�B���h�E�n���h�����恙
		int m_width, m_height;				//	�E�B���h�E�T�C�Y���恙
		bool m_endFlag;						//	�A�v���P�[�V�����̏I�����v������Ă����̃t���O���恙
	};
}
#endif // !CROWN_WINDOW