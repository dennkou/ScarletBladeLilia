#pragma once
#ifndef CROWN_RENDEROBJECT_RENDERCOMMAND_RENDERCOMMANDQUEUE
#define CROWN_RENDEROBJECT_RENDERCOMMAND_RENDERCOMMANDQUEUE
#include <d3d12.h>
#include <vector>
#include <memory>
#include <wrl.h>
#include "RenderCommandBase.h"
namespace Crown
{
	namespace RenderObject
	{
		namespace RenderCommand
		{
			//================================================
			// �쐬�� : 2023/7/12
			// �쐬�� : �����D��
			// 
			// �����^�����O�p�R�}���h��ۑ����A���s����N���X���恙
			// 
			//================================================
			class RenderCommandQueue
			{
			public:
				RenderCommandQueue();
				RenderCommandQueue(std::vector<std::shared_ptr<RenderCommandBase>>& renderCommands, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& useResource);
				~RenderCommandQueue();

				void Run(ID3D12GraphicsCommandList* commandList, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& useResource) const;
			private:
				std::vector<std::shared_ptr<RenderCommandBase>>		m_renderCommands;
				std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_useResource;			//	���̃����^�����O�R�}���h�Ŏg�p���郊�\�[�X�ւ̃|�C���^�[���恙
			};
		}
	}
}
#endif // !CROWN_RENDEROBJECT_RENDERCOMMAND_RENDERCOMMANDQUEUE