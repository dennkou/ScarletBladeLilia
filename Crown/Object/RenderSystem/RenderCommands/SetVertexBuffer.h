#pragma once
#ifndef CROWN_RENDEROBJECT_RENDERCOMMAND_SETVERTEXBUFFER
#define CROWN_RENDEROBJECT_RENDERCOMMAND_SETVERTEXBUFFER
#include "./RenderCommandBase.h"
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
			class SetVertexBuffer	:	public RenderCommandBase
			{
			public:
				SetVertexBuffer(UINT startSlot, UINT numViews, D3D12_VERTEX_BUFFER_VIEW* vertexBufferView);
				virtual ~SetVertexBuffer();

				virtual void Run(ID3D12GraphicsCommandList* commandList) override;
			private:
				UINT m_startSlot;
				UINT m_numViews;
				D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
			};
		}
	}
}
#endif //!CROWN_RENDEROBJECT_RENDERCOMMAND_SETVERTEXBUFFER