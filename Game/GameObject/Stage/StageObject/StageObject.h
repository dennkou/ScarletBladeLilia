#pragma once
#include "./../../../../Crown/Object/RenderSystem/Model/Model.h"
#include "./../../GameObject.h"
#include <DirectXMath.h>

class StageObject
{
public:
	StageObject() = default;
	virtual ~StageObject() = default;

	virtual void Update() = 0;
private:

};