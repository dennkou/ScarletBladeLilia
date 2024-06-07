#pragma once
#ifndef CROWN_RENDEROBJECT_PMX
#define CROWN_RENDEROBJECT_PMX
#include <d3d12.h>
#include <string>
#include <vector>
#include <memory>
#include "./../../TextureBuffer.h"
#include "./../Vertices.h"
#include "./../MaterialMesh.h"
#include "./../Bone.h"

namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// 作成日 : 2023/7/31
		// 作成者 : 服部颯澄
		// 
		// Pmx形式のモデルデータを作成するクラスだよ☆
		//
		//================================================
		class Pmx
		{
		public:
			Pmx() = default;
			~Pmx() = default;
			static void Load(ID3D12Device* device, std::wstring& fileName, TextureBuffer& textureBuffer, Vertices& verticesBuffer, std::vector<MaterialMesh>& materialMeshs, BoneData& boneData, unsigned int descriptorOffset, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
			static const D3D12_INPUT_LAYOUT_DESC GetInputLayout() { return inputLayoutDesc; }
			static unsigned int GetInputElementNum() noexcept { return InputElementNum; }
		private:
			enum class LoadModeIndexs
			{
				encodeType,
				addUvNum,
				vertexIndexSize,
				textureIndexSize,
				materialIndexSize,
				boneIndexSize,
				morphIndexSize,
				rigidIndexSize
			};

			//	頂点データの定義だよ☆
			struct VerticesData
			{
				DirectX::XMFLOAT3 pos;
				DirectX::XMFLOAT3 normal;
				DirectX::XMFLOAT2 uv;
				//DirectX::XMFLOAT4 addUv[4];
				//char weightType;
				unsigned int boneIndex[4];
				float boneWeight[4];
				//DirectX::XMFLOAT3			sdef_C;			//	SDEF-C
				//DirectX::XMFLOAT3			sdef_R0;		//	SDEF-R0
				//DirectX::XMFLOAT3			sdef_R1;		//	SDEF-R1
				float									toonEdgeScale;	//	トゥーンエッジのスケール

				VerticesData() = default;
			};

			//static constexpr unsigned int InputElementNum = 13;
			static constexpr unsigned int InputElementNum = 5;
			static std::unique_ptr<GraphicsPipeline> graphicsPipeline;
			static const D3D12_INPUT_ELEMENT_DESC inputLayout[InputElementNum];

			static D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
		};
	}
}
#endif // !CROWN_RENDEROBJECT_PMX