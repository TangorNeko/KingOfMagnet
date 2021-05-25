#pragma once
#include "MyCapsuleCollider.h"

class Player;
class BackGround;
class SampleScene;

class Debris : public IGameObject
{
	~Debris();
	bool Start() override;
	void Update() override;

	//�n�ʂɗ����Ă��鎞�̋���
	void AsDropBehave();

	//�e�Ƃ��Ĕ��˂���Ă��鎞�̋���
	void AsBulletBehave();

	//�v���C���[�ɕێ�����Ă��鎞�̋���
	void AsHoldBehave();

	//�����ɓ���������̋���
	void AsPopBehave();

public:
	//�K���L�̏��
	enum enDebrisState
	{
		enDrop,//�����Ă���B
		enBullet,//�e�Ƃ��Ĕ��˂���Ă���
		enHold,//�v���C���[���ێ����Ă���
		enPop,//�����ɓ���������
	};

	enDebrisState m_debrisState = enDrop;

	//�K���L�̌`
	enum enDebrisShape
	{
		enStone,//��
		enSword,//��
		//�������瑼�̃A�C�e��(���@�̏�)�Ȃǒǉ����Ă���?
		enSpecialCharger,
	};

	enDebrisShape m_debrisShape = enStone;

	Vector3 m_position;//���W
	Vector3 m_oldPosition;//�O�t���[���̍��W

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//�K���L�̃��f��

	Player* m_parent = nullptr;//�e�̃v���C���[(�z�[���h���A���ˎ��Ɏg�p)

	MyCapsuleCollider m_bulletCollider;//�v���C���[�Ƃ̓����蔻��p�̃J�v�Z����̓����蔻��

	BackGround* m_stageModel = nullptr;//�����蔻��p�̃X�e�[�W�̃N���X

	SampleScene* m_gameScene = nullptr;

	Vector3 m_moveDirection = { 0.0f,0.0f,0.0f };//�ړ��������
	const float m_velocity = 50.0f;//�e��

	//�X�y�V�����`���[�W���[�p
	float m_specialChargeCount = 0.0f;	//�X�y�V�����Q�[�W�𑝂₷�܂ł̃J�E���g

	//�n�ʂɂ��Ă��邩�ǂ���
	bool m_isOnGround = false;
};

