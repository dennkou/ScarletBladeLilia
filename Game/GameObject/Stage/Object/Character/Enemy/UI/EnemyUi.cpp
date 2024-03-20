#include "EnemyUi.h"
#include "./../../Crown/Object/RenderSystem/UI/UiManager.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"

Microsoft::WRL::ComPtr<ID3D12PipelineState> EnemyUi::pipelineState;

EnemyUi::EnemyUi()
{
	m_constBuffer = new Crown::RenderObject::BlobConstBuffer(BUFFER_DATA, Crown::RenderObject::UiManager::GetInstance()->GetDevice());

	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::VIEW_PROJECTION_OFFSET), Crown::RenderObject::Camera::GetInstance()->GetView() * Crown::RenderObject::Camera::GetInstance()->GetProjection());
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::POSITION_OFFSET), DirectX::XMFLOAT3(0, 20, -100));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::HP_PERCENT_OFFSET), 1);
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::SIZE_OFFSET), DirectX::XMFLOAT2(0.07, 0.01));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::COLOR_OFFSET), DirectX::XMFLOAT3(1, 0.2, 0.2));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::FLAME_COLOR_OFFSET), DirectX::XMFLOAT3(0.2, 0.2, 0.2));
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::BACKGROUND_COLOR_OFFSET), DirectX::XMFLOAT3(0.4, 0.4, 0.4));

	//	レンタリングパイプラインの設定だよ☆
	Crown::RenderObject::GraphicsPipeline graphicsPipeline;
	graphicsPipeline.ChangeGraphicsPipelineState(Crown::RenderObject::UiManager::GetInstance()->GetDefaultGraphicsPipelineStateDesc());
	//	カリングの設定だよ☆
	{
		D3D12_RASTERIZER_DESC rasterizerDesc = graphicsPipeline.GetState().RasterizerState;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT;
		graphicsPipeline.SetRasterizerState(rasterizerDesc);
	}
	//	シェーダーの設定だよ☆
	{
		graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/EnemyHPUI_VS"));
		graphicsPipeline.SetGS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/EnemyHPUI_GS"));
		graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"UI/EnemyHPUI_PS"));
	}
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineState = graphicsPipeline.GetState();
	Crown::RenderObject::UiManager::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineState, IID_PPV_ARGS(&pipelineState));
}

EnemyUi::~EnemyUi()
{
	delete m_constBuffer;
}

void EnemyUi::Render(ID3D12GraphicsCommandList* commandList)
{
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::VIEW_PROJECTION_OFFSET), Crown::RenderObject::Camera::GetInstance()->GetView() * Crown::RenderObject::Camera::GetInstance()->GetProjection());
	commandList->SetPipelineState(pipelineState.Get());
	commandList->SetGraphicsRootDescriptorTable(0, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_constBuffer->GetDescriptorOffset()));
}

void EnemyUi::SetPositionOffset(DirectX::XMFLOAT3 positionOffset)
{
	m_positionOffset = positionOffset;
}

void EnemyUi::SetHPPercent(float hpPercent)
{
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::HP_PERCENT_OFFSET), hpPercent);
}

void EnemyUi::SetPosition(DirectX::XMFLOAT3 position)
{
	position.x += m_positionOffset.x;
	position.y += m_positionOffset.y;
	position.z += m_positionOffset.z;
	m_constBuffer->SetParameter(static_cast<unsigned int>(ConstBufferOffset::POSITION_OFFSET), position);
}