#include "TriangleColliderDebug.h"
#include "./../../RenderCommands/RenderCommandFactory.h"
#include "./../../Shader.h"
#include "./../../Camera.h"

Microsoft::WRL::ComPtr<ID3D12RootSignature> Crown::RenderObject::TriangleColliderDebug::rootSignature = nullptr;
std::unique_ptr<Crown::RenderObject::GraphicsPipeline> Crown::RenderObject::TriangleColliderDebug::graphicsPipeline = nullptr;
D3D12_INPUT_LAYOUT_DESC Crown::RenderObject::TriangleColliderDebug::inputLayoutDesc;
const D3D12_INPUT_ELEMENT_DESC Crown::RenderObject::TriangleColliderDebug::inputLayout[1] =
{
	{ "POSITION",		0,DXGI_FORMAT_R32G32B32_FLOAT,		0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
};

void Crown::RenderObject::TriangleColliderDebug::Load(ID3D12Device* device, Vertices& verticesBuffer, std::vector<MaterialMesh>& materialMeshs, const std::initializer_list<ColliderAlgorithm::Triangle>& collider, DirectX::XMFLOAT4 color, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	if (rootSignature.Get() == nullptr || !graphicsPipeline)
	{
		//	�f�B�X�N���v�^�����W�̐ݒ肾�恙
		D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};
		descriptorRange[0].NumDescriptors = 1;
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		descriptorRange[0].BaseShaderRegister = 0;
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		descriptorRange[1].NumDescriptors = 1;
		descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		descriptorRange[1].BaseShaderRegister = 1;
		descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//	���[�g�p�����[�^�̍쐬���s���恙
		static const unsigned int parameterNum = 2;
		D3D12_ROOT_PARAMETER rootParameter[parameterNum] = {};
		rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameter[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];
		rootParameter[0].DescriptorTable.NumDescriptorRanges = 1;

		rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameter[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1];
		rootParameter[1].DescriptorTable.NumDescriptorRanges = 1;

		//	���[�g�V�O�l�`���[�̐ݒ肾�恙
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootParameter;
		rootSignatureDesc.NumParameters = parameterNum;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.NumStaticSamplers = 0;

		//	���[�g�V�O�l�`���[���쐬����恙
		ID3DBlob* rootSigBlob = nullptr;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, nullptr);
		device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));	//	���[�g�V�O�l�`���[���쐬����恙
		rootSigBlob->Release();



		graphicsPipeline.reset(new GraphicsPipeline());
		D3D12_RASTERIZER_DESC rasterizerDesc;
		rasterizerDesc = graphicsPipeline->GetState().RasterizerState;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		graphicsPipeline->SetRasterizerState(rasterizerDesc);
		graphicsPipeline->SetVS(*Shader::GetInstance()->GetShader(L"Debug/DebugVS"));
		graphicsPipeline->SetPS(*Shader::GetInstance()->GetShader(L"Debug/DebugPS"));
		graphicsPipeline->SetRootSignature(rootSignature.Get());
		inputLayoutDesc.pInputElementDescs = inputLayout;
		inputLayoutDesc.NumElements = 1;
		graphicsPipeline->SetInputLayout(inputLayoutDesc);
		graphicsPipeline->Commit(*device);
	}

	//	���_�o�b�t�@�̍쐬���恙
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

	//	�萔�o�b�t�@�̍쐬��
	std::vector<BlobConstBuffer::DataType> bufferData;			//	�f�[�^�\�����w�肷��z�񂾂恙
	bufferData.emplace_back(BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(BlobConstBuffer::DataType::Matrix);
	BlobConstBuffer constBuffer(bufferData, device);
	constBuffer.SetParameter(0, color);
	constBuffer.SetParameter(1, DirectX::XMMatrixIdentity());

	//	�}�e���A���`��̎d�������聙
	std::vector<std::shared_ptr<RenderCommand::RenderCommandBase>> renderCommands;
	RenderCommand::RenderCommandFactory::CreateSetRootSignature(renderCommands, rootSignature.Get());
	RenderCommand::RenderCommandFactory::CreateSetPipelineState(renderCommands, graphicsPipeline->GetPipelineState());
	RenderCommand::RenderCommandFactory::CreateSetPrimitiveTopology(renderCommands, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	RenderCommand::RenderCommandFactory::CreateSetVertexBuffer(renderCommands, 0, 1, verticesBuffer.GetVertexBufferView());
	RenderCommand::RenderCommandFactory::CreateSetIndexBuffer(renderCommands, verticesBuffer.GetIndexBufferView());
	RenderCommand::RenderCommandFactory::CreateSetDescriptorHeap(renderCommands);
	RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 0, constBuffer.GetDescriptorOffset());
	RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 1, Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
	resources.emplace_back(verticesBuffer.GetConstVertexBuffer());
	resources.emplace_back(verticesBuffer.GetConstIndexBuffer());
	resources.emplace_back(resource);
	resources.emplace_back(constBuffer.GetBuffer());
	RenderCommand::RenderCommandQueue pmdRenderCommandQueue(renderCommands, resources);

	//	�萔�z����쐬��
	std::vector<BlobConstBuffer> constbuffers;
	constbuffers.push_back(constBuffer);

	//	�}�e���A����K�p���郁�b�V���f�[�^�̍쐬���s���恙
	materialMeshs.emplace_back(indexes.size(), 0);
	materialMeshs.back().GetMaterial(MaterialTag::Normal).CreateData(pmdRenderCommandQueue, constbuffers);
}