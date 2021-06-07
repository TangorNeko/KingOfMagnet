#pragma once
#include "MyCapsuleCollider.h"

class Player;
class BackGround;
class GameScene;

class Bomb : public IGameObject
{
	~Bomb();
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

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//���e�̃��f��

	MyCapsuleCollider m_bulletCollider;//�v���C���[�Ƃ̓����蔻��p�̃J�v�Z����̓����蔻��

	BackGround* m_stageModel = nullptr;//�����蔻��p�̃X�e�[�W�̃N���X

	GameScene* m_gameScene = nullptr;

	const float m_velocity = 20.0f;//�e��

	//�����܂ł̃J�E���g
	int m_explosionCount = 0;
public:
	//���e�̏��
	enum enBombState
	{
		enDrop,//�����Ă���B
		enBullet,//�e�Ƃ��Ĕ��˂���Ă���
		enHold,//�v���C���[���ێ����Ă���
		enPop,//�����ɓ��������@�������O
	};

	enBombState m_bombState = enDrop;

	//���e�̎��
	enum enBombShape
	{
		enGrenade,//���e
		enFlashGrenade,//�M���e
		enIncendiaryGrenade,//�ĈΒe
	};

	enBombShape m_bombShape = enGrenade;

	Vector3 m_position;//���W
	Vector3 m_oldPosition;//�O�t���[���̍��W

	Player* m_parent = nullptr;//�e�̃v���C���[(�z�[���h���A���ˎ��Ɏg�p)

	Vector3 m_moveDirection = { 0.0f,0.0f,0.0f };//�ړ��������

	//�n�ʂɂ��Ă��邩�ǂ���
	bool m_isOnGround = false;
};

