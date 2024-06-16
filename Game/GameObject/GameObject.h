#pragma once
#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "./../Game.h"
#include <DirectXMath.h>

class Enemy;

//	イベント引数用の前方宣言だよ☆
class Timer;

class Game::GameObject
{
public:
	GameObject(Game* game) : m_game(game) {}
	virtual ~GameObject() { m_game->DeleteGameObject(this); }


	//	イベントの定義だよ☆
	//	システム系☆
	virtual void OnGameInitialize() {};								//	ゲームが起動したよ☆
	virtual void OnGameFinalize() {}								//	ゲームが終了するよ☆
	virtual void OnGameUpdate(Timer& timer) { timer; }				//	更新☆

	//	入力系☆
	virtual void OnInputMenu() {}									//	メニューボタンが押されたよ☆

	//	ゲームプレイ中
	virtual void OnInputMove(DirectX::XMFLOAT2 input) { input; }	//	移動の入力が変わったよ☆
	virtual void OnInputAttackDown() {}								//	攻撃ボタンが押されたよ☆
	virtual void OnInputAvoidanceDown() {}							//	回避ボタンが押されたよ☆
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) { input; }	//	カメラが操作されたよ☆
	virtual void OnInputDash(bool input) { input; }					//	ダッシュボタンが押された&離されたよ☆

	//	メニュー中
	virtual void OnInputSelect() {}
	virtual void OnInputCursorUp() {}
	virtual void OnInputCursorDown() {}
	virtual void OnInputCursorLeft() {}
	virtual void OnInputCursorRight() {}
	virtual void OnInputCursorMove(DirectX::XMFLOAT2 input) { input; }

	//	シーン遷移系☆
	virtual void OnPlayStartAnimation() {}							//	タイトルからプレイを開始したよ☆
	virtual void OnPlayStart() {}									//	ゲームプレイを開始☆
	virtual void OnPlayEnd() {}										//	ゲームプレイを終了☆
	virtual void OnPlayRestart() {}									//	ゲームプレイを再開☆

	//	プレイ系☆
	virtual void OnSlowUIOperation() {}								//	ゲーム全体をスローモーションにするべきUIが操作されたよ☆
	virtual void OnStopUIOperation() {}								//	ゲーム全体を停止するべきUIが操作されたよ☆
	virtual void OnStartUIOperation() {}							//	


	virtual void OnPlayerDied()	{}
	virtual void OnEnemyDied(Enemy* diedEnemy) { diedEnemy; }		//	
	virtual void OnPlayerLockon() {}
	virtual void OnEnemyLockon() {}
	virtual void OnEnemyCreate() {}

	//	ゲームオブジェクトを追加するよ☆
	template<class Type, class... Argument>
	Type* CreateGameObject(Argument... argument)
	{
		return m_game->CreateGameObject<Type, Argument...>(argument...);
	}

	//	イベントを実行するよ☆
	template<class... Argument>
	void EventTrigger(void (GameObject::* handler)(Argument...), Argument... argument)
	{
		m_game->EventTrigger(handler, argument...);
	}
protected:


	void GameEnd()
	{
		m_game->GameEnd();
	}
private:
	Game* m_game;
};
#endif GAME_OBJECT