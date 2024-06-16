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
		// �쐬�� : 2023/9/11
		// �쐬�� : �����D��
		// 
		// UI�̊Ǘ����s���N���X���恙
		// 
		// �O���ō쐬�����ui�N���X�ƃ��C�u���������̏��̂������s���恙
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

			Vertices m_vertices;																	//	���_�f�[�^��
			Microsoft::WRL::ComPtr<ID3D12RootSignature>	m_rootSignature;							//	���[�g�V�O�l�`���[���恙
			D3D12_GRAPHICS_PIPELINE_STATE_DESC m_defaultGraphicsPipelineStateDesc;					//	
			ID3D12Device* m_device;																	//	
			std::vector<Ui*> m_ui;																	//	�Ǘ����Ă���UI�̔z�񂾂恙
			TextureBuffer* m_textureBuffer;															//	�e�N�X�`���o�b�t�@�ւ̃|�C���^��

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
			std::unique_ptr<CommandAllocator> m_commandAllocator;
			bool m_isBundle;
		};
	}
}