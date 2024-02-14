#pragma once
#include "./../Character.h"
#include "./PlayerCamera.h"
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"
#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"
#include "./../../../Collider/PlayerCollider.h"

class Player : public Character
{
public:
	Player(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate);
	virtual ~Player() override;

	virtual void OnGameUpdate(Timer& timer) override;
	virtual void OnPlayStartAnimation() override;
	virtual void OnPlayStart() override;

	//	入力系☆
	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	移動の入力が変わったよ☆
	virtual void OnInputAttackDown() override;						//	攻撃ボタンが押されたよ☆
	virtual void OnInputAvoidanceDown() override;					//	回避ボタンが押されたよ☆
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;	//	
private:
	void CreateMaterial();
	void StateSetUp();
	void AnimLoad();
	void ColliderUpdate();

	//	入力によりカメラを回転させるよ☆
	void CameraRoll(DirectX::XMFLOAT2 input);

	//	プレイヤーのステータスだよ☆
	static constexpr int MAX_HP = 1000;								//	プレイヤーの最大HPだよ☆

	PlayerCamera m_camera;
	ColliderSystem::PlayerCollider m_collider;
	DirectX::XMMATRIX m_bone[255];									//	現在プレイヤーが取るべきポーズだよ☆
	float m_stateTimer;												//	ステートで利用する想定のタイマーだよ☆この値が何を示すかは現在のステートに依存するよ☆
	DirectX::XMFLOAT2 m_inputMove;
	float m_targetAngle;											//	プレイヤーが向うとしている角度だよ☆
	bool hasSword;													//	剣を手に持っているよ☆

	class PlayerState;
	class PlayerTitle;
	class PlayerDefault;
	class PlayerAttack;
	class PlayerAvoidance;

	enum class StateID
	{
		Title,
		Normal,
		Attack,
		Avoidance,
	};

	Crown::FiniteStateMachine<StateID, PlayerState> m_playerState;	//	状態を示すステートマシンだよ☆

	//	アニメーション用の定数＆変数だよ☆

	static constexpr float ANIMATION_FPS = 30;						//	アニメーションのフレームレートだよ☆
	Crown::RenderObject::AnimationData m_standAnim;					
	static constexpr float STAND_ANIM_SPEED = 1.0f;					
	Crown::RenderObject::AnimationData m_runStartAnim;				
	static constexpr float RUN_START_ANIM_SPEED = 2.0f;				
	Crown::RenderObject::AnimationData m_runAnim;					
	static constexpr float RUN_ANIM_SPEED = 2.0f;					
	Crown::RenderObject::AnimationData m_drawingSwordAttackAnim;	
	static constexpr float DRAWING_SWORD_ATTACK_ANIM_SPEED = 2.0f;	

};