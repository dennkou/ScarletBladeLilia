#pragma once
#ifndef CROWN_RENDEROBJECT_ANIMATIONDATA
#define CROWN_RENDEROBJECT_ANIMATIONDATA
#include <string>
#include <DirectXMath.h>
#include <unordered_map>
#include "./../Model/Bone.h"

namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// 作成日 : 2023/9/6
		// 作成者 : 服部颯澄
		// 
		// アニメーションのデータだよ☆
		// 
		//================================================
		class AnimationData
		{
		public:
			AnimationData();
			~AnimationData();

			/// <summary>
			/// VMDファイルの読み込みだよ☆
			/// -1を返したら読み込みに失敗したことを示すよ☆
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			void LaodVMD(const std::wstring& filePath);

			/// <summary>
			/// アニメーションデータを取得するよ☆
			/// </summary>
			/// <param name="time"></param>
			/// <param name="bones"></param>
			/// <param name="boneMap"></param>
			void GetAnimation(float time, DirectX::XMMATRIX* bones, const BoneData& boneMap) const;

			inline unsigned int GetMaxFrame() const noexcept { return m_maxFramesNumber; }

		private:
			struct Motion
			{
				unsigned int framesNumber;
				DirectX::XMFLOAT3 location;
				DirectX::XMVECTOR quaternion;
				unsigned int bezier[4][4][4];
			};
			std::unordered_map<std::wstring, std::vector<Motion>> m_motionData;

			unsigned int m_maxFramesNumber;
		};
	}
}
#endif	//!CROWN_RENDEROBJECT_ANIMATIONDATA