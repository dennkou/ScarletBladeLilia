#include "Material.h"

Crown::RenderObject::Material::Material()
{

}

Crown::RenderObject::Material::~Material()
{
}

void Crown::RenderObject::Material::CreateData(RenderCommand::RenderCommandQueue& commandQueue, std::vector<BlobConstBuffer>& constantBuffers)
{
	for (BlobConstBuffer& buffer : constantBuffers)
	{
		m_constBuffers.push_back(buffer);
	}
	m_commandQueue = commandQueue;
}

Crown::RenderObject::BlobConstBuffer& Crown::RenderObject::Material::GetConstBuffer(unsigned int constantBufferIndex)
{
	return m_constBuffers[constantBufferIndex];
}