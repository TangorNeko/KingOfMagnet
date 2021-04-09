#pragma once
#include "Character_base.h"
class Mage :public Character_base
{
	
public:
	bool Start();
	void Update();
	~Mage() {}
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

	//遅い歩き状態に切り替えできたら切り替える。
	void TryChangeStatusWalkSlow();

	//待機状態に切り替えできたら切り替える。
	void TryChangeStatusIdle();

	prefab::CSoundSource* m_chargeSound = nullptr;

	
	//状態更新
	void UpdateState();

	void HaveMachinegun();
	
};

