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

	
	//��ԍX�V
	void UpdateState();
	
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Walk,		
		enAnimationClip_Idle,
		enAnimationClip_num,  //�񋓓��Ŏg���v�f�̐���\���_�~�[
	};
	enum EnStatus {
		enStatus_Attack,	//�U�����
		enStatus_Run,		//������		
		enStatus_Walk,		//�������			
		enStatus_Idle,		//�ҋ@���
		enStatus_Num,		//��Ԃ̐��B
	};
	AnimationClip animationClips[enAnimationClip_num];
	EnStatus status = enStatus_Idle;	//���@�g���̏�ԁB

};

