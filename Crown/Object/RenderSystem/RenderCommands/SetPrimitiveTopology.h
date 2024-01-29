#pragma once
#ifndef CROWN_RENDEROBJECT_RENDERCOMMAND_SETPRIMITIVETOPOLOGY
#define CROWN_RENDEROBJECT_RENDERCOMMAND_SETPRIMITIVETOPOLOGY
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
			class SetPrimitiveTopology : public RenderCommandBase
			{
			public:
				SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology);
				virtual ~SetPrimitiveTopology();

				virtual void Run(ID3D12GraphicsCommandList* commandList) override;
			private:
				D3D12_PRIMITIVE_TOPOLOGY m_primitiveTopology;
			};
		}
	}
}
#endif // !CROWN_RENDEROBJECT_RENDERCOMMAND_SETPRIMITIVETOPOLOGY