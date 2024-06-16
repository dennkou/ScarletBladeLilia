#pragma once
#ifndef CROWN_RENDEROBJECT_MODELLOADER
#define CROWN_RENDEROBJECT_MODELLOADER
#include "Model.h"
#include "IModel.h"
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
		class Model::ModelLoader : public IModel::IModelLoader
		{
		public:
			enum class LoadFile
			{
				PMD,
				PMX
			};

			ModelLoader(const std::wstring& fileName, LoadFile loadMode, Model& model);
			~ModelLoader();

			virtual void Load(ID3D12Device* device, TextureBuffer* textureBuffer);
		private:
			std::wstring m_filePath;
			LoadFile m_loadMode;
		protected:
			ModelLoader(Model& model) :m_model(model){};
			Model& m_model;
		};

		class Model::CreateModel : public Model::ModelLoader
		{
		public:
			CreateModel(const std::initializer_list<ColliderAlgorithm::Triangle>& collider, DirectX::XMFLOAT4 color, Model& model);
			~CreateModel();

			virtual void Load(ID3D12Device* device, TextureBuffer* textureBuffer) override;
		private:
			const std::initializer_list<ColliderAlgorithm::Triangle>& m_collider;
			DirectX::XMFLOAT4 m_color;
		};

		class Model::CopyModel : public Model::ModelLoader
		{
		public:
			CopyModel(Model& model, const Model& copyModel);
			~CopyModel();

			virtual void Load(ID3D12Device* device, TextureBuffer* textureBuffer) override;
		private:
			const Model& copyModel;
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MODELLOADER