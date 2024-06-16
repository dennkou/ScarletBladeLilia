#pragma once
#include "MaterialTag.h"
#include "./../DirectX12Wraps/GraphicsCommandList.h"
#include "./../TextureBuffer.h"

namespace Crown
{
	namespace RenderObject
	{
		class IModel
		{
		public:
			class IModelLoader
			{
			public:
				virtual void Load(ID3D12Device* device, TextureBuffer* textureBuffer) = 0;
			};
			IModel() = default;
			~IModel() = default;

			virtual void DataUpload() = 0;

			virtual void Draw(MaterialTag drawTag, ID3D12GraphicsCommandList* commandList) = 0;
		private:

		};
	}
}