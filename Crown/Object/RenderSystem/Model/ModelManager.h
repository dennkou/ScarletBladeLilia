#pragma once
#ifndef CROWN_RENDEROBJECT_MODELMANAGER
#define CROWN_RENDEROBJECT_MODELMANAGER
#include "MaterialTag.h"
#include <d3d12.h>
#include <vector>
#include "Model.h"
#include "./../TextureBuffer.h"
#include "./../DirectX12Wraps/GraphicsCommandList.h"
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/6/28
		// �쐬�� : �����D��
		// 
		// ���f���̊Ǘ����s���N���X���恙
		// 
		// �O���ō쐬�����model�N���X�ƃ��C�u���������̏��̂������s���恙
		// 
		//================================================
		class Model;
		class Model::ModelLoader;
		class ModelManager
		{
		public:
			ModelManager();
			~ModelManager();

			void Initialize(ID3D12Device* device, TextureBuffer* textureBuffer);

			void AddModel(Model* newModel);
			void DeleteModel(Model* deleteModel);

			void Draw(MaterialTag drawTag, GraphicsCommandList& commandList);

			void LoadModel(Model::ModelLoader* modelLoader);
			void CreateBundle(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& bundleCommandList, Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& bundleCommandAllocator);
		private:
			ID3D12Device* m_device;
			std::vector<Model*> m_models;
			TextureBuffer* m_textureBuffer;
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MODELMANAGER