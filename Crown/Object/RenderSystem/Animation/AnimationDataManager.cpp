#include "AnimationDataManager.h"

std::unique_ptr<Crown::RenderObject::AnimationDataManager> Crown::RenderObject::AnimationDataManager::me;

Crown::RenderObject::AnimationDataManager::AnimationDataManager()
{

}

Crown::RenderObject::AnimationDataManager::~AnimationDataManager()
{

}

Crown::RenderObject::AnimationDataManager& Crown::RenderObject::AnimationDataManager::GetInstance() noexcept
{
	if (!me)
	{
		me.reset(new AnimationDataManager);
	}
	return *me.get();
}

const Crown::RenderObject::AnimationData* Crown::RenderObject::AnimationDataManager::GetAnimationData(const std::wstring& name)
{
	//	‚Ü‚¾“Ç‚İ‚Ü‚ê‚Ä‚¢‚È‚¢–¼‘O‚È‚ç“Ç‚İ‚İ‚ğ‚İ‚é‚æ™
	if (m_animationDataMap.find(name) == m_animationDataMap.end())
	{
		LoadAnimattion(name);
	}
	return &m_animationDataMap[name];
}

void Crown::RenderObject::AnimationDataManager::LoadAnimattion(const std::wstring& filePath)
{
	m_animationDataMap[filePath].LaodVMD(filePath);
}
