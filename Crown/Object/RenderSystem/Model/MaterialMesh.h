#pragma once
#ifndef CROWN_RENDEROBJECT_MATERIALPARTS
#define CROWN_RENDEROBJECT_MATERIALPARTS
#include "MaterialTag.h"
#include "Material.h"
#include "./../DirectX12Wraps/DescriptorHeaps.h"
#include <vector>
#include <memory>
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// 作成日 : 2023/7/4
		// 作成者 : 服部颯澄
		// 
		// マテリアルを適用する頂点情報だよ☆
		// 
		//================================================
		class MaterialMesh
		{
		public:
			MaterialMesh(unsigned int indexNum, unsigned int m_indexOffset);
			~MaterialMesh();

			Material& GetMaterial(MaterialTag drawTag) { m_materialFlag[static_cast<int>(drawTag)] = true; return m_materials[static_cast<int>(drawTag)]; }

			inline void Draw(MaterialTag drawTag, ID3D12GraphicsCommandList* commandList, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& useResource) const
			{
				if (m_materialFlag[static_cast<int>(drawTag)])
				{
					m_materials[static_cast<int>(drawTag)].Draw(commandList, useResource);
					commandList->DrawIndexedInstanced(m_indexNum, 1, m_indexOffset, 0, 0);
				}
			}
		private:
			std::vector<Material> m_materials;
			std::vector<bool> m_materialFlag;
			unsigned int m_indexNum;
			unsigned int m_indexOffset;
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MATERIALPARTS