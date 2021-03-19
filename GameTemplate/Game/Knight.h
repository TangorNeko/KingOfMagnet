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

	prefab::CSkinModelRender* m_weaponModel = nullptr;

	//�ŗL�U���_���[�W�x��
	int loop_count = 0;
	bool loop_flag = false;
	//�ړ��A�N�V�����t���O
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
		enAnimationClip_num,  //�񋓓��Ŏg���v�f�̐���\���_�~�[
	};
	enum EnStatus{
		enStatus_Attack,	//�U�����
		enStatus_Run,		//������
		enStatus_Idle,		//�ҋ@���
		enStatus_Walk,		//�������
		enStatus_Num,		//��Ԃ̐��B
	};
	AnimationClip animationClips[enAnimationClip_num];
	EnStatus status = enStatus_Idle;	//�i�C�g�̏�ԁB
};

