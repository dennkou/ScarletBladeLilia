#pragma once
#ifndef CROWN_RENDEROBJECT_RENDERCOMMAND_SETINDEXBUFFER
#define CROWN_RENDEROBJECT_RENDERCOMMAND_SETINDEXBUFFER
#include "RenderCommandBase.h"
namespace Crown
{
	namespace RenderObject
	{
		namespace RenderCommand
		{
			//====================================================
			// �쐬�� : 2023/7/12
			// �쐬�� : �����D��
			// 
			// �����^�����O�R�}���h�̊��N���X���恙
			// 
			//====================================================
			class SetIndexBuffer : public RenderCommandBase
			{
			public:
				SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* indexBufferView);
				virtual ~SetIndexBuffer();

				virtual void Run(ID3D12GraphicsCommandList* commandList) override;
			private:
				D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
			};
		}
	}
}
#endif //!CROWN_RENDEROBJECT_RENDERCOMMAND_SETINDEXBUFFER