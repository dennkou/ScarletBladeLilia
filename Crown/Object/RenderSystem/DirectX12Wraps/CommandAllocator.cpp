#include "CommandAllocator.h"

Crown::RenderObject::CommandAllocator::CommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
{
	m_fenceValue = 0;

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	device->CreateCommandAllocator(type, IID_PPV_ARGS(&m_commandAllocator));							//	コマンドアロケーターの作成だよ☆
}

Crown::RenderObject::CommandAllocator::~CommandAllocator()
{
	WaitForGpu();
	m_commandAllocator.Reset();
}

bool Crown::RenderObject::CommandAllocator::NotDrewEnd()
{
	return m_fence->GetCompletedValue() != m_fenceValue;
}

void Crown::RenderObject::CommandAllocator::WaitForGpu() noexcept
{
	if (NotDrewEnd())
	{
		m_fence->SetEventOnCompletion(m_fenceValue, nullptr);
	}
}

void Crown::RenderObject::CommandAllocator::Reset()
{
	m_commandAllocator->Reset();	
	m_resourceHolder.clear();		//	保持していたデータを解放可能にするよ☆
}