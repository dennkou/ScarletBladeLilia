#include "Stage.h"

#include "./Object/Character/Player/Player.h"
#include "./Object/Character/Enemy/Enemy.h"
#include "./StageObject/StageObject.h"
#include "./Collider/ColliderSystem.h"

Stage::Stage(Game* game)
	:
	GameObject(game)
{
	this->CreateGameObject<Player>(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0, 0, 0));	//	ƒvƒŒƒCƒ„[‚ð¶¬‚·‚é‚æ™
	this->CreateGameObject<Enemy>(DirectX::XMFLOAT3(0,0,-100), DirectX::XMFLOAT3(0, 0, 0));

	int num = 100;
	for (int i = 0; i < num; ++i)
	{
		this->CreateGameObject<StageObject>(DirectX::XMFLOAT3(0, 0, (i * 100) - ((num / 2) * 100.0f)));
	}
}

Stage::~Stage()
{

}

void Stage::OnGameUpdate(Timer& timer)
{
	timer;
	ColliderSystem::Update();
}
