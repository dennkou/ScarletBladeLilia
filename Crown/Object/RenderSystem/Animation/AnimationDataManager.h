#pragma once
#ifndef CROWN_RENDEROBJECT_ANIMATIONDATAMANAGER
#define CROWN_RENDEROBJECT_ANIMATIONDATAMANAGER
//================================================
// 作成日 : 2023/6/7
// 作成者 : 服部颯澄
// 
// アニメーションデータを管理をするよ☆
// 
//================================================
#include <string>
#include <DirectXMath.h>
#include <unordered_map>
#include "AnimationData.h"
#include <memory>
namespace Crown
{
	namespace RenderObject
	{
		class AnimationDataManager
		{
		public:
			~AnimationDataManager();
			static AnimationDataManager& GetInstance()noexcept;

			const AnimationData* GetAnimationData(const std::wstring& name);
		private:
			AnimationDataManager();
			AnimationDataManager(AnimationDataManager& me) = delete;
			AnimationDataManager& operator=(AnimationDataManager& me) = delete;
			static std::unique_ptr<AnimationDataManager> me;

			void LoadAnimattion(const std::wstring& filePath);

			std::unordered_map<std::wstring, AnimationData> m_animationDataMap;
		};
	}
}
#endif // !CROWN_RENDEROBJECT_ANIMATIONDATAMANAGER