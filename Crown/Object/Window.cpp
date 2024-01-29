#include "Window.h"

#include "Window.h"
#include <tchar.h>

//	�f�t�H���g�̃X�N���[���T�C�Y���恙
#define DEFAULT_WINDOW_WIDTH	1280
#define DEFAULT_WINDOW_HEIGHT	720

Crown::Window::Window()
	:
	m_width(DEFAULT_WINDOW_WIDTH),
	m_height(DEFAULT_WINDOW_HEIGHT),
	m_hwnd(0),
	m_endFlag(false)
{

}

Crown::Window::~Window()
{

}

void Crown::Window::MakeWindow(std::wstring windowName, WNDPROC windowProcedure, DWORD dwStyle)
{
	WNDCLASSEX w = {};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = windowProcedure;			//	�E�B���h�E�v���[�W���̎w�肾�恙
	w.lpszClassName = windowName.data();		//	�A�v���P�[�V�����̖��O���恙
	w.hInstance = GetModuleHandle(0);			//	�A�v���P�[�V�����n���h���̎w�肾�恙

	RegisterClassEx(&w);						//	�E�B���h�E�N���X��o�^����恙

	RECT wrc = { 0,0, m_width, m_height };		//	�E�B���h�E�T�C�Y�����߂�恙
	AdjustWindowRect(&wrc, dwStyle, false);		//	�N���C�A���g�T�C�Y����E�B���h�E�T�C�Y���v�Z����恙
	//�E�B���h�E�I�u�W�F�N�g�̐���
	m_hwnd = CreateWindow
	(
		windowName.data(),						//	�N���X���w�肾�恙
		windowName.data(),						//	�^�C�g���o�[�̕������恙
		dwStyle,								//	�E�B���h�E�^�C�v���w�肳�ꂽ���̂Ɂ�
		CW_USEDEFAULT,							//	�\��X���W��OS�ɔC����恙
		CW_USEDEFAULT,							//	�\��Y���W��OS�ɔC����恙
		wrc.right - wrc.left,					//	�E�B���h�E���̎w�肾�恙
		wrc.bottom - wrc.top,					//	�E�B���h�E���̎w�肾�恙
		nullptr,								//	�e�E�B���h�E�n���h���͂Ȃ��恙
		nullptr,								//	���j���[�n���h���͂Ȃ��恙
		GetModuleHandle(0),						//	�Ăяo���A�v���P�[�V�����n���h���̎w�肾�恙
		nullptr									//	�ǉ��p�����[�^�͂Ȃ��恙
	);
}

void Crown::Window::DisplayWindow()
{
	if (m_hwnd != 0)
	{
		ShowWindow(m_hwnd, SW_SHOW);					//	�E�B���h�E�\������恙
	}
}

void Crown::Window::ProcessMessage()
{
	MSG msg;											//	�E�B���h�E���b�Z�[�W�̎擾�p�ϐ����恙
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))	//	���܂��Ă���E�B���h�E���b�Z�[�W�����ׂď�������恙
	{
		TranslateMessage(&msg);							//	�E�B���h�E���b�Z�[�W�̎擾������恙
		DispatchMessage(&msg);							//	�E�B���h�E�v���[�W���̌Ăяo�����恙
	}
	if (msg.message == WM_QUIT)
	{
		m_endFlag = true;
	}
}

void Crown::Window::GetWindowPosition(int& x, int& y) const
{
	WINDOWINFO windowInfo;

	//�E�B���h�E�̈ʒu���擾����恙
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(m_hwnd, &windowInfo);

	x = windowInfo.rcWindow.left;
	y = windowInfo.rcWindow.top + 35;

}