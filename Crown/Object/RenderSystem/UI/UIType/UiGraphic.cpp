#include "UiGraphic.h"
#include "./../../../../System.h"
#include "./../UiManager.h"
#include "./../../Shader.h"
#include "./../../TextureBuffer.h"

Microsoft::WRL::ComPtr<ID3D12PipelineState> Crown::RenderObject::UiGraphic::pipelineState = nullptr;

Crown::RenderObject::UiGraphic::UiGraphic()
	:
	m_descriptorOffset(0),
	m_textureResource()
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
	commandList->SetGraphicsRootDescriptorTable(1, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_descriptorOffset));
}

void Crown::RenderObject::UiGraphic::CreatPipelineState()
{
	GraphicsPipeline graphicsPipeline;
	graphicsPipeline.ChangeGraphicsPipelineState(UiManager::GetInstance()->GetDefaultGraphicsPipelineStateDesc());
	graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/UIvs"));
	graphicsPipeline.SetGS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/UIGs"));
	graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/UIPs"));
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineState = graphicsPipeline.GetState();
	UiManager::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineState, IID_PPV_ARGS(&pipelineState));
}

void Crown::RenderObject::UiGraphic::CreatConstBuffer()
{
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;

	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float2);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float2);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float3);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float);

	m_constBuffer = new Crown::RenderObject::BlobConstBuffer(bufferData, UiManager::GetInstance()->GetDevice());

	m_constBuffer->SetParameter(POSITION_OFFSET, DirectX::XMFLOAT2(0,0));
	m_constBuffer->SetParameter(SIZE_OFFSET, DirectX::XMFLOAT2(1,1));
	m_constBuffer->SetParameter(ROTATE_OFFSET, 0.0f);
	m_constBuffer->SetParameter(STRIP_OFFSET, DirectX::XMFLOAT4(0.0f,1.0f,0.0f,1.0f));
	m_constBuffer->SetParameter(COLOR_OFFSET, DirectX::XMFLOAT3(1.0f,1.0f,1.0f));
	m_constBuffer->SetParameter(ALPHA_OFFSET, 1.0f);
}