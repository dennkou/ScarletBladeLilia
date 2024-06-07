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
		class IModel;
		class IModel::IModelLoader;
		class ModelManager
		{
		public:
			ModelManager();
			~ModelManager();

			void Initialize(ID3D12Device* device, TextureBuffer* textureBuffer);

			void AddModel(IModel* newModel);
			void DeleteModel(IModel* deleteModel);

			void DataCopy();
			void Draw(MaterialTag drawTag, GraphicsCommandList& commandList, unsigned int index);

			void LoadModel(IModel::IModelLoader* modelLoader);

			void StackDataUploadQueue(IModel* model);
		private:
			void ResetBundle();

			ID3D12Device* m_device;
			std::vector<IModel*> m_models;
			TextureBuffer* m_textureBuffer;

			std::vector<bool> m_bundle;
			std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>		m_bundleCommandLists;			//	�`��킲�Ƃ̃o���h���R�}���h���X�g��
			std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>			m_bundleCommandAllocators;		//	�`��킲�Ƃ̃o���h���R�}���h�A���P�[�^�[��
			std::vector<IModel*> m_uploadQueue;
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MODELMANAGER