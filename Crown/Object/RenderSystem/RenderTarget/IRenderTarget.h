#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "./../Model/MaterialTag.h"
#include "./../DirectX12Wraps/GraphicsCommandList.h"
#include "./../Model/ModelManager.h"
#include "./../DirectX12Wraps/DescriptorHeaps.h"

namespace Crown
{
	namespace RenderObject
	{
		class IRenderTarget
		{
		public:
			IRenderTarget() = default;
			virtual constexpr ~IRenderTarget() noexcept = default;

			virtual void Initialize(ID3D12Device* device) = 0;
			virtual void Draw(GraphicsCommandList& commandList, ModelManager* modelManager, unsigned int index) = 0;
		private:
		};
	}
}