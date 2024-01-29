#pragma once
#ifndef CROWN_STRING_ALGORITHM
#define CROWN_STRING_ALGORITHM
//================================================
// �쐬�� : 2023/6/14
// �쐬�� : �����D��
// 
// string,wstring�Ɋ֌W����A���S���Y����񋟂���N���X���恙
// 
//================================================
#include <string>
#include <Windows.h>
namespace Crown
{
	class StringAlgorithm
	{
	public:
		/// <summary>
		/// string��wstring�ɕϊ�����恙
		/// </summary>
		static std::wstring StringToWstring(std::string str)
		{
			//	���������擾����恙
			int num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
			std::wstring wstr;
			wstr.resize(num1);//	����ꂽ�����񐔂Ń��T�C�Y����恙
			//	�ϊ�����恙
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);
			//	�I�[�������폜����恙
			if (wstr.back() == 0)
			{
				wstr.pop_back();
			}
			return wstr;
		};
		/// <summary>
		/// wstring��string�ɕϊ�����恙
		/// </summary>
		static std::string WstringToString(std::wstring wstr)
		{
			//	���������擾����恙
			int num = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
			std::string str;
			str.resize(num);//	����ꂽ�����񐔂Ń��T�C�Y����恙
			//	�ϊ�����恙
			WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], num, nullptr, nullptr);
			return str;
		};
	};
}
#endif // !CROWN_STRING_ALGORITHM