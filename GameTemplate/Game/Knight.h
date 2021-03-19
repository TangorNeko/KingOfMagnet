#pragma once
#include "Character_base.h"
class Knight :public Character_base
{
	bool Start();
	void Update();
	~Knight();
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

	prefab::CSkinModelRender* m_weaponModel = nullptr;

//固有攻撃ダメージ遅延
	int loop_count = 0;
	bool loop_flag = false;
	enum {
		enAnimationClip_Attack,

		enAnimationClip_num,  //列挙内で使う要素の数を表すダミー
	};
	AnimationClip animationClips[enAnimationClip_num];
};

