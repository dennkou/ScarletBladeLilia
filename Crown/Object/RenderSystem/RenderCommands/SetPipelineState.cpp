#include "SetPipelineState.h"

Crown::RenderObject::RenderCommand::SetPipelineState::SetPipelineState(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState)
	:
	m_pipelineState(pipelineState)
{
}

Crown::RenderObject::RenderCommand::SetPipelineState::~SetPipelineState()
{
}

void Crown::RenderObject::RenderCommand::SetPipelineState::Run(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetPipelineState(m_pipelineState.Get());
}