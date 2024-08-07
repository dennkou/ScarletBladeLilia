#include "UiGraphic.h"
#include "./../../../../System.h"
#include "./../UiManager.h"
#include "./../../Shader.h"
#include "./../../TextureBuffer.h"

Microsoft::WRL::ComPtr<ID3D12PipelineState> Crown::RenderObject::UiGraphic::pipelineState = nullptr;

Crown::RenderObject::UiGraphic::UiGraphic()
	:
	m_descriptorOffset(0),
	m_textureResource(),
	m_pos(0,0),
	m_size(1,1)
{
	CreatConstBuffer();
	SetAlpha(0.0f);
}

Crown::RenderObject::UiGraphic::UiGraphic(std::wstring texturePath)
	:
	UiGraphic()
{
	Load(texturePath);
}

Crown::RenderObject::UiGraphic::~UiGraphic()
{
	delete m_constBuffer;
}

void Crown::RenderObject::UiGraphic::Load(std::wstring texturePath)
{
	m_descriptorOffset = UiManager::GetInstance()->GetTextureBuffer()->TextureAcquisition(texturePath);
	m_textureResource = UiManager::GetInstance()->GetTextureBuffer()->GetTextureBuffer(m_descriptorOffset);
	//	パイプラインステートが無ければ生成するよ☆
	if (!pipelineState)
	{
		CreatPipelineState();
	}
	SetAlpha(1.0f);
}

void Crown::RenderObject::UiGraphic::Render(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetPipelineState(pipelineState.Get());
	commandList->SetGraphicsRootDescriptorTable(0, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_constBuffer->GetDescriptorOffset()));
	commandList->SetGraphicsRootDescriptorTable(2, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_descriptorOffset));
}

void Crown::RenderObject::UiGraphic::CreatPipelineState()
{
	GraphicsPipeline graphicsPipeline;
	graphicsPipeline.ChangeGraphicsPipelineState(UiManager::GetInstance()->GetDefaultGraphicsPipelineStateDesc());
	graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/UIvs"));
	graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/UIPs"));
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineState = graphicsPipeline.GetState();
	UiManager::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineState, IID_PPV_ARGS(&pipelineState));
}

void Crown::RenderObject::UiGraphic::CreatConstBuffer()
{
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;

	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Matrix);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float3);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float);

	m_constBuffer = new Crown::RenderObject::BlobConstBuffer(bufferData, UiManager::GetInstance()->GetDevice());

	ConstBufferUpdate();
}

void Crown::RenderObject::UiGraphic::ConstBufferUpdate()
{
	m_constBuffer->SetParameter(MATRIX_OFFSET, DirectX::XMMatrixScaling(m_size.x, m_size.y, 0) * DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, 0));
	//m_constBuffer->SetParameter(MATRIX_OFFSET, DirectX::XMMatrixIdentity());
	m_constBuffer->SetParameter(COLOR_OFFSET, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_constBuffer->SetParameter(ALPHA_OFFSET, 1.0f);
}