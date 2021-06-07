#pragma once
#include "MyCapsuleCollider.h"

class Debris;
class Player;
class BackGround;
class GameScene;

class GravityBullet : public IGameObject
{
	~GravityBullet();
	bool Start() override;
	void Update() override;

	//���˂���Ă���r���̋���
	void AsBulletBehave();

	//���e�����u�Ԃ̋���
	void AsExplodeBehave();

	//���e������̋���
	void AsGravityBehave();

	//�t�B�j�b�V���U�����̋���
	void AsFinishBehave();

private:
	//���͒e�̏��
	enum enGravityBulletState
	{
		enBullet,//���˒�
		enExplode,//���������u��
		enGravity,//�����񂹂Ȃ�����͂̃K���L�𕂂����Ă���
		enFinish,//�t�B�j�b�V���U����
	};

	enGravityBulletState m_gravityBulletState = enBullet;//���͒e�̏��
	
	Vector3 m_oldPosition;//�O�t���[���̍��W

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//���͒e�̃��f��

	MyCapsuleCollider m_bulletCollider;//�v���C���[�Ƃ̓����蔻��p�̃J�v�Z����̓����蔻��

	BackGround* m_stageModel = nullptr;//�����蔻��p�̃X�e�[�W�̃N���X

	GameScene* m_gameScene = nullptr;

	const float m_velocity = 30.0f;//�e��

	std::vector<Debris*> m_controlDebrisVector;//�R���g���[������K���L���i�[����R���e�i

	int m_gravityTimeCount = 0;//���͂ōS�����鎞�Ԃ̃J�E���g

	const float m_bacuumSpeed = 6.5f;//�v���C���[�������񂹂�X�s�[�h�B

	prefab::CEffect* m_effect = nullptr;	//�G�t�F�N�g
	prefab::CEffect* m_effect2 = nullptr;	//�G�t�F�N�g
	prefab::CEffect* m_inflateEffect = nullptr;	//�c���G�t�F�N�g
	prefab::CEffect* m_wearingEffect = nullptr;	//���ˎ��ɒe���Z���G�t�F�N�g

	float angle = 0.0f;//���f���̊p�x

public:
	Vector3 m_position = { 0.0f,0.0f,0.0f };//���W

	Player* m_parent = nullptr;//�e�̃v���C���[(�������Ɏg�p)

	Vector3 m_moveDirection = { 0.0f,0.0f,0.0f };//�ړ��������
};

