#pragma once
#include <d3d12.h>
#include "./../TextureBuffer.h"
#include "./../DirectX12Wraps/GraphicsCommandList.h"
#include "./../Model/Vertices.h"
#include "./../DirectX12Wraps/GraphicPipeline.h"
#include "./../DirectX12Wraps/CommandAllocator.h"
#include <memory>
#include "Ui.h"
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// 作成日 : 2023/9/11
		// 作成者 : 服部颯澄
		// 
		// UIの管理を行うクラスだよ☆
		// 
		// 外部で作成されるuiクラスとライブラリ内部の情報のやり取りを行うよ☆
		// 
		//================================================
		class UiManager
		{
		public:
			~UiManager() = default;
			static UiManager* GetInstance();

			void AddUi(Ui* newUi);
			void DeleteUi(Ui* newUi);

			void Initialize(ID3D12Device* device, TextureBuffer* textureBuffer);

			void Render(GraphicsCommandList& commandList);

			ID3D12Device* GetDevice() noexcept { return m_device; }
			TextureBuffer* GetTextureBuffer() noexcept { return m_textureBuffer; }
			D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetDefaultGraphicsPipelineStateDesc() noexcept { return m_defaultGraphicsPipelineStateDesc; }
		private:
			UiManager();
			UiManager(UiManager& uiManager) = delete;
			UiManager& operator=(UiManager& uiManager) = delete;
			static std::unique_ptr<UiManager> me;

			void CreatRootSignature(unsigned int texNum);
			void CreatVertices();
			void CreatDefaultGraphicsPipelineStateDesc();

			static constexpr unsigned int INPUTLAYOUT_NUM = 2;
			static constexpr D3D12_INPUT_ELEMENT_DESC INPUTLAYOUT[INPUTLAYOUT_NUM] =
			{
				{ "POSITION",		0,DXGI_FORMAT_R32G32B32_FLOAT,		0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
				{ "UV",				0,DXGI_FORMAT_R32G32_FLOAT,			0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
			};

			Vertices m_vertices;																	//	頂点データ☆
			Microsoft::WRL::ComPtr<ID3D12RootSignature>	m_rootSignature;							//	ルートシグネチャーだよ☆
			D3D12_GRAPHICS_PIPELINE_STATE_DESC m_defaultGraphicsPipelineStateDesc;					//	
			ID3D12Device* m_device;																	//	
			std::vector<Ui*> m_ui;																	//	管理しているUIの配列だよ☆
			TextureBuffer* m_textureBuffer;															//	テクスチャバッファへのポインタ☆

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
			std::unique_ptr<CommandAllocator> m_commandAllocator;
			bool m_isBundle;
		};
	}
}