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
		m_graphicsCommandList[i].commandList->Close();																	//	コマンドリストをクローズ☆以降このコマンドリストは命令を受け付けないよ☆
		commandList[i] = m_graphicsCommandList[i].commandList.Get();
	}

	m_commandQueue->ExecuteCommandLists(commandListNum, commandList.data());	//	コマンドリストの内容を実行するよ☆
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
	//	バッファー数が必ず1以上になるようにするよ☆
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
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;																						//	タイムアウトなしだよ☆
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;																				//	優先度は高いよ☆
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;																						//	コマンドリストのタイプはダイレクトコマンドリストだよ☆
	device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_commandQueue));																//	コマンドキュー生成だよ☆

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()));
	m_fenceVal = 0;
}

void Crown::RenderObject::GraphicsCommandList::WaitForGpu() noexcept
{
	m_fence->SetEventOnCompletion(m_fenceVal, nullptr);
}

void Crown::RenderObject::GraphicsCommandList::CopyExecute()
{
	m_copyCommandList->Close();																									//	コピー用コマンドリストをクローズ☆
	ID3D12CommandList* cmdlists[] = { m_copyCommandList.Get() };
	m_commandQueue->ExecuteCommandLists(1, cmdlists);																			//	コマンドリストの内容を実行するよ☆
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
		m_graphicsCommandList[i].commandList->Close();																	//	コマンドリストをクローズ☆以降このコマンドリストは命令を受け付けないよ☆
		commandList[i] = m_graphicsCommandList[i].commandList.Get();
	}

	m_commandQueue->ExecuteCommandLists(commandListNum, commandList.data());	//	コマンドリストの内容を実行するよ☆
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
	//	次に実行使うコマンドアロケーターを探すよ☆
	for (int i = 0, size = static_cast<int>(m_commandAllocators.size()); i < size; ++i)
	{
		//	コマンドアロケーターの中から処理が終わっているものを探すよ☆
		if (!m_commandAllocators[i]->NotDrewEnd())
		{
			//	処理が終っているのでこのコマンドアロケーターを次に使用することとするよ☆
			index = i;
			m_failedProcessingNum = 0;
			return;
		}
	}

	//	使えるコマンドアロケーターが無いので処理落ちが発生したと判定するよ☆
	++m_failedProcessingNum;

	//	処理落ちが許可されたフレーム数より多く発生している？
	if (MAX_FAILEDPROCESSING_NUM < m_failedProcessingNum)
	{
		if (m_addCommandAllocatorNum > 0)
		{
			--m_addCommandAllocatorNum;																			//	現在許可されたアロケーターの数を減らすよ☆
			index = static_cast<int>(m_commandAllocators.size());												//	追加されるコマンドアロケーターを次に使用することとするよ☆
			m_commandAllocators.emplace_back(new CommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT));	//	コマンドアロケーターを追加するよ☆
			m_failedProcessingNum = 0;																			//	処理落ちを回避したよ☆
			return;
		}
	}

	//	GPUを待機☆
	++index;
	index %= m_commandAllocators.size();
	m_commandAllocators[index]->WaitForGpu();
}