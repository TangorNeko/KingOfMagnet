#pragma once
#include "TriangleCollider.h"

class ShowModel : public IGameObject
{
	~ShowModel();
	bool Start() override;
	void Update()override;


	void ChangeMagnetPower();
	void Collision();
	void DisplayStatus();
	void MoveAction();
	void NormalAttack();
	void Charge();
	void SpecialAttack();
	void Camera();
	void Floor();

public:
	Quaternion rot;
	Vector3 m_position = { 0.0f,0.0f,0.0f };
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };
	Vector3 m_toCamera = { 0.0f,100.0f,-100.0f };
	Vector3 Scale = { 0.6,0.6,0.6 };	

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	prefab::CSkinModelRender* floor_skinModelRender = nullptr;
	prefab::CPointLight* m_pointLight = nullptr;
	prefab::CFontRender* m_fontRender = nullptr;
	prefab::CSpriteRender* m_spriteRender = nullptr;
	CharacterController m_charaCon;
	TriangleCollider m_collider;
	int m_playerNum = -1;//�v���C���[�̔ԍ� 1P(0)�A2P(1)
	int m_magPower;//���́A�Ȃ�(0)�A���͏��(-1,-2)�A�˗͏��(1,2)
	int m_normalAttackCount = 0;
	int m_moveActionCount = 0;//�ړ��A�N�V�����Ɏg���J�E���g
	int m_hp = 1000;//�̗�
	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };//���͂��o�Ă��錴�_
	float m_charge = 0;//�`���[�W
	float m_deg = 0;//�L�����̌����̊p�x
	bool m_isLock = false;//���b�N�I�����Ă��邩�B
	int m_timer = 0;//���͕ω��p�̃^�C�}�[
	bool m_isMagPowerIncreasing = false;//���͂��������Ă��邩�������Ă��邩
	bool m_isSceneStop = false;//�����邩�����Ȃ���
public:
	int GetMagPower() { return m_magPower; }
	void Damage(int damage);
	void Win();
	void Lose();
	ShowModel* m_enemy = nullptr;
};
