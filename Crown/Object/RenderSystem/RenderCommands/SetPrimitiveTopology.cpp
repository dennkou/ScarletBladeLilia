#include "SetPrimitiveTopology.h"

Crown::RenderObject::RenderCommand::SetPrimitiveTopology::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology)
	:
	m_primitiveTopology(primitiveTopology)
{
}

Crown::RenderObject::RenderCommand::SetPrimitiveTopology::~SetPrimitiveTopology()
{
}

void Crown::RenderObject::RenderCommand::SetPrimitiveTopology::Run(ID3D12GraphicsCommandList* commandList)
{
	commandList->IASetPrimitiveTopology(m_primitiveTopology);
}