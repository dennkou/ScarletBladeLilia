#include "MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../../Crown/Object/RenderSystem/RenderCommands/RenderCommandFactory.h"
#include "./../../../Crown/Object/RenderSystem/DirectX12Wraps/DefaultRootSignature.h"

MaterialFactory::MaterialFactory()
{
}

MaterialFactory::~MaterialFactory()
{
}

void MaterialFactory::CreateMaterial(Crown::RenderObject::GraphicsPipeline& graphicsPipeline, Crown::RenderObject::Model& model, unsigned int meshID, Crown::RenderObject::MaterialTag materialTag, std::vector<unsigned int>& constOffsets, std::vector<unsigned int>& textureOffsets, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& resourceList, std::vector<Crown::RenderObject::BlobConstBuffer>& materialData)
{
	//	パイプラインステートの生成☆
	graphicsPipeline.SetRootSignature(Crown::RenderObject::DefaultRootSignature::GetRootSignature().GetRootSignature().Get());
	graphicsPipeline.Commit(*Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = model.GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW indexBufferView = model.GetIndexBufferView();

	//	マテリアル描画の仕方を決定☆
	std::vector<std::shared_ptr<Crown::RenderObject::RenderCommand::RenderCommandBase>> renderCommands;
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetPipelineState(renderCommands, graphicsPipeline.GetPipelineState());
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetVertexBuffer(renderCommands, 0, 1, &vertexBufferView);
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetIndexBuffer(renderCommands, &indexBufferView);

	for (int i = 0, size = static_cast<int>(constOffsets.size()); i < size; ++i)
	{
		Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, i, constOffsets[i]);
	}
	for (int i = 0, size = static_cast<int>(textureOffsets.size()); i < size; ++i)
	{
		Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, Crown::RenderObject::RootSignature::CBV_NUM + i, textureOffsets[i]);
	}
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
	resources.emplace_back(model.GetConstVertexBuffer());
	resources.emplace_back(model.GetConstIndexBuffer());
	for (Microsoft::WRL::ComPtr<ID3D12Resource> resource : resourceList)
	{
		resources.emplace_back(resource);
	}
	Crown::RenderObject::RenderCommand::RenderCommandQueue pmdRenderCommandQueue(renderCommands, resources);
	
	model.GetMaterialMesh(meshID).GetMaterial(materialTag).CreateData(pmdRenderCommandQueue, materialData);
}