#pragma once
#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"
#include "./../../Crown/Object/RenderSystem/Model/Model.h"
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"

//	プレイヤーの見た目とアニメーションを制御するよ☆
class PlayerModel
{
public:
	static constexpr float ANIMATION_FPS = 30;						//	アニメーションのフレームレートだよ☆

	PlayerModel();
	~PlayerModel();

	void Update();

	//	アクセサ☆
	inline DirectX::XMFLOAT3 GetPosition() const noexcept { return m_model.GetPosition(); }
	inline void SetPosition(const DirectX::XMFLOAT3& position) noexcept { return m_model.SetPosition(position); }
	inline DirectX::XMFLOAT3 GetRotate() const noexcept { return m_model.GetRotate(); }
	inline void SetRotate(const DirectX::XMFLOAT3& rotate) noexcept { return m_model.SetRotate(rotate); }
	inline DirectX::XMMATRIX* GetBone() noexcept { return m_bone; }
	inline const DirectX::XMMATRIX& GetBone(std::wstring name) const noexcept { return m_bone[m_model.GetBoneDate().Find(name).GetIndex()]; }
	inline const BoneData& GetBoneMap() const noexcept { return m_model.GetBoneDate(); }

	inline Crown::RenderObject::AnimationData GetStandAnim() const noexcept { return m_standAnim; }
	inline Crown::RenderObject::AnimationData GetWalkAnim() const noexcept { return m_walkAnim; }
	inline Crown::RenderObject::AnimationData GetWalkStartAnim() const noexcept { return m_walkStartAnim; }
	//inline Crown::RenderObject::AnimationData GetRunStartAnim() const noexcept { return m_runStartAnim; }
	inline Crown::RenderObject::AnimationData GetRunAnim() const noexcept { return m_runAnim; }
	inline Crown::RenderObject::AnimationData GetFrontStepAnim() const noexcept { return m_frontStepAnim; }
	inline Crown::RenderObject::AnimationData GetDrawingSwordAttackAnim() const noexcept { return m_drawingSwordAttackAnim; }
	inline Crown::RenderObject::AnimationData GetSlashAttackAnim() const noexcept { return m_slashAttackAnim; }
private:
	void AnimLoad();
	void CreateMaterial();

	Crown::RenderObject::Model m_model;
	DirectX::XMFLOAT2 m_move;

	//	アニメーション用の定数＆変数だよ☆

	DirectX::XMMATRIX m_bone[255];									//	現在プレイヤーが取るべきポーズだよ☆

	Crown::RenderObject::AnimationData m_standAnim;
	Crown::RenderObject::AnimationData m_walkAnim;
	Crown::RenderObject::AnimationData m_walkStartAnim;
	//Crown::RenderObject::AnimationData m_runStartAnim;
	Crown::RenderObject::AnimationData m_runAnim;
	Crown::RenderObject::AnimationData m_frontStepAnim;
	Crown::RenderObject::AnimationData m_drawingSwordAttackAnim;
	Crown::RenderObject::AnimationData m_slashAttackAnim;


	//static constexpr float SLASH_ATTACK_ANIM_SPEED = 0.5f;
};