#pragma once
#ifndef CROWN_RENDEROBJECT_PMD
#define CROWN_RENDEROBJECT_PMD
#include <d3d12.h>
#include <string>
#include <vector>
#include "./../../TextureBuffer.h"
#include "./../Vertices.h"
#include "./../MaterialMesh.h"
#include "./../../DirectX12Wraps/RootSignature.h"
#include "./../../DirectX12Wraps/GraphicPipeline.h"
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/7/31
		// �쐬�� : �����D��
		// 
		// Pmd�`���̃��f���f�[�^���쐬����N���X���恙
		//
		//================================================
		class Pmd
		{
		public:
			//	���_�f�[�^�̒�`���恙
			struct VerticesData
			{
				DirectX::XMFLOAT3 pos;
				DirectX::XMFLOAT3 normal;
				DirectX::XMFLOAT2 uv;
				unsigned int boneIndex[2];
				float boneWeight;

				VerticesData() = default;
				VerticesData(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 uv, unsigned int boneIndex[2], float boneWeight)
					:
					pos(pos),
					normal(normal),
					uv(uv),
					boneWeight(boneWeight)
				{
					this->boneIndex[0] = boneIndex[0];
					this->boneIndex[1] = boneIndex[1];
				}
			};

			Pmd() = default;
			~Pmd() = default;
			static void Load(ID3D12Device* device, std::wstring& fileName, TextureBuffer& textureBuffer, Vertices& verticesBuffer, std::vector<MaterialMesh>& materialMeshs, unsigned int descriptorOffset, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
		private:
			static RootSignature* rootSignature;
			static GraphicsPipeline* graphicsPipeline;
			static const D3D12_INPUT_ELEMENT_DESC inputLayout[5];
		};
	}
}
#endif // !CROWN_RENDEROBJECT_PMD