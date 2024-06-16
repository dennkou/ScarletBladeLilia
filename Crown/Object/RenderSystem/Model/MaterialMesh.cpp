#include "MaterialMesh.h"

Crown::RenderObject::MaterialMesh::MaterialMesh(unsigned int indexNum, unsigned int m_indexOffset)
	:
	m_indexNum(indexNum),
	m_indexOffset(m_indexOffset),
	m_materials(static_cast<int>(MaterialTag::Num)),
	m_materialFlag(static_cast<int>(MaterialTag::Num))
{
}

Crown::RenderObject::MaterialMesh::~MaterialMesh()
{
}