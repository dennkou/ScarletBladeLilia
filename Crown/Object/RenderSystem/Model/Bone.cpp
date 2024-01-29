#include "Bone.h"

BoneData::BoneData()
{

}

BoneData::~BoneData()
{

}

const Bone& BoneData::Find(const std::wstring& name) const
{
    return m_data.at(name);
}

Bone::Bone()
{

}

Bone::~Bone()
{

}