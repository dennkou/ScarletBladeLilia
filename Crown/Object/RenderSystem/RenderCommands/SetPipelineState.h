#pragma once
#ifndef CROWN_RENDEROBJECT_RENDERCOMMAND_SETPIPELINESTATE
#define CROWN_RENDEROBJECT_RENDERCOMMAND_SETPIPELINESTATE
#include "RenderCommandBase.h"
#include <wrl.h>
namespace Crown
{
	namespace RenderObject
	{
		namespace RenderCommand
		{
			//====================================================
			// 作成日 : 2023/7/12
			// 作成者 : 服部颯澄
			// 
			// PSOをセットするよ☆
			// 
			//====================================================
			class SetPipelineState	:	public RenderCommandBase
			{
			public:
				SetPipelineState(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);
				virtual ~SetPipelineState();

				virtual void Run(ID3D12GraphicsCommandList* commandList) override;

			private:
				const Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
			};
		}
	}
}
#endif //!CROWN_RENDEROBJECT_RENDERCOMMAND_SETPIPELINESTATE