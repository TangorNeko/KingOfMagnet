#pragma once
#include "Character_base.h"
class Knight :public Character_base
{
	
public:
	bool Start();
	~Knight();
	void Update()override;
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

	prefab::CSkinModelRender* m_weaponModel = nullptr;
};

