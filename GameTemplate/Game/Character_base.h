#pragma once
#include "TriangleCollider.h"
class Character_base : public IGameObject
{
public:
	//bool Start() override;	
	//�̗́A�`���[�W�A���͂̏�ԓ��̕\��
	virtual void DisplayStatus()=0;

	//�ړ��A�N�V����
	virtual void MoveAction() = 0;

	//�ʏ�U��
	virtual void NormalAttack() = 0;	

protected:
	//���͂̒���I�ȕω�
	void ChangeMagnetPower();

	//�`���[�W
	virtual void Charge() = 0;

	//�G�Ɍ��������蔻������
	void Collision();

	//�J�����̈ړ�
	void Camera();

	//�ŗL�U��
	virtual void SpecialAttack()=0;

	//�A�j���[�V����
	virtual void AnimationSelect() = 0;

public:
	virtual ~Character_base();
	Quaternion rot;//�L�����N�^�[�̉�]
	Vector3 m_position = { 0.0f,0.0f,0.0f };//�L�����N�^�[�̍��W
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//�L�����N�^�[�̈ړ����x
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//�L�����N�^�[�̌���
	Vector3 m_toCamera = { 0.0f,100.0f,-100.0f };//�L�����N�^�[����J�����ւ̃x�N�g��
	Vector3 Scale = { 0.3,0.3,0.3 };//�L�����N�^�[�̊g�嗦
	Vector3 front;//�J�����̑O����
	Vector3 right;//�J�����̉E����
	float n;//����
	float angle;//�A�[�N�R�T�C��

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//�L�����N�^�[�̃��f��
	prefab::CPointLight* m_pointLight = nullptr;//�`���[�W�m�F�p�̃|�C���g���C�g(TODO:�ォ��G�t�F�N�g�ɍ����ւ��\��)
	prefab::CFontRender* m_fontRender = nullptr;//�̗́A�`���[�W�A���͓��m�F�p�̃t�H���g(TODO:�ォ��UI�X�v���C�g�ɍ����ւ��\��)
	prefab::CSpriteRender* m_spriteRender = nullptr;//�����������͔s�k���ɕ\������X�v���C�g
	CharacterController m_charaCon;//�v���C���[�̃L�����R��
	TriangleCollider m_collider;//�P���ȎO�p�`�̓����蔻��(TODO:�����Ƃ������肵�������蔻�����肽��)
	int m_playerNum = -1;//�v���C���[�̔ԍ� 1P(0)�A2P(1)
	int m_magPower;//���́A�Ȃ�(0)�A���͏��(-1,-2)�A�˗͏��(1,2)
	int m_normalAttackCount = 0;//�ʏ�U���̃^�C�}�[
	int m_moveActionCount = 0;//�ړ��A�N�V�����Ɏg���J�E���g
	int m_hp = 1000;//�̗�
	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };//���͂��o�Ă��錴�_
	float m_charge = 0;//�`���[�W
	int m_chargelevel = 1;//�`���[�W���x��
	float m_deg = 0;//�L�����̌����̊p�x
	bool m_isLock = false;//���b�N�I�����Ă��邩�B
	int m_timer = 0;//���͕ω��p�̃^�C�}�[
	bool m_isMagPowerIncreasing = false;//���͂��������Ă��邩�������Ă��邩
	bool m_isSceneStop = false;//�����邩�����Ȃ���
	//���݂̎��͂̏�Ԃ��擾
	int GetMagPower()const { return m_magPower; }

	//�����̗̑͂Ƀ_���[�W��^����
	void Damage(int damage);

	//����������
	void Win();

	//�s�k������
	void Lose();

	//�G�̃C���X�^���X
	Character_base* m_enemy = nullptr;

	

	int m_anim_num = 0;



};

