#pragma once
#include <vector>
#include <DirectXMath.h>
#include <unordered_map>
#include <string>

//	ボーン一つ一つのデータ☆
class Bone
{
public:
	Bone();
	~Bone();

	inline void RecursiveMatrixMultiply(DirectX::XMMATRIX pause[255]) const
	{
		RecursiveMatrixMultiply(pause, DirectX::XMMatrixIdentity());
	}

	inline void SetPosition(const DirectX::XMFLOAT3& pos) noexcept { m_position = pos; }
	inline const DirectX::XMFLOAT3& GetPosition() const noexcept { return m_position; }
	inline void SetIndex(unsigned int index) noexcept { m_index = index; };
	inline unsigned int GetIndex() const noexcept { return m_index; }
	inline void PushChildren(Bone* children) { m_children.push_back(children); children->SetParent(this); }

	inline DirectX::XMMATRIX GetWorldMatrix(DirectX::XMMATRIX pause[255]) const noexcept
	{
		if (parent)
		{
			return pause[m_index] *= parent->GetWorldMatrix(pause);
		}
		else
		{
			return pause[m_index];
		}
	}
private:
	inline void SetParent(Bone* bone) noexcept { parent = bone; }

	unsigned int m_index;
	DirectX::XMFLOAT3 m_position;
	std::vector<Bone*> m_children;
	Bone* parent;

	inline void RecursiveMatrixMultiply(DirectX::XMMATRIX pause[255], const DirectX::XMMATRIX& matrix) const
	{
		pause[m_index] *= matrix;
		for (const Bone* childrenNode : m_children)
		{
			childrenNode->RecursiveMatrixMultiply(pause, pause[m_index]);
		}
	}
};

//	ボーンの集合。一つのモデルのボーン情報を表すよ☆
class BoneData
{
public:
	BoneData();
	~BoneData();

	std::unordered_map<std::wstring, Bone>& GetData() { return m_data; }
	const Bone& Find(const std::wstring& name) const;



	inline bool IsActive() const noexcept { return !m_data.empty(); }
private:
	std::unordered_map<std::wstring, Bone> m_data;
};