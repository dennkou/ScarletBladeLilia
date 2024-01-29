#pragma once
#include "./../../GameObject.h"
#include <DirectXMath.h>
#include "./../../Crown/Object/RenderSystem/Model/Model.h"

//	ステージ上に配置されるオブジェクトだよ☆
class Object : public Game::GameObject
{
public:
	Object(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate , std::wstring modelName)
		:
		GameObject(game)
	{
		m_model.LoadPMX(modelName);
		m_model.SetPosition(position);
		m_model.SetRotate(rotate);
	}
	virtual ~Object()
	{

	}

	virtual DirectX::XMFLOAT3 GetPosition() const noexcept final { return m_model.GetPosition(); }
	virtual DirectX::XMFLOAT3 GetRotate() const noexcept final { return m_model.GetRotate(); }
protected:
	virtual void SetPosition(const DirectX::XMFLOAT3& position) noexcept { m_model.SetPosition(position); }
	virtual void SetRotate(const DirectX::XMFLOAT3& rotate) noexcept { m_model.SetRotate(rotate); }
	virtual void SetPause(const DirectX::XMMATRIX* pause) noexcept { m_model.SetPause(pause); }
	virtual Crown::RenderObject::Model& GetModel() noexcept { return m_model; }
private:
	Crown::RenderObject::Model m_model;
};