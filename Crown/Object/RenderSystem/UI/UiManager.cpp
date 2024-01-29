#include "UiManager.h"
#include "./Ui.h"
#include <vector>
#include <algorithm>

std::unique_ptr<Crown::RenderObject::UiManager> Crown::RenderObject::UiManager::me;

Crown::RenderObject::UiManager* Crown::RenderObject::UiManager::GetInstance()
{
	if (!me)
	{
		me.reset(new UiManager);
	}
	return me.get();
}

Crown::RenderObject::UiManager::UiManager()
{
}

void Crown::RenderObject::UiManager::AddUi(Ui* newUi)
{
#ifdef _DEBUG
	for (Ui* ui : m_ui)
	{
		if (ui == newUi)
		{
			assert(0);					//	�o�^���悤�Ƃ��Ă���I�u�W�F�N�g�����łɂ������恙
		}
	}
#endif // _DEBUG

	m_ui.push_back(newUi);

	std::sort(m_ui.begin(), m_ui.end(), [](const Ui* lhs, const Ui* rhs) { return lhs->GetPriority() < rhs->GetPriority(); });
}

void Crown::RenderObject::UiManager::DeleteUi(Ui* newUi)
{
	for (std::vector<Ui*>::iterator i = m_ui.begin(); i < m_ui.end(); ++i)
	{
		if (*i == newUi)
		{
			m_ui.erase(i);
			return;
		}
	}
}

void Crown::RenderObject::UiManager::Initialize(ID3D12Device* device, TextureBuffer* textureBuffer)
{
	m_device = device;
	m_textureBuffer = textureBuffer;
	
	CreatVertices();			//	���_�f�[�^���쐬����恙
	CreatRootSignature(8);		//	���[�g�V�O�l�`���[���쐬����恙
	CreatDefaultGraphicsPipelineStateDesc();
}

void Crown::RenderObject::UiManager::Render(GraphicsCommandList& commandList)
{
	commandList.GetGraphicsCommandList()->SetGraphicsRootSignature(m_rootSignature.Get());
	commandList.GetGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	commandList.GetGraphicsCommandList()->IASetVertexBuffers(0, 1, m_vertices.GetVertexBufferView());
	for (Ui* ui : m_ui)
	{
		ui->Render(commandList.GetGraphicsCommandList());
		commandList.GetGraphicsCommandList()->DrawIndexedInstanced(1, 1, 0, 0, 0);
	}
}

void Crown::RenderObject::UiManager::CreatRootSignature(unsigned int texNum)
{
	//	�f�B�X�N���v�^�����W�̐ݒ肾�恙
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRanges;

	D3D12_DESCRIPTOR_RANGE descriptorRange = {};

	//	�萔�p���恙
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	descriptorRanges.push_back(descriptorRange);

	//	�e�N�X�`���悤���恙
	for (unsigned int i = 0; i < texNum; ++i)
	{
		descriptorRange.NumDescriptors = 1;
		descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange.BaseShaderRegister = i;
		descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		descriptorRanges.push_back(descriptorRange);
	}

	//	���[�g�p�����[�^�̍쐬���s���恙
	std::vector<D3D12_ROOT_PARAMETER> rootParameters;

	D3D12_ROOT_PARAMETER rootParameter = {};

	//	�萔�p���恙
	rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter.DescriptorTable.pDescriptorRanges = &descriptorRanges[0];
	rootParameter.DescriptorTable.NumDescriptorRanges = 1;
	rootParameters.push_back(rootParameter);

	//	�}�e���A���p���恙
	for (unsigned int i = 1; i < texNum + 1; ++i)
	{
		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameter.DescriptorTable.pDescriptorRanges = &descriptorRanges[i];
		rootParameter.DescriptorTable.NumDescriptorRanges = 1;
		rootParameters.push_back(rootParameter);
	}

	//	�T���v���[�̐ݒ肾�恙
	const unsigned int samplerNum = 1;
	D3D12_STATIC_SAMPLER_DESC samplerDesc[samplerNum] = {};
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].Filter = D3D12_FILTER_ANISOTROPIC;
	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc[0].MinLOD = 0.0f;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].ShaderRegister = 0;

	//	���[�g�V�O�l�`���[�̐ݒ肾�恙
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParameters.data();
	rootSignatureDesc.NumParameters = static_cast<UINT>(rootParameters.size());
	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = samplerNum;

	//	���[�g�V�O�l�`���[���쐬����恙
	ID3DBlob* rootSigBlob = nullptr;
	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, nullptr);
	m_device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf()));	//	���[�g�V�O�l�`���[���쐬����恙
	rootSigBlob->Release();
}

void Crown::RenderObject::UiManager::CreatVertices()
{
	struct Vertices
	{
		DirectX::XMFLOAT3 position;
	};
	std::vector<Vertices> v;
	v.push_back(Vertices(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)));
	std::vector<unsigned int> index{ 0};
	m_vertices.CreateBuffer(m_device, v, index);
}

void Crown::RenderObject::UiManager::CreatDefaultGraphicsPipelineStateDesc()
{
	Crown::RenderObject::GraphicsPipeline graphicsPipeline;

	graphicsPipeline.SetRootSignature(m_rootSignature.Get());

	//	Z�o�b�t�@�̐ݒ肾�恙
	D3D12_DEPTH_STENCIL_DESC desc = {};
	desc.DepthEnable = false;
	desc.StencilEnable = false;
	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	graphicsPipeline.SetDepthStencilState(desc);

	//	�C���v�b�g���C�A�E�g�̐ݒ肾�恙
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.pInputElementDescs = INPUTLAYOUT;
	inputLayoutDesc.NumElements = INPUTLAYOUT_NUM;
	graphicsPipeline.SetInputLayout(inputLayoutDesc);

	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

	graphicsPipeline.SetRasterizerState(rasterizerDesc);

	graphicsPipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

	m_defaultGraphicsPipelineStateDesc = graphicsPipeline.GetState();
}