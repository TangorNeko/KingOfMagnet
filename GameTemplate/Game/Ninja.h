#pragma once
#include "Character_base.h"
class Ninja :public Character_base
{

public:
	bool Start();
	~Ninja();
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


};


