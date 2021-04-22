#pragma once
#include "Character_base.h"
class Knight :public Character_base
{
	bool Start();
	void Update();
	~Knight();
public:
	//��ԍX�V
	void UpdateState();

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

	//�ҋ@��Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusIdle();

	//�ړ��A�N�V������Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusMove();

	//������Ԃɐ؂�ւ���
	void TryChangeStatusFall();
	
	prefab::CSoundSource* m_swordSound=nullptr;
	prefab::CSoundSource* m_chargeSound = nullptr;
	float m_chargeSoundVolume = 0.6f;
	//�ŗL�U���_���[�W�x��
	int loop_count = 0;
	bool loop_flag = false;//�U�������������Ƃ�
	int SpecialAttack_count = 0;
	bool SpecialAttack_flag = false;//����U�����Ƃ�
	//�ړ��A�N�V�����t���O
	int m_move_count = 0;
	bool m_move_on=false;
	bool m_move_attack = true;
	void HaveMachinegun();
	//���͒e
	void HaveGravityGrenade();};

