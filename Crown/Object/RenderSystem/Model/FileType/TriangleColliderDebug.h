#pragma once
#include <d3d12.h>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include "./../../TextureBuffer.h"
#include "./../Vertices.h"
#include "./../MaterialMesh.h"
#include "./../Bone.h"

#include "./../../Game/GameObject/Stage/Collider/ColliderAlgorithm.h"

namespace Crown
{
	namespace RenderObject
	{
		class TriangleColliderDebug
		{
		public:
			TriangleColliderDebug() = default;
			~TriangleColliderDebug() = default;

			static void Load(ID3D12Device* device, Vertices& verticesBuffer, std::vector<MaterialMesh>& materialMeshs, const std::initializer_list<ColliderAlgorithm::Triangle>& collider, DirectX::XMFLOAT4 color, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
			static const D3D12_INPUT_LAYOUT_DESC GetInputLayout() { return inputLayoutDesc; }
		private:
			static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
			static std::unique_ptr<GraphicsPipeline> graphicsPipeline;
			static const D3D12_INPUT_ELEMENT_DESC inputLayout[1];

			static D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
		};
	}
}