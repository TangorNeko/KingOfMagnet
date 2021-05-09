#pragma once
#include "MyCapsuleCollider.h"
#include "TriangleCollider.h"

class BackGround;
class Debris;

class Player : public IGameObject
{
	~Player();
	bool Start() override;
	void Update() override;

	//�̗́A���r�E�X�Q�[�W�̕\��
	void DisplayStatus();

	//�ړ�
	void Move();

	//�U��
	void Attack();

	//�K�E�Z
	void SpecialAttack();

	//�ێ����Ă���K���L�𕂗V������B
	void HoldDebris();

	//���̓o�[�X�g���g�p���Ă��Ȃ����̒ʏ�̓���
	void MagneticBehavior();

	//���̓o�[�X�g
	void MagneticBurst();

	//���͂̕ω�
	void ChangeMagnetPower();

	//�J�����̈ړ�
	void Camera();

	//�����蔻��
	void Collision();

	//�U����Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusAttack();

	//�����Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusRun();

	//������Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusWalk();

	//������Ԃɐ؂�ւ���
	void TryChangeStatusFall();

	//�ҋ@��Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusIdle();

	//�A�j���[�V�����̏�ԍX�V
	void UpdateState();

	//�A�j���[�V�������Đ�
	void AnimationSelect();

	//���ː�̌v�Z�B
	bool GetShootPoint(Vector3& crossPoint);

public:

	//�����̗̑͂Ƀ_���[�W��^����
	void Damage(int damage);

	//����������
	void Win();

	//�s�k������
	void Lose();

	//�K�E�Z�Q�[�W���`���[�W����B
	void ChargeSpecialAttackGauge(int charge);

	//�m�b�N�o�b�N������B
	void KnockBack();

public:
	Vector3 m_position = { 0.0f,0.0f,0.0f }; //�L�����N�^�[�̍��W
	Quaternion rot;//�L�����N�^�[�̉�]
	Vector3 m_scale = { 0.8f, 0.8f, 0.8f };//�L�����N�^�[�̊g�嗦
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//�L�����N�^�[�̈ړ����x
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//�L�����N�^�[�̌���
	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };//�J�����ւ̌���

	Vector3 front;//�J�����̑O����
	Vector3 right;//�J�����̉E����
	Vector3 cameraPos;//�J�����̃|�W�V����
	Quaternion qRotY;
	float n;//����
	float angle;//�A�[�N�R�T�C��

	int m_fallLoop = 0;//��������p�̃��[�v�J�E���g
	float m_characterSpeed = 6.0;//�L�����N�^�[�̈ړ����x

	bool m_isLock = false; //���b�N���Ă��邩�B

	BackGround* m_stageModel;//�w�i�̃��f��(�����蔻��p)
	CharacterController m_charaCon;//�v���C���[�̃L�����R��
	TriangleCollider m_triCollider[2];//�P���ȎO�p�`�̓����蔻��(���ː�̔���Ɏg��)
	MyCapsuleCollider m_collider;//�J�v�Z����̓����蔻��(�e�̓����蔻��Ɏg��)

	prefab::CSkinModelRender* m_skinModelRender = nullptr; //�L�����N�^�[�̃��f��
	prefab::CFontRender* m_statusFontRender = nullptr;//�X�e�[�^�X�\���p�̃t�H���g
	prefab::CSpriteRender* m_resultSpriteRender = nullptr; //���s���̃X�v���C�g
	prefab::CSpriteRender* m_crosshairRender = nullptr; //�Ə��̃X�v���C�g

	int m_playerNum = -1;//�v���C���[�̔ԍ� 1P(0)�A2P(1)
	int m_magPower;//���́A���͏��(-1)�A�˗�(1)
	float m_charge = 1000;//���̓Q�[�W�̌��݂̗�
	int m_hp = 1000;//�̗�
	bool m_isBurst = false;//�o�[�X�g���Ă��邩�B
	int m_burstCount = 0;//�o�[�X�g�̎�������
	bool m_isAttacking = false; //�U���𔭎˂��Ă��邩�B
	int m_attackCount = 0;//�U���̌��ňړ����x�������Ă��鎞�ԁB
	bool m_isSteal = false;//���̈��̓o�[�X�g���ɂ��łɓG�̒e��D������

	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };//���͂��o�Ă��錴�_

	std::vector<Debris*> m_holdDebrisVector;//�ێ����Ă���K���L���i�[�����R���e�i
	float m_holdDebrisRotateDeg = 0;//�ێ����Ă���K���L�̉�]�p�x

	//�A�j���[�V�����̐�
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,
		enAnimationClip_Fall,
		enAnimationClip_num,  //�񋓓��Ŏg���v�f�̐���\���_�~�[
	};

	//��Ԃ̐�
	enum EnStatus {
		enStatus_Attack,	//�U�����
		enStatus_Run,		//������
		enStatus_Idle,		//�ҋ@���
		enStatus_Walk,		//�������
		enStatus_Move,		//�ړ��A�N�V�������		
		enStatus_Fall,		//�������
		enStatus_Num,		//��Ԃ̐��B
	};

	AnimationClip animationClips[enAnimationClip_num];//�A�j���[�V�����N���b�v
	EnStatus m_animStatus = enStatus_Idle;	//���݂̏�ԁB

	Player* m_enemy = nullptr; //�G

	int m_specialAttackGauge = 0;//�K�E�Z�̃Q�[�W
	bool m_isGravityBulletAttack = false;//���͂̕K�E�Z�̍U���^�C�~���O���w������p�ϐ��B

	//�m�b�N�o�b�N�֘A
	bool m_isKnockBack = false;
	int m_isknockBackCount = 0;

	//HP�o�[
	prefab::CSpriteRender* m_HPBarSpriteRender = nullptr;
	prefab::CSpriteRender* m_HPBarDarkSpriteRender = nullptr;

	//�������邩�ǂ���
	bool m_canMove = false;

};

