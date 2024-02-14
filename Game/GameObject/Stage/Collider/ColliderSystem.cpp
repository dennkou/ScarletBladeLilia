#include "ColliderSystem.h"
#include "InstanceCollection.h"

ColliderSystem::InstanceCollection ColliderSystem::m_instanceCollection;

void ColliderSystem::Update()
{
	m_instanceCollection.Update();
}