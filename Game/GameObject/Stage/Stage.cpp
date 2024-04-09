#include "Stage.h"

#include "./Object/Character/Player/Player.h"
#include "./Object/Character/Enemy/Enemy.h"
#include "./Object/Character/Enemy/State/Patrol/NavigationAI/PointPatrol.h"
#include "./StageObject/StageObject.h"
#include "./Collider/ColliderSystem.h"

Stage::Stage(Game* game)
	:
	GameObject(game)
{
	this->CreateGameObject<Player>(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0, 0, 0));	//	ƒvƒŒƒCƒ„[‚ð¶¬‚·‚é‚æ™


	this->CreateGameObject<Enemy>(DirectX::XMFLOAT3(0, 0, -100), DirectX::XMFLOAT3(0, 0, 0), new PointPatrol({DirectX::XMFLOAT3(0,0,-150), DirectX::XMFLOAT3(0,0,-500), DirectX::XMFLOAT3(10,0,-500) }));

	int num = 100;
	for (int i = 0; i < num; ++i)
	{
		this->CreateGameObject<StageObject>(DirectX::XMFLOAT3(0, 0, (i * 99.9999) - ((num / 2) * 99.9999)));
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
