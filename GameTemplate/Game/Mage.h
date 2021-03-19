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
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Walk,
		enAnimationClip_Idle,
		enAnimationClip_num,  //�񋓓��Ŏg���v�f�̐���\���_�~�[
	};
	AnimationClip animationClips[enAnimationClip_num];

};

