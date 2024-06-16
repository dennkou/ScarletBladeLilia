#include "DefaultRootSignature.h"

Crown::RenderObject::DefaultRootSignature::Instance Crown::RenderObject::DefaultRootSignature::instance;

Crown::RenderObject::RootSignature& Crown::RenderObject::DefaultRootSignature::GetRootSignature()
{
	return instance.GetInstance()->m_RootSignature;
}