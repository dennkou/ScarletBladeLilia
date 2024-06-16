#include "GraphicsCommandList.h"

Crown::RenderObject::GraphicsCommandList::GraphicsCommandList()
	:
	m_graphicsCommandList(),
	m_copyCommandList(nullptr),
	m_commandQueue(nullptr),
	m_copyIndex(0),
	m_addCommandAllocatorNum(0),
	MAX_FAILEDPROCESSING_NUM(0),
	m_failedProcessingNum(0),
	m_device(nullptr)
{

}

Crown::RenderObject::GraphicsCommandList::~GraphicsCommandList()
{
	unsigned int commandListNum = static_cast<unsigned int>(m_graphicsCommandList.size());
	std::vector<ID3D12CommandList*> commandList(commandListNum);
	for (unsigned int i = 0; i < commandListNum; ++i)
	{
		m_graphicsCommandList[i].commandList->Close();																	//	�R�}���h���X�g���N���[�Y���ȍ~���̃R�}���h���X�g�͖��߂��󂯕t���Ȃ��恙
		commandList[i] = m_graphicsCommandList[i].commandList.Get();
	}

	m_commandQueue->ExecuteCommandLists(commandListNum, commandList.data());	//	�R�}���h���X�g�̓��e�����s����恙
	++m_fenceVal;
	m_commandQueue->Signal(m_fence.Get(), m_fenceVal);

	for (unsigned int i = 0; i < commandListNum; ++i)
	{
		m_commandQueue->Signal(m_commandAllocators[m_graphicsCommandList[i].allocatorIndex]->GetFence(), m_commandAllocators[m_graphicsCommandList[i].allocatorIndex]->GetFenceValue());
	}
	WaitForGpu();
}

ID3D12GraphicsCommandList* Crown::RenderObject::GraphicsCommandList::GetGraphicsCommandList(unsigned int index) noexcept
{
	if (m_graphicsCommandList.size() == index)
	{
		CommandList commandList;
		NextCommandAllocator(commandList.allocatorIndex);
		m_device->CreateCommandList(1, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[commandList.allocatorIndex]->GetCommandAllocator(), nullptr, IID_PPV_ARGS(&commandList.commandList));
		m_commandAllocators[commandList.allocatorIndex]->CountUp();
		m_graphicsCommandList.push_back(commandList);
	}
	return m_graphicsCommandList[index].commandList.Get();
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
	m_addCommandAllocatorNum = allocatorNum - 1;

	CommandList commandList;
	m_graphicsCommandList.push_back(commandList);
	m_commandAllocators.emplace_back(new CommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT));
	device->CreateCommandList(1, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators.back()->GetCommandAllocator(), nullptr, IID_PPV_ARGS(&m_graphicsCommandList.back().commandList));
	m_commandAllocators.back()->CountUp();

	m_copyAllocators.emplace_back(new CommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT));
	device->CreateCommandList(1, D3D12_COMMAND_LIST_TYPE_DIRECT, m_copyAllocators.back()->GetCommandAllocator(), nullptr, IID_PPV_ARGS(&m_copyCommandList));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;																						//	�^�C���A�E�g�Ȃ����恙
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;																				//	�D��x�͍����恙
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;																						//	�R�}���h���X�g�̃^�C�v�̓_�C���N�g�R�}���h���X�g���恙
	device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_commandQueue));																//	�R�}���h�L���[�������恙

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()));
	m_fenceVal = 0;
}

void Crown::RenderObject::GraphicsCommandList::WaitForGpu() noexcept
{
	m_fence->SetEventOnCompletion(m_fenceVal, nullptr);
}

void Crown::RenderObject::GraphicsCommandList::CopyExecute()
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

void Crown::RenderObject::GraphicsCommandList::DrawExecute()
{
	unsigned int commandListNum = static_cast<unsigned int>(m_graphicsCommandList.size());
	std::vector<ID3D12CommandList*> commandList(commandListNum);
	for (unsigned int i = 0; i < commandListNum; ++i)
	{
		m_graphicsCommandList[i].commandList->Close();																	//	�R�}���h���X�g���N���[�Y���ȍ~���̃R�}���h���X�g�͖��߂��󂯕t���Ȃ��恙
		commandList[i] = m_graphicsCommandList[i].commandList.Get();
	}

	m_commandQueue->ExecuteCommandLists(commandListNum, commandList.data());	//	�R�}���h���X�g�̓��e�����s����恙
	++m_fenceVal;
	m_commandQueue->Signal(m_fence.Get(), m_fenceVal);

	for (unsigned int i = 0; i < commandListNum; ++i)
	{
		m_commandQueue->Signal(m_commandAllocators[m_graphicsCommandList[i].allocatorIndex]->GetFence(), m_commandAllocators[m_graphicsCommandList[i].allocatorIndex]->GetFenceValue());
		NextCommandAllocator(m_graphicsCommandList[i].allocatorIndex);
		m_commandAllocators[m_graphicsCommandList[i].allocatorIndex]->Reset();
		m_graphicsCommandList[i].commandList->Reset(m_commandAllocators[m_graphicsCommandList[i].allocatorIndex]->GetCommandAllocator(), nullptr);
		m_commandAllocators[m_graphicsCommandList[i].allocatorIndex]->CountUp();
	}
}

void Crown::RenderObject::GraphicsCommandList::NextCommandAllocator(unsigned int& index)
{
	//	���Ɏ��s�g���R�}���h�A���P�[�^�[��T���恙
	for (int i = 0, size = static_cast<int>(m_commandAllocators.size()); i < size; ++i)
	{
		//	�R�}���h�A���P�[�^�[�̒����珈�����I����Ă�����̂�T���恙
		if (!m_commandAllocators[i]->NotDrewEnd())
		{
			//	�������I���Ă���̂ł��̃R�}���h�A���P�[�^�[�����Ɏg�p���邱�ƂƂ���恙
			index = i;
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
			index = static_cast<int>(m_commandAllocators.size());												//	�ǉ������R�}���h�A���P�[�^�[�����Ɏg�p���邱�ƂƂ���恙
			m_commandAllocators.emplace_back(new CommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT));	//	�R�}���h�A���P�[�^�[��ǉ�����恙
			m_failedProcessingNum = 0;																			//	������������������恙
			return;
		}
	}

	//	GPU��ҋ@��
	++index;
	index %= m_commandAllocators.size();
	m_commandAllocators[index]->WaitForGpu();
}