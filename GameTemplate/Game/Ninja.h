#pragma once
#include "Character_base.h"
class Ninja :public Character_base
{
	bool Start();
	void Update();
	~Ninja();

public:
	
	//�̗́A�`���[�W�A���͂̏�ԓ��̕\��
	void DisplayStatus();

	//�ړ��A�N�V����
	void MoveAction();

	//�ʏ�U��
	void NormalAttack();

	//�`���[�W
	void Charge();

	//�`���[�W�U��
	void SpecialAttack();


};


