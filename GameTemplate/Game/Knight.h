#pragma once
#include "Character_base.h"
class Knight :public Character_base
{
	bool Start();
	void Update();
	~Knight();
public:
	//状態更新
	void UpdateState();

	//体力、チャージ、磁力の状態等の表示
	void DisplayStatus();

	//移動アクション
	void MoveAction();

	//通常攻撃
	void NormalAttack();

	//チャージ
	void Charge();

	//チャージ攻撃
	void SpecialAttack();

	//アニメーション
	void AnimationSelect();

	//攻撃状態に切り替えできたら切り替える。
	void TryChangeStatusAttack();

	//走り状態に切り替えできたら切り替える。
	void TryChangeStatusRun();
	
	//歩き状態に切り替えできたら切り替える。
	void TryChangeStatusWalk();

	//待機状態に切り替えできたら切り替える。
	void TryChangeStatusIdle();

	//移動アクション状態に切り替えできたら切り替える。
	void TryChangeStatusMove();

	//落下状態に切り替える
	void TryChangeStatusFall();
	
	prefab::CSoundSource* m_swordSound=nullptr;
	prefab::CSoundSource* m_chargeSound = nullptr;
	float m_chargeSoundVolume = 0.6f;
	//固有攻撃ダメージ遅延
	int loop_count = 0;
	bool loop_flag = false;//攻撃があたったとき
	int SpecialAttack_count = 0;
	bool SpecialAttack_flag = false;//剣を振ったとき
	//移動アクションフラグ
	int m_move_count = 0;
	bool m_move_on=false;
	bool m_move_attack = true;
	void HaveMachinegun();
	//引力弾
	void HaveGravityGrenade();};

