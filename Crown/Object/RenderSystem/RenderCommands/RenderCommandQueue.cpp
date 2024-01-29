#include "RenderCommandQueue.h"

Crown::RenderObject::RenderCommand::RenderCommandQueue::RenderCommandQueue()
	:
	m_renderCommands(),
	m_useResource()
{
}

Crown::RenderObject::RenderCommand::RenderCommandQueue::RenderCommandQueue(std::vector<std::shared_ptr<RenderCommandBase>>& renderCommands, const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& useResource)
	:
	m_renderCommands(renderCommands),
	m_useResource(useResource)
{
}

Crown::RenderObject::RenderCommand::RenderCommandQueue::~RenderCommandQueue()
{
}

void Crown::RenderObject::RenderCommand::RenderCommandQueue::Run(ID3D12GraphicsCommandList* commandList, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& useResource) const
{
	for (const std::shared_ptr<RenderCommandBase>& renderCommand : m_renderCommands)
	{
		renderCommand->Run(commandList);
	}

	//	使用するリソースを登録☆
	useResource.resize(useResource.size() + m_useResource.size());
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_useResource.size()); i < size; ++i)
	{
		useResource.emplace_back(m_useResource[i]);
	}
}