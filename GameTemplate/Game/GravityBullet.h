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

	/**
	 * @brief ���˂���Ă���r���̋���
	*/
	void AsBulletBehave();

	/**
	 * @brief ���e�����u�Ԃ̋���
	*/
	void AsExplodeBehave();

	/**
	 * @brief ���e������̋���
	*/
	void AsGravityBehave();

	/**
	 * @brief �t�B�j�b�V���U�����̋���
	*/
	void AsFinishBehave();

public:

	/**
	 * @brief ���͒e�̍��W��ݒ�
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief ���͒e�̍��W���擾
	 * @return ���W
	*/
	Vector3 GetPosition() { return m_position; }

	/**
	 * @brief ���͒e�̈ړ�������ݒ�
	 * @param moveDirection �ړ�����
	*/
	void SetMoveDirection(const Vector3& moveDirection) { m_moveDirection = moveDirection; }

	/**
	 * @brief ���͒e�𔭎˂����v���C���[��ݒ�
	 * @param parent �v���C���[
	*/
	void SetParent(Player* parent) { m_parent = parent; }


private:
	//���͒e�̏��
	enum enGravityBulletState
	{
		enBullet,//���˒�
		enExplode,//���������u��
		enGravity,//�����񂹂Ȃ�����͂̃K���L�𕂂����Ă���
		enFinish,//�t�B�j�b�V���U����
	};

	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//���͒e�̃��f��
	prefab::CEffect* m_gravityEffect = nullptr;				//�G�t�F�N�g
	prefab::CEffect* m_gravityEffect2 = nullptr;			//�G�t�F�N�g
	prefab::CEffect* m_inflateEffect = nullptr;				//�c���G�t�F�N�g
	prefab::CEffect* m_wearingEffect = nullptr;				//���ˎ��ɒe���Z���G�t�F�N�g
	Vector3 m_position = { 0.0f,0.0f,0.0f };				//���W
	Vector3 m_oldPosition;									//�O�t���[���̍��W
	Vector3 m_moveDirection = { 0.0f,0.0f,0.0f };			//�ړ��������
	float angle = 0.0f;										//���f���̊p�x
	enGravityBulletState m_gravityBulletState = enBullet;	//���͒e�̏��
	std::vector<Debris*> m_controlDebrisVector;				//�R���g���[������K���L���i�[����R���e�i
	int m_gravityTimeCount = 0;								//���͂ōS�����鎞�Ԃ̃J�E���g
	MyCapsuleCollider m_bulletCollider;						//�v���C���[�Ƃ̓����蔻��p�̃J�v�Z����̓����蔻��
	BackGround* m_stageModel = nullptr;						//�����蔻��p�̃X�e�[�W�̃N���X
	GameScene* m_gameScene = nullptr;						//�Q�[���V�[��
	Player* m_parent = nullptr;								//�e�̃v���C���[(�������Ɏg�p)
};

