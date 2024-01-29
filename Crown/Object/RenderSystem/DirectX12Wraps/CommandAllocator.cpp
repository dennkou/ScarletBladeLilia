#include "CommandAllocator.h"

Crown::RenderObject::CommandAllocator::CommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
{
	m_fenceValue = 0;

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	device->CreateCommandAllocator(type, IID_PPV_ARGS(&m_commandAllocator));							//	�R�}���h�A���P�[�^�[�̍쐬���恙
}

Crown::RenderObject::CommandAllocator::~CommandAllocator()
{
	HANDLE waitEvent = CreateEvent(nullptr, false, false, nullptr);	//	�C�x���g���쐬����恙
	if (waitEvent != 0)
	{
		WaitForGpu(waitEvent);
		CloseHandle(waitEvent);
	}
	m_commandAllocator.Reset();
}

bool Crown::RenderObject::CommandAllocator::NotDrewEnd()
{
	return m_fence->GetCompletedValue() != m_fenceValue;
}

void Crown::RenderObject::CommandAllocator::WaitForGpu(HANDLE waitEvent) noexcept
{
	if (NotDrewEnd())
	{
		m_fence->SetEventOnCompletion(m_fenceValue, waitEvent);
		WaitForSingleObject(waitEvent, INFINITE);
	}
}

void Crown::RenderObject::CommandAllocator::Reset()
{
	m_commandAllocator->Reset();	
	m_resourceHolder.clear();		//	�ێ����Ă����f�[�^������\�ɂ���恙
}