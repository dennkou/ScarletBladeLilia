#pragma once
#ifndef CROWN_RENDEROBJECT_RENDERCOMMAND_SETROOTSIGNATURE
#define CROWN_RENDEROBJECT_RENDERCOMMAND_SETROOTSIGNATURE
#include "RenderCommandBase.h"
#include <wrl.h>
namespace Crown
{
	namespace RenderObject
	{
		namespace RenderCommand
		{
			class SetRootSignature : public RenderCommandBase
			{
			public:
				SetRootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature);
				virtual ~SetRootSignature();

				virtual void Run(ID3D12GraphicsCommandList* commandList) override;
			private:
				Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
			};
		}
	}
}
#endif //!CROWN_RENDEROBJECT_RENDERCOMMAND_SETROOTSIGNATURE