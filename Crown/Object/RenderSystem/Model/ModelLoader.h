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
		// 作成日 : 2023/7/6
		// 作成者 : 服部颯澄
		// 
		// モデルデータの読み込みを行うクラスだよ☆
		// modelクラスの内、モデルデータの作成部分を分離したものだよ☆
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