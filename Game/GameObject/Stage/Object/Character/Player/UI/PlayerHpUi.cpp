#include "PlayerHpUi.h"
#include "./../../Crown/Object/RenderSystem/UI/UiManager.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"

PlayerHpUi::PlayerHpUi()
{
	m_constBuffer = new Crown::RenderObject::BlobConstBuffer(BUFFER_DATA, Crown::RenderObject::UiManager::GetInstance()->GetDevice());

	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::HP_PERCENT_OFFSET), 1.0f);
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::INCLINATION_OFFSET), 0.08f);
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::COLOR_OFFSET), DirectX::XMFLOAT3(0.6, 0.2, 0.2));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::FLAME_COLOR_OFFSET), DirectX::XMFLOAT3(0.2, 0.2,0.2));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::BACKGROUND_COLOR_OFFSET), DirectX::XMFLOAT3(0.4, 0.4,0.4));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::SIZE_OFFSET), DirectX::XMFLOAT2(0.8,0.05));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::POSITION_OFFSET), DirectX::XMFLOAT2(0.25, 1.8));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::ALPHA_OFFSET), 1.0f);

	Crown::RenderObject::GraphicsPipeline graphicsPipeline;
	graphicsPipeline.ChangeGraphicsPipelineState(Crown::RenderObject::UiManager::GetInstance()->GetDefaultGraphicsPipelineStateDesc());
	graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/PlayerHPUI_VS"));
	graphicsPipeline.SetGS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/PlayerHPUI_GS"));
	graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/PlayerHPUI_PS"));
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineState = graphicsPipeline.GetState();
	Crown::RenderObject::UiManager::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineState, IID_PPV_ARGS(&m_pipelineState));
}

PlayerHpUi::~PlayerHpUi()
{
	delete m_constBuffer;
}

void PlayerHpUi::SetAlpha(float alpha)
{
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::ALPHA_OFFSET), alpha);
}

void PlayerHpUi::SetPlayerHPPercent(float hpPercent)
{
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::HP_PERCENT_OFFSET), hpPercent);
}

void PlayerHpUi::Render(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetPipelineState(m_pipelineState.Get());
	commandList->SetGraphicsRootDescriptorTable(0, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_constBuffer->GetDescriptorOffset()));
}