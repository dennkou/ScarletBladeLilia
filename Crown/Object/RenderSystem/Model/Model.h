#pragma once
#ifndef CROWN_RENDEROBJECT_MODEL
#define CROWN_RENDEROBJECT_MODEL
#include "MaterialTag.h"
#include <DirectXMath.h>
#include "Vertices.h"
#include "MaterialMesh.h"
#include <vector>
#include <d3d12.h>
#include "./../DirectX12Wraps/DescriptorHeaps.h"
#include <string>
#include "./../DirectX12Wraps/GraphicsCommandList.h"
#include "./Bone.h"
#include <d3d12.h>

namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/6/28
		// �쐬�� : �����D��
		// 
		// ���f�����恙
		// 
		//================================================
		class Model
		{
		public:
			class ModelLoader;

			Model();
			Model(const Model& model);
			~Model();

			inline DirectX::XMFLOAT3 GetRotate() const noexcept { return m_rotate; }
			inline void SetRotate(const DirectX::XMFLOAT3& rotate) noexcept { m_rotate = rotate; m_updateFlag = true; }
			inline DirectX::XMFLOAT3 GetPosition() const noexcept { return m_position; }
			inline void SetPosition(const DirectX::XMFLOAT3& position) noexcept { m_position = position; m_updateFlag = true; }
			inline const BoneData& GetBoneDate() const noexcept { return m_bone; }
			void SetPause(const DirectX::XMMATRIX* pause) noexcept;
			inline MaterialMesh& GetMaterialMesh(unsigned int index) { return m_materialMeshs[index]; }
			inline void SetDrawFlag(bool flag);

			void LoadPMD(const std::wstring& fileName);
			void LoadPMX(const std::wstring& fileName);

			inline unsigned int GetDescriptorOffest() { return m_descriptorOffset; }
			inline const Microsoft::WRL::ComPtr<ID3D12Resource>& GetModelData() { return m_resource; }

			inline D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() { return *m_vertices.GetVertexBufferView(); }
			inline const Microsoft::WRL::ComPtr<ID3D12Resource> GetConstVertexBuffer() { return m_vertices.GetConstVertexBuffer(); }
			inline D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() { return *m_vertices.GetIndexBufferView(); }
			inline const Microsoft::WRL::ComPtr<ID3D12Resource> GetConstIndexBuffer() { return m_vertices.GetConstIndexBuffer(); }

			void Draw(MaterialTag drawTag, GraphicsCommandList& commandList);
		private:
			struct ModelData		//���f���̃C���X�^���X�f�[�^��
			{
				DirectX::XMMATRIX world;
				DirectX::XMMATRIX bone[255];
			};

			//	�f�[�^�̃A�b�v���[�h���s���恙
			void DataUpload();

			DirectX::XMFLOAT3 m_position;
			DirectX::XMFLOAT3 m_rotate;
			BoneData m_bone;
			DirectX::XMMATRIX m_pause[255];

			bool m_updateFlag;																					//	�ϐ��ɍX�V�����������̃t���O���恙

			Vertices m_vertices;																				//	���_�f�[�^��
			std::vector<MaterialMesh> m_materialMeshs;															//
			unsigned int m_descriptorOffset;																	//	���f���̃C���X�^���X�f�[�^�̃I�t�Z�b�g�ʒu��
			Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;													//	���f���̃C���X�^���X�f�[�^�̃������̈恙
			std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>		m_bundleCommandLists;			//	�`��킲�Ƃ̃o���h���R�}���h���X�g��
			std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>			m_bundleCommandAllocators;		//	�`��킲�Ƃ̃o���h���R�}���h�A���P�[�^�[��
			std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>>	m_bundleResource;				//	�e�`���̕`��Ŏg�p���郊�\�[�X�f�[�^��
			std::vector<bool>													m_bundleFlag;					//	�e�`��킪�o���h��������Ă��邩�̃t���O��
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MODEL