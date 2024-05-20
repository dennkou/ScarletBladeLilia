#pragma once
#ifndef CROWN_RENDERSYSTEM
#define CROWN_RENDERSYSTEM
#include <d3d12.h>
#include "DirectX12Wraps/GraphicsCommandList.h"
#include "DirectX12Wraps/SwapChain.h"
#include "Model/ModelManager.h"
#include "DirectX12Wraps/RootSignature.h"
#include <wrl.h>
#include "TextureBuffer.h"
#include "./RenderTarget.h"
#include <vector>

namespace Crown
{
	namespace RenderObject
	{
	//================================================================================
	// �쐬�� : 2023/6/27
	// �쐬�� : �����D��
	// 
	// �`��S�̂̓������s���N���X���恙
	// 
	//================================================================================
		class RenderSystem
		{
		public:
			RenderSystem(Crown::Window& renderTargetWindow);
			~RenderSystem();

			/// <summary>
			/// �`��G���W���S�̂̏�����������恙
			/// </summary>
			void Initialize();

			void Finalize();

			/// <summary>
			/// �\�����e�̍X�V������恙
			/// </summary>
			void Update();

			
			void AddRenderTarget(int priority, std::shared_ptr<RenderTarget> renderTarget);

			ModelManager& GetModelManager() { return m_modelManager; }
			Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return m_device; }
			TextureBuffer& GetTextureBuffer() { return m_textureBuffer; }
		private:
			Crown::Window& m_renderTargetWindow;			//	�`��Ώۂ̃E�B���h�E���恙

			Microsoft::WRL::ComPtr<ID3D12Device> m_device;

			GraphicsCommandList m_commandList;
			SwapChain m_swapChain;

			std::vector<std::pair<int, std::shared_ptr<RenderTarget>>> m_renderTargets;
			ModelManager m_modelManager;
			TextureBuffer m_textureBuffer;

			static constexpr float CLEAR_COLOR[4] = {0.0f, 0.0f, 0.0f, 1.0f};
		};
	}
}
#endif // !CROWN_RENDERSYSTEM