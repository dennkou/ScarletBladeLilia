#pragma once
#include <vector>
#include "./StageObject.h"
#include "./../../../../Crown/Object/RenderSystem/Model/Model.h"
#include "./../../GameObject.h"
#include <DirectXMath.h>
#include "./../Collider/AisleCollider.h"
#include "./../StageModel.h"

class Aisle : public StageObject
{
public:
	Aisle();
	~Aisle();

	void Create(DirectX::XMFLOAT3 position, float rotate, const Crown::RenderObject::Model* model, unsigned int num);

	virtual void Update() override;
private:
	static constexpr float size = 100.0f;
	void CreateMaterial();
	std::vector<Crown::RenderObject::Model> m_model;

	ColliderSystem::AisleCollider m_collider;

	std::unique_ptr<StageModel> m_stageModel;
};