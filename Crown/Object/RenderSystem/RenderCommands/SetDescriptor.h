#pragma once
#ifndef CROWN_RENDEROBJECT_RENDERCOMMAND_SETDESCRIPTOR
#define CROWN_RENDEROBJECT_RENDERCOMMAND_SETDESCRIPTOR
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
			// �f�B�X�N���v�^���Z�b�g����
			// 
			//====================================================
			class SetDescriptor : public RenderCommandBase
			{
			public:
				SetDescriptor(unsigned int descriptorOffset, unsigned int rootParameterIndex);
				virtual ~SetDescriptor();

				void Run(ID3D12GraphicsCommandList* commandList) override;
			private:
				unsigned int m_offset;
				unsigned int m_rootParameterIndex;
			};
		}
	}
}
#endif // !CROWN_RENDEROBJECT_RENDERCOMMAND_RENDERCOMMANDBASE