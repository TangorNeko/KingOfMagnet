#pragma once
#include "Character_base.h"
class Tank :public Character_base
{
	bool Start();
	void Update();
	~Tank();
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

