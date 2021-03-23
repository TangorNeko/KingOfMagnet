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

	prefab::CSkinModelRender* m_weaponModel = nullptr;

	//固有攻撃ダメージ遅延
	int loop_count = 0;
	bool loop_flag = false;//攻撃があたったとき
	int SpecialAttack_count = 0;
	bool SpecialAttack_flag = false;//剣を振ったとき

	//移動アクションフラグ
	int m_move_count = 0;
	bool m_move_on=false;
	bool m_move_attack = true;

	Vector3 front;	
	Vector3 to_enemy;
	float angle_with_enemy;
	Vector3 position_with_enemy;

	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run, 		
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,
		enAnimationClip_num,  //列挙内で使う要素の数を表すダミー
	};
	enum EnStatus{
		enStatus_Attack,	//攻撃状態
		enStatus_Run,		//走り状態
		enStatus_Idle,		//待機状態
		enStatus_Walk,		//歩き状態
		enStatus_Move,		//移動アクション状態
		enStatus_Num,		//状態の数。
	};
	AnimationClip animationClips[enAnimationClip_num];
	EnStatus status = enStatus_Idle;	//ナイトの状態。
};

