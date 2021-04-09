#pragma once
#include "Character_base.h"
class Mage :public Character_base
{
	
public:
	bool Start();
	void Update();
	~Mage() {}
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

	//�A�j���[�V����
	void AnimationSelect();

	//�U����Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusAttack();

	//�����Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusRun();

	//������Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusWalk();

	//�x��������Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusWalkSlow();

	//�ҋ@��Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusIdle();

	prefab::CSoundSource* m_chargeSound = nullptr;

	
	//��ԍX�V
	void UpdateState();

	void HaveMachinegun();
	
};

