#pragma once
#include "MyCapsuleCollider.h"

class Player;
class BackGround;
class GameScene;

class Bomb : public IGameObject
{
	~Bomb() override;
	bool Start() override;
	void Update() override;

	/**
	 * @brief �n�ʂɗ����Ă��鎞�̋���
	*/
	void AsDropBehave();

	/**
	 * @brief �e�Ƃ��Ĕ��˂���Ă��鎞�̋���
	*/
	void AsBulletBehave();

	/**
	 * @brief �v���C���[�ɕێ�����Ă��鎞�̋���
	*/
	void AsHoldBehave();

	/**
	 * @brief �����ɓ���������̋���
	*/
	void AsPopBehave();
public:
	//���e�̏��
	enum enBombState
	{
		enDrop,//�����Ă���B
		enBullet,//�e�Ƃ��Ĕ��˂���Ă���
		enHold,//�v���C���[���ێ����Ă���
		enPop,//�����ɓ��������@�������O
	};

	//���e�̎��
	enum enBombShape
	{
		enGrenade,//���e
		enFlashGrenade,//�M���e
		enIncendiaryGrenade,//�ĈΒe
	};

	/**
	 * @brief ���e�̍��W��ݒ�
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief ���e�̍��W���擾
	 * @return 
	*/
	const Vector3& GetPosition() const { return m_position; }

	/**
	 * @brief ���e�̏�Ԃ�ݒ�
	 * @param state ���e�̏��
	*/
	void SetBombState(enBombState state) { m_bombState = state; }

	/**
	 * @brief ���e�̎�ނ�ݒ�
	 * @param shape ���
	*/
	void SetBombShape(enBombShape shape) { m_bombShape = shape; }

	/**
	 * @brief ���e�̈ړ�������ݒ�
	 * @param direction �ړ�����
	*/
	void SetMoveDirection(const Vector3& direction)
	{
		m_moveDirection = direction;
		m_moveDirection.Normalize();
	}
	
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//���e�̃��f��
	Vector3 m_position = Vector3::Zero;						//���W
	Vector3 m_oldPosition = Vector3::Zero;					//�O�t���[���̍��W
	Vector3 m_moveDirection = { 1.0f,0.0f,0.0f };			//�ړ��������
	enBombState m_bombState = enDrop;						//���e�̏��
	enBombShape m_bombShape = enGrenade;					//���e�̎��
	bool m_isOnGround = false;								//�n�ʂɂ��Ă��邩�ǂ���
	MyCapsuleCollider m_bulletCollider;						//�v���C���[�Ƃ̓����蔻��p�̃J�v�Z����̓����蔻��
	BackGround* m_stageModel = nullptr;						//�����蔻��p�̃X�e�[�W�̃N���X
	GameScene* m_gameScene = nullptr;						//�Q�[���V�[��
	int m_explosionCount = 0;								//�����܂ł̃J�E���g
	Player* m_parent = nullptr;								//�e�̃v���C���[(�z�[���h���A���ˎ��Ɏg�p)
};

