#include "FrameRender.h"
#include "./../../Crown/Object/RenderSystem/UI/UiManager.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"

FrameRender::FrameRender()
{
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> constBuffer;
	constBuffer.push_back(Crown::RenderObject::BlobConstBuffer::DataType::Matrix);
	constBuffer.push_back(Crown::RenderObject::BlobConstBuffer::DataType::Float3);
	m_constBuffer = new Crown::RenderObject::BlobConstBuffer(constBuffer, Crown::RenderObject::UiManager::GetInstance()->GetDevice());


	m_graphicsPipeline.ChangeGraphicsPipelineState(Crown::RenderObject::UiManager::GetInstance()->GetDefaultGraphicsPipelineStateDesc());
	m_graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Default/RenderingVS"));
	m_graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Default/RenderingPS"));
	m_graphicsPipeline.Commit(Crown::RenderObject::UiManager::GetInstance()->GetDevice());
}

FrameRender::~FrameRender()
{
	delete m_constBuffer;
}

void FrameRender::Render(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetPipelineState(m_graphicsPipeline.GetPipelineState().Get());
	commandList->SetGraphicsRootDescriptorTable(0, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_constBuffer->GetDescriptorOffset()));
	commandList->SetGraphicsRootDescriptorTable(1, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_camera));
	commandList->SetGraphicsRootDescriptorTable(2, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_colorBuffer));
	commandList->SetGraphicsRootDescriptorTable(3, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_normalBuffer));
	commandList->SetGraphicsRootDescriptorTable(4, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_depthBuffer));
	commandList->SetGraphicsRootDescriptorTable(5, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_shadowBuffer));
}
