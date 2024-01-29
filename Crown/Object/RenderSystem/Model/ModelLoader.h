#pragma once
#ifndef CROWN_RENDEROBJECT_MODELLOADER
#define CROWN_RENDEROBJECT_MODELLOADER
#include "Model.h"
#include "./../TextureBuffer.h"
#include <memory>

namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/7/6
		// �쐬�� : �����D��
		// 
		// ���f���f�[�^�̓ǂݍ��݂��s���N���X���恙
		// model�N���X�̓��A���f���f�[�^�̍쐬�����𕪗��������̂��恙
		//
		//================================================
		class Model::ModelLoader
		{
		public:
			enum class LoadFile
			{
				PMD,
				PMX,
			};

			ModelLoader(const std::wstring& fileName, LoadFile loadMode, Model& model);
			~ModelLoader();

			void Load(ID3D12Device* device, TextureBuffer* textureBuffer);
		private:

			Model& m_model;
			std::wstring m_filePath;
			LoadFile m_loadMode;
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MODELLOADER