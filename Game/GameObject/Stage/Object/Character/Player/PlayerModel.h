#pragma once
#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"
#include "./../../Crown/Object/RenderSystem/Model/Model.h"
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"

//	プレイヤーの見た目とアニメーションを制御するよ☆
class PlayerModel
{
public:
	enum class AnimState
	{
		Stand,			//	待機注
		StandToWalk,	//	待機から歩き初めに
		Walk,			//	歩いている
		WalkToStand,	//	歩き状態から待機に
		Run,			//	走っている
		Attack,			//	抜刀攻撃
		SlashAttack,	//	攻撃
		Avoidance,		//	回避
		Died,			//	死亡
	};
	static constexpr float ANIMATION_FPS = 30;						//	アニメーションのフレームレートだよ☆

	PlayerModel();
	~PlayerModel();

	void Update(float time);

	//	状態遷移☆
	void Attack();
	void Avoidance();
	void Move(DirectX::XMFLOAT2 move);

	//	アクセサ☆
	inline DirectX::XMFLOAT3 GetPosition() const noexcept { return m_model.GetPosition(); }
	inline void SetPosition(const DirectX::XMFLOAT3& position) noexcept { return m_model.SetPosition(position); }
	inline DirectX::XMFLOAT3 GetRotate() const noexcept { return m_model.GetRotate(); }
	inline void SetRotate(const DirectX::XMFLOAT3& rotate) noexcept { return m_model.SetRotate(rotate); }
	inline DirectX::XMMATRIX GetBone(const std::wstring& name) const noexcept { return m_bone[m_model.GetBoneDate().Find(name).GetIndex()]; }

	inline AnimState GetAnimState() const noexcept { return m_state.GetStateID(); }
	float GetAnimTime();
	float GetMaxFrame();
private:
	void CreateMaterial();
	void StateSetUp();
	void AnimLoad();

	Crown::RenderObject::Model m_model;
	DirectX::XMFLOAT2 m_move;

	//	アニメーション用の定数＆変数だよ☆

	DirectX::XMMATRIX m_bone[255];									//	現在プレイヤーが取るべきポーズだよ☆

	Crown::RenderObject::AnimationData m_runStartAnim;
	static constexpr float RUN_START_ANIM_SPEED = 2.0f;
	Crown::RenderObject::AnimationData m_runAnim;
	static constexpr float RUN_ANIM_SPEED = 7.0f;

	Crown::RenderObject::AnimationData m_frontStepAnim;
	static constexpr float FRONT_STEP_ANIM_SPEED = 1.0f;

	Crown::RenderObject::AnimationData m_drawingSwordAttackAnim;
	static constexpr float DRAWING_SWORD_ATTACK_ANIM_SPEED = 2.0f;

	Crown::RenderObject::AnimationData m_slashAttackAnim;
	static constexpr float SLASH_ATTACK_ANIM_SPEED = 0.5f;

	class PlayerAnimState;
	class PlayerAnimStand;
	class PlayerAnimStandToWalk;
	class PlayerAnimWalk;
	Crown::FiniteStateMachine<AnimState, PlayerAnimState> m_state;

	//static constexpr float SLASH_ATTACK_ANIM_SPEED = 0.5f;
};