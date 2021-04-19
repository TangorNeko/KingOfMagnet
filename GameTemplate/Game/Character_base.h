#pragma once
#include "MyCapsuleCollider.h"
class BackGround;

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

	//�v���C���[�ɂ����鎥�͂̉e��
	void PlayerMagneticMove();

public:
	virtual ~Character_base();
	Quaternion rot;//�L�����N�^�[�̉�]
	Vector3 m_position = { 0.0f,0.0f,0.0f };//�L�����N�^�[�̍��W
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//�L�����N�^�[�̈ړ����x
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//�L�����N�^�[�̌���
	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };
	Vector3 Scale = { 0.3,0.3,0.3 };//�L�����N�^�[�̊g�嗦
	Vector3 m_fallScale = { 0.66f,0.66f,0.66f };
	Vector3 front;//�J�����̑O����
	Vector3 right;//�J�����̉E����
	Vector3 cameraPos;//�J�����̃|�W�V����
	Quaternion qRotY;
	float n;//����
	float angle;//�A�[�N�R�T�C��
	
	Vector3 m_to_enemy;
	float m_angle_with_enemy;
	Vector3 m_position_with_enemy;

	Vector3 m_repulsionSpeed;

	int m_magStatediff;

	prefab::CSkinModelRender* m_weaponModel = nullptr;//���������

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//�L�����N�^�[�̃��f��
	prefab::CPointLight* m_pointLight = nullptr;//�`���[�W�m�F�p�̃|�C���g���C�g(TODO:�ォ��G�t�F�N�g�ɍ����ւ��\��)
	prefab::CFontRender* m_fontRender = nullptr;//�̗́A�`���[�W�A���͓��m�F�p�̃t�H���g(TODO:�ォ��UI�X�v���C�g�ɍ����ւ��\��)
	prefab::CSpriteRender* m_spriteRender = nullptr;//�����������͔s�k���ɕ\������X�v���C�g
	prefab::CSpriteRender* m_crosshairRender = nullptr;//�Ə��̃X�v���C�g
	BackGround* m_stageModel;
	CharacterController m_charaCon;//�v���C���[�̃L�����R��
	//TriangleCollider m_collider;//�P���ȎO�p�`�̓����蔻��(TODO:�����Ƃ������肵�������蔻�����肽��)
	MyCapsuleCollider m_collider;
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
	int m_loop = 0;//��������p�̃��[�v�J�E���g
	bool m_isMagPowerIncreasing = false;//���͂��������Ă��邩�������Ă��邩
	bool m_isSceneStop = false;//�����邩�����Ȃ���
	float m_Speed = 6.0;
	//�}�V���K�����������Ƃ�
	bool m_MachinegunHave=false;
	bool m_MachinegunDelete = false;	
	int m_Machinegun_loopcount = 0;
	int m_Machinegun_deletetime = 500;
	int m_Machinegun_bulletNum = 100;	
	
	//���݂̎��͂̏�Ԃ��擾
	int GetMagPower()const { return m_magPower; }

	//�����̗̑͂Ƀ_���[�W��^����
	void Damage(int damage);

	//����������
	void Win();

	//�s�k������
	void Lose();

	//�˗͏����߂��Ƃ�
	//void NearRepulsionFloor();
	Vector3 m_Yspeed = { 0.0f,0.0f,0.0f };
	//�G�̃C���X�^���X
	Character_base* m_enemy = nullptr;

	//�T�C�R�L�l�V�X�����Ă邩(���@�g������)
	bool m_Psycho_on = false;

	int m_anim_num = 0;

	int m_timerAccele = 1;
	int m_AcceleLoop = 0;

	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,	
		enAnimationClip_Fall,
		enAnimationClip_Gun_Run,
		enAnimationClip_Gun_Idle,
		enAnimationClip_Gun_Walk,	
		enAnimationClip_num,  //�񋓓��Ŏg���v�f�̐���\���_�~�[
	};
	enum EnStatus {
		enStatus_Attack,	//�U�����
		enStatus_Run,		//������
		enStatus_Idle,		//�ҋ@���
		enStatus_Walk,		//�������
		enStatus_Move,		//�ړ��A�N�V�������		
		enStatus_Fall,		//�������
		enStatus_Num,		//��Ԃ̐��B
	};
	AnimationClip animationClips[enAnimationClip_num];
	EnStatus status = enStatus_Idle;	//���݂̏�ԁB

	int m_hitcount=0;
	int m_oldhitcount=0;
	int m_damage = 0;
	prefab::CFontRender* m_fontRenderDamage= nullptr;
	Vector3 damagePos;
	Vector2 screenDamagePos;
	std::wstring damageText;//�_���[�W�ʕ\��
	//void DamageDisplay();
};

