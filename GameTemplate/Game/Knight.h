#pragma once
#include "Character_base.h"
class Knight :public Character_base
{
	bool Start();
	void Update();
	~Knight();
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

//�ŗL�U���_���[�W�x��
	int loop_count = 0;
	bool loop_flag = false;
	enum {
		enAnimationClip_Attack,

		enAnimationClip_num,  //�񋓓��Ŏg���v�f�̐���\���_�~�[
	};
	AnimationClip animationClips[enAnimationClip_num];
};

