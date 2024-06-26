#include "MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/RenderCommands/RenderCommandFactory.h"
#include "./../../Crown/Object/RenderSystem/DirectX12Wraps/DefaultRootSignature.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"

#include "./Render.h"

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
	graphicsPipeline.Commit(Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());

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

void MaterialFactory::CreateShadow(Crown::RenderObject::Model& model, unsigned int meshID)
{
	std::vector<Crown::RenderObject::BlobConstBuffer> materialData;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = model.GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW indexBufferView = model.GetIndexBufferView();

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
	resources.emplace_back(model.GetConstVertexBuffer());
	resources.emplace_back(model.GetConstIndexBuffer());
	resources.emplace_back(model.GetModelData());

	//	マテリアル描画の仕方を決定☆
	std::vector<std::shared_ptr<Crown::RenderObject::RenderCommand::RenderCommandBase>> renderCommands;
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetPipelineState(renderCommands, Render::GetShadowMapGraphicsPipeline()->GetPipelineState());
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetVertexBuffer(renderCommands, 0, 1, &vertexBufferView);
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetIndexBuffer(renderCommands, &indexBufferView);
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 0, Render::GetShadowMapBuffer()->GetDescriptorOffset());
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 1, model.GetDescriptorOffest());
	Crown::RenderObject::RenderCommand::RenderCommandQueue pmdRenderCommandQueue(renderCommands, resources);

	model.GetMaterialMesh(meshID).GetMaterial(Crown::RenderObject::MaterialTag::Shadow).CreateData(pmdRenderCommandQueue, materialData);
}

void MaterialFactory::CreateDefaultMaterial(Crown::RenderObject::Model& model, unsigned int meshID, unsigned int textureIndex, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT3 ambient)
{
	Crown::RenderObject::GraphicsPipeline graphicsPipeline;
	{
		D3D12_RASTERIZER_DESC rasterizer = graphicsPipeline.GetState().RasterizerState;
		rasterizer.CullMode = D3D12_CULL_MODE_NONE;
		graphicsPipeline.SetRasterizerState(rasterizer);
		graphicsPipeline.SetNumRenderTargets(2);
		graphicsPipeline.SetRTVFormats(0, DXGI_FORMAT_R8G8B8A8_UNORM);
		graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
		graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Default/FrameBufferVS"));
		graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Default/FrameBufferPS"));
		graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		graphicsPipeline.SetRootSignature(Crown::RenderObject::DefaultRootSignature::GetRootSignature().GetRootSignature().Get());
		graphicsPipeline.Commit(Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
	}

	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;				//	データ構造を指定する配列だよ☆
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float3);

	//	定数バッファの作成☆
	Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
	constBuffer.SetParameter(0, diffuse);
	constBuffer.SetParameter(1, ambient);

	//	定数バッファを指定☆
	std::vector<unsigned int> constBufferIndexs;
	constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
	constBufferIndexs.push_back(model.GetDescriptorOffest());
	constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

	//	テクスチャを指定☆
	std::vector<unsigned int> textureBufferIndexs;
	textureBufferIndexs.push_back(textureIndex);

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
	resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
	resources.emplace_back(model.GetModelData());
	resources.emplace_back(constBuffer.GetBuffer());
	resources.emplace_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().GetTextureBuffer(textureIndex));

	std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
	constBuffers.push_back(constBuffer);

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = model.GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW indexBufferView = model.GetIndexBufferView();

	//	マテリアル描画の仕方を決定☆
	std::vector<std::shared_ptr<Crown::RenderObject::RenderCommand::RenderCommandBase>> renderCommands;
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetPipelineState(renderCommands, graphicsPipeline.GetPipelineState());
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetVertexBuffer(renderCommands, 0, 1, &vertexBufferView);
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetIndexBuffer(renderCommands, &indexBufferView);
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 0, Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 1, model.GetDescriptorOffest());
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 2, constBuffer.GetDescriptorOffset());
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 3, Render::GetShadowMapBuffer()->GetDescriptorOffset());
	Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, Crown::RenderObject::RootSignature::CBV_NUM, textureIndex);
	//Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, Crown::RenderObject::RootSignature::CBV_NUM + 1, Render::GetShadowMapColorBufferIndex());
	//Crown::RenderObject::RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, Crown::RenderObject::RootSignature::CBV_NUM + 2, Render::GetShadowMapDepthBufferIndex());
	Crown::RenderObject::RenderCommand::RenderCommandQueue pmdRenderCommandQueue(renderCommands, resources);

	model.GetMaterialMesh(meshID).GetMaterial(Crown::RenderObject::MaterialTag::FrameBuffer).CreateData(pmdRenderCommandQueue, constBuffers);
}