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
		// �쐬�� : 2023/9/6
		// �쐬�� : �����D��
		// 
		// �A�j���[�V�����̃f�[�^���恙
		// 
		//================================================
		class AnimationData
		{
		public:
			AnimationData();
			~AnimationData();

			/// <summary>
			/// VMD�t�@�C���̓ǂݍ��݂��恙
			/// -1��Ԃ�����ǂݍ��݂Ɏ��s�������Ƃ������恙
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			void LaodVMD(const std::wstring& filePath);

			/// <summary>
			/// �A�j���[�V�����f�[�^���擾����恙
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