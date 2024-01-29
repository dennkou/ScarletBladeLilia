#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "./Model/MaterialTag.h"
#include "./DirectX12Wraps/GraphicsCommandList.h"
#include "./Model/ModelManager.h"
#include "./DirectX12Wraps/DescriptorHeaps.h"

namespace Crown
{
	namespace RenderObject
	{
		class RenderTarget
		{
		public:
			RenderTarget(MaterialTag materialTag, unsigned int x, unsigned int y, DirectX::XMFLOAT4 clearColor);
			~RenderTarget();

			void Initialize(ID3D12Device* device);
			void Draw(GraphicsCommandList& commandList, ModelManager* modelManager);

			unsigned int GetTexture() const;
			unsigned int GetDepth() const;
			ID3D12DescriptorHeap* GetRenderTargetDescriptorHeap() const { return m_rtvHeaps.Get(); }
			ID3D12DescriptorHeap* GetDepthStencilDescriptorHeap() const { return m_dsvHeap.Get(); }

		private:
			inline void CreateRenderTargetView(ID3D12Device* device, UINT width, UINT height);		//	レンダーターゲットビューの作成だよ☆
			inline void CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height);			//	深度、ステンシルビューの作成だよ☆

			MaterialTag m_materialTag;
			unsigned int m_xSize;
			unsigned int m_ySize;
			DirectX::XMFLOAT4 CLEAR_COLOR;
			unsigned int m_texture;
			unsigned int m_depth;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_rtvHeaps;			//	レンダーターゲットビューのヒープ領域だよ☆
			Microsoft::WRL::ComPtr<ID3D12Resource>			m_rtvResource;		//	レンダーターゲットヒープだよ☆バックバッファーと合わさっているよ☆
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_dsvHeap;			//	デプスステンシルヒープだよ☆
			Microsoft::WRL::ComPtr<ID3D12Resource>			m_depthResource;	//	デプスステンシルバッファーだよ☆
		};
	}
}