#include "GraphicsCommandList.h"

Crown::RenderObject::GraphicsCommandList::GraphicsCommandList()
	:
	m_graphicsCommandList(nullptr),
	m_copyCommandList(nullptr),
	m_commandQueue(nullptr),
	m_waitEvent(nullptr),
	m_index(0),
	m_copyIndex(0),
	m_addCommandAllocatorNum(0),
	MAX_FAILEDPROCESSING_NUM(0),
	m_failedProcessingNum(0),
	m_device(nullptr)
{

}

Crown::RenderObject::GraphicsCommandList::~GraphicsCommandList()
{
	Finalize();
	CloseHandle(m_waitEvent);
}

void Crown::RenderObject::GraphicsCommandList::Initialize(ID3D12Device* device, unsigned int allocatorNum,  unsigned int failedProcessingNum)
{
	//	�o�b�t�@�[�����K��1�ȏ�ɂȂ�悤�ɂ���恙
	if (allocatorNum < 1)
	{
		allocatorNum = 1;
	}
	m_device = device;
	const_cast<unsigned int&>(MAX_FAILEDPROCESSING_NUM) = failedProcessingNum;
	m_waitEvent = CreateEvent(nullptr, false, false, nullptr);	//	�C�x���g���쐬����恙
	m_addCommandAllocatorNum = allocatorNum - 1;

	m_commandAllocators.emplace_back(new CommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT));
	device->CreateCommandList(1, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators.front()->GetCommandAllocator(), nullptr, IID_PPV_ARGS(&m_graphicsCommandList));	//	�R�}���h���X�g�̍쐬���恙
	m_graphicsCommandList->Close();

	m_copyAllocators.emplace_back(new CommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT));
	device->CreateCommandList(1, D3D12_COMMAND_LIST_TYPE_DIRECT, m_copyAllocators.front()->GetCommandAllocator(), nullptr, IID_PPV_ARGS(&m_copyCommandList));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;																						//	�^�C���A�E�g�Ȃ����恙
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;																			//	�D��x�͕��ʂ��w�肷��恙
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;																						//	�R�}���h���X�g�̃^�C�v�̓_�C���N�g�R�}���h���X�g���恙
	device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_commandQueue));																//	�R�}���h�L���[�������恙
}

void Crown::RenderObject::GraphicsCommandList::Finalize()
{
	Begin();
	End();
	WaitForGpu();
}

void Crown::RenderObject::GraphicsCommandList::Begin()
{
	NextCommandAllocator();
	m_commandAllocators[m_index]->Reset();
	m_graphicsCommandList->Reset(m_commandAllocators[m_index]->GetCommandAllocator(), nullptr);
}

void Crown::RenderObject::GraphicsCommandList::End()
{
	{
		m_copyCommandList->Close();																									//	�R�s�[�p�R�}���h���X�g���N���[�Y��
		ID3D12CommandList* cmdlists[] = { m_copyCommandList.Get() };
		m_commandQueue->ExecuteCommandLists(1, cmdlists);																			//	�R�}���h���X�g�̓��e�����s����恙
		m_copyAllocators[m_copyIndex]->CountUp();
		m_commandQueue->Signal(m_copyAllocators[m_copyIndex]->GetFence(), m_copyAllocators[m_copyIndex]->GetFenceValue());
		for (unsigned int i = 0, size = static_cast<unsigned int>(m_copyAllocators.size()); i <= size; ++i)
		{
			if (i == size)
			{
				m_copyAllocators.emplace_back(new CommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT));
			}
			if (!m_copyAllocators[i]->NotDrewEnd())
			{
				m_copyIndex = i;
				break;
			}
		}
		m_copyAllocators[m_copyIndex]->Reset();
		m_copyCommandList->Reset(m_copyAllocators[m_copyIndex]->GetCommandAllocator(), nullptr);
	}
	{
		m_graphicsCommandList->Close();																								//	�R�}���h���X�g���N���[�Y���ȍ~���̃R�}���h���X�g�͖��߂��󂯕t���Ȃ��恙
		ID3D12CommandList* cmdlists[] = { m_graphicsCommandList.Get() };
		m_commandQueue->ExecuteCommandLists(1, cmdlists);																			//	�R�}���h���X�g�̓��e�����s����恙
		m_commandAllocators[m_index]->CountUp();
		m_commandQueue->Signal(m_commandAllocators[m_index]->GetFence(), m_commandAllocators[m_index]->GetFenceValue());
	}
}

void Crown::RenderObject::GraphicsCommandList::WaitForGpu() noexcept
{
	for (std::unique_ptr<CommandAllocator>& allocator : m_commandAllocators)
	{
		allocator->WaitForGpu(m_waitEvent);
	}
}

void Crown::RenderObject::GraphicsCommandList::NextCommandAllocator()
{
	//	���Ɏ��s�g���R�}���h�A���P�[�^�[��T���恙
	for (int i = 0, size = static_cast<int>(m_commandAllocators.size()); i < size; ++i)
	{
		//	�R�}���h�A���P�[�^�[�̒����珈�����I����Ă�����̂�T���恙
		if (!m_commandAllocators[i]->NotDrewEnd())
		{
			//	�������I���Ă���̂ł��̃R�}���h�A���P�[�^�[�����Ɏg�p���邱�ƂƂ���恙
			m_index = i;
			m_failedProcessingNum = 0;
			return;
		}
	}

	//	�g����R�}���h�A���P�[�^�[�������̂ŏ������������������Ɣ��肷��恙
	++m_failedProcessingNum;

	//	���������������ꂽ�t���[������葽���������Ă���H
	if (MAX_FAILEDPROCESSING_NUM < m_failedProcessingNum)
	{
		if (m_addCommandAllocatorNum > 0)
		{
			--m_addCommandAllocatorNum;																			//	���݋����ꂽ�A���P�[�^�[�̐������炷�恙
			m_index = static_cast<int>(m_commandAllocators.size());												//	�ǉ������R�}���h�A���P�[�^�[�����Ɏg�p���邱�ƂƂ���恙
			m_commandAllocators.emplace_back(new CommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT));	//	�R�}���h�A���P�[�^�[��ǉ�����恙
			m_failedProcessingNum = 0;																			//	������������������恙
			return;
		}
	}

	//	GPU��ҋ@��
	++m_index;
	m_index %= m_commandAllocators.size();
	m_commandAllocators[m_index]->WaitForGpu(m_waitEvent);
}