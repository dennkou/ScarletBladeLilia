#include "TriangleColliderDebug.h"
#include "./../../RenderCommands/RenderCommandFactory.h"
#include "./../../Shader.h"
#include "./../../Camera.h"
#include "./../../DirectX12Wraps/DefaultRootSignature.h"

std::unique_ptr<Crown::RenderObject::GraphicsPipeline> Crown::RenderObject::TriangleColliderDebug::graphicsPipeline = nullptr;
D3D12_INPUT_LAYOUT_DESC Crown::RenderObject::TriangleColliderDebug::inputLayoutDesc;
const D3D12_INPUT_ELEMENT_DESC Crown::RenderObject::TriangleColliderDebug::inputLayout[1] =
{
	{ "POSITION",	0,DXGI_FORMAT_R32G32B32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
};

void Crown::RenderObject::TriangleColliderDebug::Load(ID3D12Device* device, Vertices& verticesBuffer, std::vector<MaterialMesh>& materialMeshs, const std::initializer_list<ColliderAlgorithm::Triangle>& collider, DirectX::XMFLOAT4 color, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	if (!graphicsPipeline)
	{
		graphicsPipeline.reset(new GraphicsPipeline());
		D3D12_RASTERIZER_DESC rasterizerDesc;
		rasterizerDesc = graphicsPipeline->GetState().RasterizerState;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		graphicsPipeline->SetRasterizerState(rasterizerDesc);
		graphicsPipeline->SetVS(*Shader::GetInstance()->GetShader(L"Debug/DebugVS"));
		graphicsPipeline->SetPS(*Shader::GetInstance()->GetShader(L"Debug/DebugPS"));
		graphicsPipeline->SetRootSignature(DefaultRootSignature::GetRootSignature().GetRootSignature().Get());
		inputLayoutDesc.pInputElementDescs = inputLayout;
		inputLayoutDesc.NumElements = 1;
		graphicsPipeline->SetInputLayout(inputLayoutDesc);
		graphicsPipeline->Commit(device);
	}

	//	頂点バッファの作成だよ☆
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<unsigned int> indexes;
	unsigned int indexNum = 0;
	for (const ColliderAlgorithm::Triangle& triangle : collider)
	{
		for (int i = 0; i < 3; ++i)
		{
			vertices.push_back(triangle.point[i].point);
			indexes.push_back(indexNum);
			++indexNum;
		}
	}
	verticesBuffer.CreateBuffer(device, vertices, indexes);

	//	定数バッファの作成☆
	std::vector<BlobConstBuffer::DataType> bufferData;			//	データ構造を指定する配列だよ☆
	bufferData.emplace_back(BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(BlobConstBuffer::DataType::Matrix);
	bufferData.emplace_back(BlobConstBuffer::DataType::Matrix);
	BlobConstBuffer constBuffer(bufferData, device);
	constBuffer.SetParameter(0, color);
	constBuffer.SetParameter(1, DirectX::XMMatrixIdentity());
	constBuffer.SetParameter(2, Crown::RenderObject::Camera::GetInstance()->GetView() * Crown::RenderObject::Camera::GetInstance()->GetProjection());

	//	マテリアル描画の仕方を決定☆
	std::vector<std::shared_ptr<RenderCommand::RenderCommandBase>> renderCommands;
	RenderCommand::RenderCommandFactory::CreateSetPipelineState(renderCommands, graphicsPipeline->GetPipelineState());
	RenderCommand::RenderCommandFactory::CreateSetVertexBuffer(renderCommands, 0, 1, verticesBuffer.GetVertexBufferView());
	RenderCommand::RenderCommandFactory::CreateSetIndexBuffer(renderCommands, verticesBuffer.GetIndexBufferView());
	RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 0, constBuffer.GetDescriptorOffset());
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
	resources.emplace_back(verticesBuffer.GetConstVertexBuffer());
	resources.emplace_back(verticesBuffer.GetConstIndexBuffer());
	resources.emplace_back(resource);
	resources.emplace_back(constBuffer.GetBuffer());
	RenderCommand::RenderCommandQueue pmdRenderCommandQueue(renderCommands, resources);

	//	定数配列を作成☆
	std::vector<BlobConstBuffer> constbuffers;
	constbuffers.push_back(constBuffer);

	//	マテリアルを適用するメッシュデータの作成を行うよ☆
	materialMeshs.emplace_back(static_cast<unsigned int>(indexes.size()), 0);
	materialMeshs.back().GetMaterial(MaterialTag::Normal).CreateData(pmdRenderCommandQueue, constbuffers);
}