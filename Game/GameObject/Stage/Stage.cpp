#include "Stage.h"

#include "./Object/Character/Player/Player.h"
#include "./Object/Character/Enemy/Enemy.h"
#include "./Object/Character/Enemy/AIState/NavigationAI/PointPatrol.h"
#include "./Collider/ColliderSystem.h"

Stage::Stage(Game* game)
	:
	GameObject(game)
{
	this->CreateGameObject<Player>(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0, 0, 0));	//	プレイヤーを生成するよ☆

	std::shared_ptr<std::mt19937> random;
	random.reset(new std::mt19937(8239));
	this->CreateGameObject<Enemy>(DirectX::XMFLOAT3(0, 0, -100), DirectX::XMFLOAT3(0, 0, 0), new PointPatrol({DirectX::XMFLOAT3(0,0,-150), DirectX::XMFLOAT3(0,0,-500), DirectX::XMFLOAT3(10,0,-500) }), random);

	Crown::RenderObject::Model model;
	model.LoadPMX(L"Resource/Model/PMX/ステージ通路.pmx");
	{
		Aisle* aisle = new Aisle();
		int num = 100;
		float rotate = 0.0f;
		aisle->Create(DirectX::XMFLOAT3(-(num * 100.0f / 2) * sin(DirectX::XMConvertToRadians(rotate)), 0, -(num * 100.0f / 2) * cos(DirectX::XMConvertToRadians(rotate))), rotate, &model, num);
		m_stageObjects.push_back(std::unique_ptr<StageObject>(aisle));
	}
}

Stage::~Stage()
{

}

void Stage::OnGameUpdate(Timer& timer)
{
	timer;
	ColliderSystem::Update();

	for (unsigned int i = 0, size = static_cast<unsigned int>(m_stageObjects.size()); i < size; ++i)
	{
		m_stageObjects[i]->Update();
	}
}
