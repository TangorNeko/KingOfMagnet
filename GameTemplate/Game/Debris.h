#pragma once
#include "MyCapsuleCollider.h"

class Player;
class BackGround;
class GameScene;

class Debris : public IGameObject
{
	~Debris();
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
	 * @brief �e�Ƃ��Ĕ��˂��ꂽ�K���L���v���C���[�Ƀq�b�g������
	 * @param player �q�b�g�����v���C���[
	*/
	void PlayerHitAsBullet(Player* player);

	/**
	 * @brief �e�Ƃ��Ĕ��˂��ꂽ�K���L���X�e�[�W�Ƀq�b�g������
	 * @param crossPoint �q�b�g�������W
	*/
	void StageHitAsBullet(const Vector3& crossPoint);

	/**
	 * @brief �v���C���[�ɕێ�����Ă��鎞�̋���
	*/
	void AsHoldBehave();

	/**
	 * @brief �����ɓ���������̋���
	*/
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

	//�K���L�̌`
	enum enDebrisShape
	{
		enScrap,//��
		enSword,//��
		//�������瑼�̃A�C�e��(���@�̏�)�Ȃǒǉ����Ă���?
		enSpecialCharger,
	};

	/**
	 * @brief �K���L�̍��W��ݒ�
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief �K���L�̍��W���擾
	 * @return ���W
	*/
	const Vector3& GetPosition() const { return m_position; }

	/**
	 * @brief �K���L�̏�Ԃ�ݒ�
	 * @param state �K���L�̏��
	*/
	void SetDebrisState(enDebrisState state) { m_debrisState = state; }

	/**
	 * @brief �K���L�̏�Ԃ��擾
	 * @return �K���L�̏��
	*/
	enDebrisState GetDebrisState() { return m_debrisState; }

	/**
	 * @brief �K���L�̌`���ݒ�
	 * @param shape �K���L�̌`��
	*/
	void SetDebrisShape(enDebrisShape shape) { m_debrisShape = shape; }

	/**
	 * @brief �K���L�����L����v���C���[���擾
	 * @param player �v���C���[
	*/
	void SetParent(Player* player) { m_parent = player; }

	/**
	 * @brief �K���L�̈ړ�������ݒ�
	 * @param direction �ړ����� 
	*/
	void SetMoveDirection(const Vector3& direction) 
	{
		m_moveDirection = direction;
		m_moveDirection.Normalize();
	}

	/**
	 * @brief �K���L���n�ʂɑ��݂��邩�̃t���O��ݒ�
	 * @param flag �t���O
	*/
	void SetOnGroundFlag(bool flag) { m_isOnGround = flag; }
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//�K���L�̃��f��
	Vector3 m_position;										//���W
	Vector3 m_oldPosition;									//�O�t���[���̍��W
	Vector3 m_moveDirection = { 1.0f,0.0f,0.0f };			//�ړ��������
	enDebrisState m_debrisState = enDrop;					//�K���L�̏��
	enDebrisShape m_debrisShape = enScrap;					//�K���L�̌`��
	bool m_isOnGround = false;								//�n�ʂɂ��Ă��邩�ǂ���
	MyCapsuleCollider m_bulletCollider;						//�v���C���[�Ƃ̓����蔻��p�̃J�v�Z����̓����蔻��
	BackGround* m_stageModel = nullptr;						//�����蔻��p�̃X�e�[�W�̃N���X
	GameScene* m_gameScene = nullptr;						//�Q�[���V�[��
	//�X�y�V�����`���[�W���[�p
	int m_specialChargeCount = 0;							//�X�y�V�����Q�[�W�𑝂₷�܂ł̃J�E���g
	Player* m_parent = nullptr;								//�e�̃v���C���[(�z�[���h���A���ˎ��Ɏg�p)
};

