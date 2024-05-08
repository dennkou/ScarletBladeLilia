#pragma once
#include <d3d12.h>
#include <wrl.h>

#include "./../../Crown/Object/RenderSystem/DirectX12Wraps/GraphicPipeline.h"
#include "./../../Crown/Object/RenderSystem/Model/Model.h"

class MaterialFactory
{
public:
	MaterialFactory();
	~MaterialFactory();

	static void CreateMaterial(Crown::RenderObject::GraphicsPipeline& graphicsPipeline, Crown::RenderObject::Model& model, unsigned int meshID, Crown::RenderObject::MaterialTag materialTag, std::vector<unsigned int>& constOffsets, std::vector<unsigned int>& textureOffsets, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resourceList, std::vector<Crown::RenderObject::BlobConstBuffer>& materialData);
private:


};