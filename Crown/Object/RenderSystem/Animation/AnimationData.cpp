#include "AnimationData.h"
#include "./../../StringAlgorithm.h"
#include <algorithm>
#include <cmath>

Crown::RenderObject::AnimationData::AnimationData()
	:
	m_maxFramesNumber(0)
{

}

Crown::RenderObject::AnimationData::~AnimationData()
{

}

void Crown::RenderObject::AnimationData::LaodVMD(const std::wstring& filePath)
{
	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");
	if (file == nullptr)
	{
		assert(0);
		return ;
	}
	{
		//	ファイル形式の確認☆
		std::string checkString;
		checkString.resize(30);
		fread_s(checkString.data(), sizeof(char) * 30, sizeof(char), 30, file);
		if (checkString.find("Vocaloid Motion Data 0002\0") == -1)
		{
			assert(0);
			return ;
		}

		//	モデル名のスキップ☆
		std::string modelName;
		modelName.resize(20);
		fread_s(modelName.data(), sizeof(char) * 20, sizeof(char), 20, file);
	}

	struct Load
	{
		char name[15];
		unsigned int framesNumber;
		DirectX::XMFLOAT3 location;
		DirectX::XMFLOAT3 quaternion;
		unsigned char bezier[4][4][4];
	};

	unsigned int framesNum = 0;
	fread_s(&framesNum, sizeof(unsigned int), sizeof(unsigned int), 1, file);
	for (unsigned int i = 0; i < framesNum; ++i)
	{
		std::string boneName;
		boneName.resize(15);
		fread(boneName.data(), sizeof(char) * 15, 1, file);

		Motion& data = m_motionData[StringAlgorithm::StringToWstring(boneName)].emplace_back();

		fread(&data.framesNumber, sizeof(unsigned int), 1, file);
		if (data.framesNumber > m_maxFramesNumber)
		{
			m_maxFramesNumber = data.framesNumber;
		}
		fread(&data.location, sizeof(float) * 3, 1, file);
		fread(&data.quaternion, sizeof(float) * 4, 1, file);
		fread(data.bezier, sizeof(unsigned char), 64, file);
	}
	for (auto& sort : m_motionData)
	{
		std::sort(sort.second.begin(), sort.second.end(), [](const Motion& l, const Motion& r) {return l.framesNumber < r.framesNumber; });
	}
}

void Crown::RenderObject::AnimationData::GetAnimation(float time, DirectX::XMMATRIX* bones, const BoneData& boneMap) const
{
	//	time %= m_maxFramesNumber;☆
	if (m_maxFramesNumber > 0)
	{
		while (time > m_maxFramesNumber)
		{
			time -= m_maxFramesNumber;
		}
	}

	for (const std::pair<std::wstring, std::vector<Motion>>& boneData : m_motionData)
	{
		const Bone& node = boneMap.Find(boneData.first);
		const Motion* front = nullptr;
		const Motion* behind = nullptr;

		float t = 0;

		//	指定されたモーションがどのキーフレームの間にあるのか探すよ☆
		for (int i = 0, size = static_cast<int>(boneData.second.size()); i < size; ++i)
		{
			//	キーフレームが後ろかどうかでこのデータかどうか確認するよ☆
			if (boneData.second[i].framesNumber > time)
			{
				//	見つけた☆
				if (i > 0)
				{
					front = &boneData.second[i - 1];
				}
				else
				{
					front = &boneData.second[i];
				}

				behind = &boneData.second[i];
				//	補完値を算出☆
				t = (time - front->framesNumber) / (static_cast<float>(behind->framesNumber) - static_cast<float>(front->framesNumber));
				break;
			}
		}

		//	もし見つけてなかったら最後のデータを選択するよ☆
		if (front == nullptr || behind == nullptr)
		{
			front = &boneData.second.back();
			behind = &boneData.second.back();
		}

		DirectX::XMFLOAT3 pos = node.GetPosition();

		bones[node.GetIndex()] = DirectX::XMMatrixTranslation(-pos.x, -pos.y, -pos.z)
			* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionSlerp(front->quaternion, behind->quaternion, t))
			* DirectX::XMMatrixTranslation(std::lerp(front->location.x, behind->location.x, t), std::lerp(front->location.y, behind->location.y, t), std::lerp(front->location.z, behind->location.z, t))
			* DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	}
}