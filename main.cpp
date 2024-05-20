#include <Windows.h>
#include <iostream>
#include "Crown/System.h"
#include "Game/Game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//	�����͎g��Ȃ��恙
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

#ifdef _DEBUG
	// ���������[�N�`�F�b�N�̂��߂̃f�o�b�O�t���O��ݒ肷��恙
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// ���|�[�g���[�h��ݒ肷��恙
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	AllocConsole();
	//	�W�����o�͂Ɋ��蓖�Ă�恙
	FILE* console = NULL;
	freopen_s(&console, "CONOUT$", "w+", stdout);
	freopen_s(&console, "CONIN$", "r+", stdin);
#endif // _DEBUG


	Crown::System& crown = Crown::System::GetInstance();
	crown.Initialize();
	Game* game = new Game;

	while (!crown.GetEndFlag() && !game->GetGameEndFlag())
	{
		game->Update();
		crown.Update();
	}

	crown.Finalize();

	delete game;

	return 0;
}