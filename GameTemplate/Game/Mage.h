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

	
	//状態更新
	void UpdateState();
	
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Walk,		
		enAnimationClip_Idle,
		enAnimationClip_num,  //列挙内で使う要素の数を表すダミー
	};
	enum EnStatus {
		enStatus_Attack,	//攻撃状態
		enStatus_Run,		//走り状態		
		enStatus_Walk,		//歩き状態			
		enStatus_Idle,		//待機状態
		enStatus_Num,		//状態の数。
	};
	AnimationClip animationClips[enAnimationClip_num];
	EnStatus status = enStatus_Idle;	//魔法使いの状態。

};

