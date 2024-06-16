#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "./../Model/MaterialTag.h"
#include "./../DirectX12Wraps/GraphicsCommandList.h"
#include "./../Model/ModelManager.h"
#include "./../DirectX12Wraps/DescriptorHeaps.h"
#include "./IRenderTarget.h"
#include <vector>
#include <initializer_list>

namespace Crown
{
	namespace RenderObject
	{
		class MultipleRenderTarget : public IRenderTarget
		{
		public:
			struct RenderTargetSetting
			{
				DirectX::XMFLOAT4 clearColors;
				DXGI_FORMAT format;
				unsigned int width;
				unsigned int height;

			};
			MultipleRenderTarget(MaterialTag materialTag, unsigned int width, unsigned int height, std::initializer_list<DirectX::XMFLOAT4> clearColors, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
			~MultipleRenderTarget() override;

			void Initialize(ID3D12Device* device) override;
			void Draw(GraphicsCommandList& commandList, ModelManager* modelManager, unsigned int index) override;

			inline unsigned int GetTexture(unsigned int index) const { return m_renderTarget[index].texture; }
			inline unsigned int GetDepth(unsigned int index) const { return m_renderTarget[index].depth; }
			ID3D12DescriptorHeap* GetRenderTargetDescriptorHeap() const { return m_rtvHeaps.Get(); }
			ID3D12DescriptorHeap* GetDepthStencilDescriptorHeap() const { return m_dsvHeap.Get(); }

		private:
			struct RenderTarget
			{
				DirectX::XMFLOAT4 clearColor;
				DXGI_FORMAT format;
				unsigned int width;
				unsigned int height;
				unsigned int texture;
				unsigned int depth;
				Microsoft::WRL::ComPtr<ID3D12Resource> rtvResource;		//	レンダーターゲットヒープだよ☆バックバッファーと合わさっているよ☆
				Microsoft::WRL::ComPtr<ID3D12Resource> depthResource;	//	デプスステンシルバッファーだよ☆
			};

			inline void CreateRenderTarget(ID3D12Device* device, RenderTarget& renderTarget, unsigned int index);	//	レンダーターゲットビューの作成だよ☆
			inline void CreatDepthBuffer(ID3D12Device* device, RenderTarget& renderTarget, unsigned int index);			//	深度、ステンシルビューの作成だよ☆

			static constexpr unsigned int MAX_RENDER_TARGET = 8;

			unsigned int RTVDescriptorHandleIncrementSize;
			unsigned int DSVDescriptorHandleIncrementSize;

			unsigned int m_width;
			unsigned int m_height;
			DXGI_FORMAT m_format;
			std::vector<RenderTarget> m_renderTarget;
			MaterialTag m_materialTag;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_rtvHeaps;			//	レンダーターゲットビューのヒープ領域だよ☆
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_dsvHeap;			//	デプスステンシルヒープだよ☆
		};
	}
}