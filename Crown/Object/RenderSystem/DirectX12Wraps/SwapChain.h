#pragma once
#ifndef CROWN_RENDER_OBJECT_SWAPCHAIN
#define CROWN_RENDER_OBJECT_SWAPCHAIN

#include <dxgi1_6.h>
#include <d3d12.h>
#include <vector>
#include <wrl.h>
namespace Crown
{
	class Window;

	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/6/21
		// �쐬�� : �����D��
		// 
		// �X���b�v�`�F�[�������b�v����N���X���恙
		// 
		//================================================
		class SwapChain
		{
		public:
			SwapChain();
			~SwapChain();

			/// <summary>
			/// ������������恙
			/// </summary>
			/// <param name="dxgiFactory"></param>
			/// <param name="device"></param>
			/// <param name="window"></param>
			/// <param name="commandQueue"></param>
			/// <param name="bufferNum"></param>
			void Initialize(ID3D12Device* device, const Crown::Window* const window, ID3D12CommandQueue* commandQueue, unsigned int bufferNum = 3);

			/// <summary>
			/// �X���b�v�`�F�[�����t���b�v����恙
			/// </summary>
			/// <param name="syncInterval"></param>
			void Present(UINT syncInterval);

			ID3D12DescriptorHeap* GetRenderTargetDescriptorHeap() const { return m_rtvHeaps.Get(); }
			ID3D12DescriptorHeap* GetDepthStencilDescriptorHeap() const { return m_dsvHeap.Get();  }
			ID3D12Resource* GetBackBuffer() const { return m_backBuffers[m_backBufferIndex].Get(); }
			UINT GetBackBufferIndex() const { return m_backBufferIndex; }
			unsigned int GetRenderTargetViewSize() const { return RTVSIZE; }

		private:
			inline void CreateSwapChain(IDXGIFactory2* dxgiFactory, UINT width, UINT height, HWND windowHandle, ID3D12CommandQueue* commandQueue);	//	�X���b�v�`�F�[���̍쐬���恙
			inline void CreateRenderTargetView(ID3D12Device* device);																				//	�����_�[�^�[�Q�b�g�r���[�̍쐬���恙
			inline void CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height);															//	�[�x�A�X�e���V���r���[�̍쐬���恙



			const unsigned int									RTVSIZE;																			//	�����_�[�^�[�Q�b�g�r���[�̃T�C�Y���恙

			UINT												m_backBufferCount;																	//	�o�b�N�o�b�t�@�[�̐����恙
			UINT												m_backBufferIndex;																	//	�o�b�N�o�b�t�@�[�̃C���f�b�N�X���恙

			Microsoft::WRL::ComPtr<IDXGISwapChain4>				m_swapchain;																		//	�X���b�v�`�F�[�����恙
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_rtvHeaps;																			//	�����_�[�^�[�Q�b�g�r���[�̃q�[�v�̈悾�恙
			std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>	m_backBuffers;																		//	�����_�[�^�[�Q�b�g�q�[�v���恙�o�b�N�o�b�t�@�[�ƍ��킳���Ă���恙
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_dsvHeap;																			//	�f�v�X�X�e���V���q�[�v���恙
			Microsoft::WRL::ComPtr<ID3D12Resource>				m_depthBuffer;																		//	�f�v�X�X�e���V���o�b�t�@�[���恙
		};
	}
}
#endif // !CROWN_RENDER_OBJECT_SWAPCHAIN