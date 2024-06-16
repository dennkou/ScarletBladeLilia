#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "./../Model/MaterialTag.h"
#include "./../DirectX12Wraps/GraphicsCommandList.h"
#include "./../Model/ModelManager.h"
#include "./../DirectX12Wraps/DescriptorHeaps.h"
#include "./IRenderTarget.h"

namespace Crown
{
	namespace RenderObject
	{
		class RenderTarget : public IRenderTarget
		{
		public:
			RenderTarget(MaterialTag materialTag, unsigned int x, unsigned int y, DirectX::XMFLOAT4 clearColor, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
			~RenderTarget() override;

			void Initialize(ID3D12Device* device) override;
			void Draw(GraphicsCommandList& commandList, ModelManager* modelManager, unsigned int index) override;

			unsigned int GetTexture() const;
			unsigned int GetDepth() const;
			ID3D12DescriptorHeap* GetRenderTargetDescriptorHeap() const { return m_rtvHeaps.Get(); }
			ID3D12DescriptorHeap* GetDepthStencilDescriptorHeap() const { return m_dsvHeap.Get(); }

		private:
			inline void CreateRenderTarget(ID3D12Device* device, UINT width, UINT height);		//	�����_�[�^�[�Q�b�g�r���[�̍쐬���恙
			inline void CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height);			//	�[�x�A�X�e���V���r���[�̍쐬���恙

			MaterialTag m_materialTag;
			unsigned int m_xSize;
			unsigned int m_ySize;
			DirectX::XMFLOAT4 CLEAR_COLOR;
			unsigned int m_texture;
			unsigned int m_depth;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_rtvHeaps;			//	�����_�[�^�[�Q�b�g�r���[�̃q�[�v�̈悾�恙
			Microsoft::WRL::ComPtr<ID3D12Resource>			m_rtvResource;		//	�����_�[�^�[�Q�b�g�q�[�v���恙�o�b�N�o�b�t�@�[�ƍ��킳���Ă���恙
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_dsvHeap;			//	�f�v�X�X�e���V���q�[�v���恙
			Microsoft::WRL::ComPtr<ID3D12Resource>			m_depthResource;	//	�f�v�X�X�e���V���o�b�t�@�[���恙
			DXGI_FORMAT m_format;
		};
	}
}