#include "Shader.h"
#include <sys/stat.h>
#include <filesystem>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG


Crown::RenderObject::Shader* Crown::RenderObject::Shader::me = nullptr;

Crown::RenderObject::Shader::Shader()
	:
	blob()
{
	Initialize();
}

Crown::RenderObject::Shader::~Shader()
{
	for (std::pair<const std::wstring, Microsoft::WRL::ComPtr<ID3DBlob>>& deleteBlob : blob)
	{
		deleteBlob.second->Release();
	}
}

Crown::RenderObject::Shader* Crown::RenderObject::Shader::GetInstance()
{
	if (me == nullptr)
	{
		me = new Shader();
	}
	return me;
}

void Crown::RenderObject::Shader::DeleteInstance()
{
	if (me != nullptr)
	{
		delete me;
		me = nullptr;
	}
}

ID3DBlob *const Crown::RenderObject::Shader::GetShader(const std::wstring& name)
{
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3DBlob>>::iterator data = blob.find(name);	//	�f�[�^�����邩�m�F��
	if (data == blob.end())
	{
		//	�Ȃ������I��
		std::wstring loadFile = name;
		LoadShader(loadFile);	//	�ǂݍ��݂����݂�恙

		data = blob.find(name);
		if (data == blob.end())
		{
			return nullptr;		//	����ςȂ������I��
		}
		return data->second.Get();	//	�������`��
	}
	return data->second.Get();
}





//==================================================================================================================================





void Crown::RenderObject::Shader::Initialize()
{
	//	�V�F�[�_�[�t�@�C����T���đS�ēǂߍ��ނ恙
	static const unsigned int searchFileExtensionSize = 5;
	static const wchar_t searchFileExtension[searchFileExtensionSize] = L".cso";			//	�V�F�[�_�[�t�@�C���̊g���q���恙
	static const wchar_t searchDirectory[19] = L"./Resource/Shader/";						//	�V�F�[�_�[�t�@�C����T���t�@�C���p�X���恙
	for (const std::filesystem::directory_entry& directory : std::filesystem::recursive_directory_iterator(searchDirectory))
	{
		//	�擾�����t�@�C���p�X���V�F�[�_�[�t�@�C�����ǂ������m�F��
		std::wstring filePath = directory.path().wstring();
		if (filePath.find(searchFileExtension) == filePath.length() - static_cast<size_t>(searchFileExtensionSize - 1))
		{
			LoadShader(filePath);	//	�V�F�[�_�[�t�@�C����ǂݍ��݁�
		}
	}
}

void Crown::RenderObject::Shader::LoadShader(std::wstring& fileName)
{
	//	�t�@�C�����J���邩�m�F����恙
	FILE* file = nullptr;
	_wfopen_s(&file, fileName.data(), L"rb");
	if (file == nullptr)											//	�t�@�C�����J�����̂��m�F����恙
	{
		return;
	}

	ID3DBlob* LoadBlob = nullptr;									//	�V�F�[�_�[�t�@�C���̃f�[�^�����郁�����̈悾�恙
	struct _stat st;												//	�t�@�C���ݒ�̍\���̂��恙
	_wstat(fileName.data(), &st);									//	�t�@�C�����̎擾���恙

	D3DCreateBlob(static_cast<SIZE_T>(st.st_size), &LoadBlob);		//	�������̈�̊m�ۂ��恙

	fread(LoadBlob->GetBufferPointer(), LoadBlob->GetBufferSize(), 1, file);
	fclose(file);

	//	�t�@�C�����̎擾���恙
	fileName.erase(fileName.begin(), fileName.begin() + 18);		//	���ʂ̃p�X���폜����恙
	fileName.erase(fileName.end() - 4, fileName.end());				//	�g���q���폜����恙
	//	�t�@�C�����́���/�ɕϊ�����恙
	fileName.insert(fileName.begin() + fileName.find(L"\\"),L'/');
	fileName.erase(fileName.begin() + fileName.find(L"\\"));

	blob[fileName] = LoadBlob;
}



void Crown::RenderObject::Shader::CompileFile(std::wstring& fileName, ID3DInclude* include, LPCSTR entrypoint, LPCSTR target, UINT flage)
{
	ID3DBlob* CompileBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	if (S_OK != D3DCompileFromFile(fileName.data(), nullptr, include, entrypoint, target, flage, 0, &CompileBlob, &errorBlob))
	{
#ifdef _DEBUG
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		std::cout << errstr.c_str() << std::endl;
#endif // _DEBUG
	}
}
