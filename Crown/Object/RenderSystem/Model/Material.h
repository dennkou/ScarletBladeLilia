#pragma once
#ifndef CROWN_RENDEROBJECT_MATERIAL
#define CROWN_RENDEROBJECT_MATERIAL
#include "./../DirectX12Wraps/GraphicPipeline.h"
#include "./../RenderCommands/RenderCommandQueue.h"
#include "./../DirectX12Wraps/DescriptorHeaps.h"
#include <memory>
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include "./../BlobConstBuffer/BlobConstBuffer.h"
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/7/4
		// �쐬�� : �����D��
		// 
		// �}�e���A�������i�[����恙
		// 
		//================================================
		class Material
		{
		public:
			Material();
			~Material();

			void CreateData(RenderCommand::RenderCommandQueue& commandQueue, std::vector<BlobConstBuffer>& constantBuffers);

			inline void Draw(ID3D12GraphicsCommandList* commandList, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& useResource) const
			{
				m_commandQueue.Run(commandList, useResource);
			}

			BlobConstBuffer& GetConstBuffer(unsigned int constantBufferIndex);
		private:
			std::vector<BlobConstBuffer> m_constBuffers;							//	�g�p����R���X�^���g�o�b�t�@���恙
			RenderCommand::RenderCommandQueue m_commandQueue;						//	�h���[�Ŏg�p����R�}���h���恙
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MATERIAL