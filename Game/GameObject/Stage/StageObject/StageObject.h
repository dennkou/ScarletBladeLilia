#pragma once
#include "./../Object/Object.h"
#include "./../../../../Crown/Object/RenderSystem/Model/Model.h"

class StageObject : public Object
{
public:
	StageObject(Game* game, DirectX::XMFLOAT3 position);
	~StageObject();


private:
	void CreateMaterial();
};