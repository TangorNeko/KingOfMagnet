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
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Walk,
		enAnimationClip_Idle,
		enAnimationClip_num,  //列挙内で使う要素の数を表すダミー
	};
	AnimationClip animationClips[enAnimationClip_num];

};

