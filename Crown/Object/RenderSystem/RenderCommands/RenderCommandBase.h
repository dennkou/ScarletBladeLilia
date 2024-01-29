#pragma once
#ifndef CROWN_RENDEROBJECT_RENDERCOMMAND_RENDERCOMMANDBASE
#define CROWN_RENDEROBJECT_RENDERCOMMAND_RENDERCOMMANDBASE
#include <d3d12.h>
#include <iostream>
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
			class RenderCommandBase
			{
			public:
				RenderCommandBase() = default;
				virtual ~RenderCommandBase() = default;

				virtual void Run(ID3D12GraphicsCommandList* commandList) = 0;
			private:
			};
		}
	}
}
#endif // !CROWN_RENDEROBJECT_RENDERCOMMAND_RENDERCOMMANDBASE