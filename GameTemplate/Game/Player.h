#pragma once
#include "MyCapsuleCollider.h"
#include "TriangleCollider.h"

class BackGround;
class Debris;
class Bomb;
class MobiusGauge;
class SampleScene;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
private:

	//�J�n�O�J�����ړ�
	void OpeningCamera();

	//�̗́A���r�E�X�Q�[�W�̕\��
	void DisplayStatus();

	//�ړ�
	void Move();

	//�U��
	void Attack();

	//�K�E�Z
	void SpecialAttack();
	bool m_SpecialAttackOn = false;//�A�j���[�V�����p�t���O
	int m_specialShotFlag = false;
	int m_specialShotCount = 0;

	//���e�𓊂���
	void ThrowBomb();

	//�ێ����Ă���K���L�𕂗V������B
	void HoldDebris();

	//�ێ����Ă��锚�e�𕂗V������B
	void HoldBomb();

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

	//����U����Ԃɐ؂�ւ���
	void TryChangeStatusSpecialAttack();

	//�����Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusRun();

	//������Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusWalk();

	//������Ԃɐ؂�ւ���
	void TryChangeStatusFall();

	//�ҋ@��Ԃɐ؂�ւ��ł�����؂�ւ���B
	void TryChangeStatusIdle();

	//��e��Ԃɐ؂�ւ���
	void TryChangeStatusHit();
	bool m_HitOn = false;//��e�������ǂ���
	int m_Hitcount = 30;//��e�����Ƃ��ɗ����A�j���[�V�����̃t���[����

	//���S��Ԃɐ؂�ւ���
	void TryChangeStatusDeath();
	
	//������Ԃɐ؂�ւ���
	void TryChangeStatusWin();

	//�A�j���[�V�����̏�ԍX�V
	void UpdateState();

	//�A�j���[�V�������Đ�
	void AnimationSelect();

	//���ː�̌v�Z�B
	bool GetShootPoint(Vector3& crossPoint);

	//���C�g
	prefab::CSpotLight* m_spotLight = nullptr;


public:

	//�����̗̑͂Ƀ_���[�W��^����
	void Damage(int damage);

	//����������
	void Win();

	//�s�k������
	void Lose();

	//���U���g�\��
	//void ResultDisplay();
	void FinalHit();

	//�K�E�Z�Q�[�W���`���[�W����B
	void ChargeSpecialAttackGauge(int charge);

	//�m�b�N�o�b�N������B
	void KnockBack();

public:
	Vector3 m_position = { 0.0f,0.0f,0.0f }; //�L�����N�^�[�̍��W
	Quaternion m_rot;//�L�����N�^�[�̉�]
	Vector3 m_scale = { 0.8f, 0.8f, 0.8f };//�L�����N�^�[�̊g�嗦
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//�L�����N�^�[�̈ړ����x
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//�L�����N�^�[�̌���
	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };//�J�����ւ̌���

	Vector3 m_front;//�J�����̑O����
	Vector3 right;//�J�����̉E����
	Vector3 cameraPos;//�J�����̃|�W�V����
	Quaternion qRotY;
	float n;//����
	float angle;//�A�[�N�R�T�C��

	Vector3 m_cameraPos;
	Vector3 m_targetPos = { 0.0f,0.0f,0.0f };
	float gain = 10;//�J�����ƃ^�[�Q�b�g�Ƃ̋���	
	float m_addY = 0.0f;
	int m_cameraLoopCount = 0;
	bool m_opning = true;

	int m_fallLoop = 0;//��������p�̃��[�v�J�E���g
	float m_characterSpeed = 6.0;//�L�����N�^�[�̈ړ����x

	bool m_isLock = false; //���b�N���Ă��邩�B

	SampleScene* m_gameScene = nullptr;

	BackGround* m_stageModel;//�w�i�̃��f��(�����蔻��p)
	CharacterController m_charaCon;//�v���C���[�̃L�����R��
	TriangleCollider m_triCollider[2];//�P���ȎO�p�`�̓����蔻��(���ː�̔���Ɏg��)
	MyCapsuleCollider m_collider;//�J�v�Z����̓����蔻��(�e�̓����蔻��Ɏg��)

	prefab::CSkinModelRender* m_skinModelRender = nullptr; //�L�����N�^�[�̃��f��
	prefab::CFontRender* m_bulletNumber = nullptr;//�c�e��
	prefab::CFontRender* m_resultFontRender = nullptr;//���U���g�\���p�̃t�H���g

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

	std::vector<Bomb*> m_holdBombVector;//�ێ����Ă��锚�e���i�[�����R���e�i
	int m_selectBombNo = 0;//�I�����Ă��锚�e�̔ԍ�

	//�A�j���[�V�����̐�
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,
		enAnimationClip_Fall,
		enAnimationClip_SpecialAttack,
		enAnimationClip_Hit,
		enAnimationClip_Death,
		enAnimationClip_Winner,
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
		enStatus_SpecialAttack,//�K�E�Z���
		enStatus_Hit,		//��e���
		enStatus_Death,		//���S���
		enStatus_Winner,		//�������
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

	//���r�E�X�Q�[�W
	MobiusGauge* m_mobiusGauge = nullptr;

	//���U���g�֘A
	Vector2 m_resultPos = { -1200.0f,100.0f };
	bool m_resultFirstTime = true;
	bool m_displayOff = false;
	float m_AttackNum = 0;//�U����	
	float m_TakeAttackNum = 0; //�U�����󂯂���
	float m_HitRate = 0;//������
	int m_BurstNum = 0;//�o�[�X�g��
	int m_StealNum = 0;//�G�̒e��D������
	int m_LandingNum = 0;//��������
	int m_ReceivedDamage = 0;//�󂯂��_���[�W
	int m_SaveSP = 0;//���܂����K�E�Z�|�C���g

	//�t�@�C�i���q�b�g�֘A
	bool m_Lose = false;
	bool m_LoseCameraFlag = true;
	bool m_doryInOn = true;
	bool m_WinAnimOn = false;
	int m_LoseCameraLoop = 0;
	bool m_FirstTime = true;
	int m_winnerNum = 0;
	int m_loserNum = 0;
	int m_LastCameraStatus = 0;
	float m_coef = 0.0f;
	Vector3 m_LastFront;

	Vector3 m_enemyWaistPos;//�_���[�W�G�t�F�N�g�֘A
	Vector3 m_damegeEffectFront = {0.0f,0.0f,0.0f};

	//�˗́E���̓G�t�F�N�g�֘A
	prefab::CEffect* m_magEffect[3] = { nullptr,nullptr,nullptr };		//�O�̃G�t�F�N�g��A���ōĐ���������B
	int m_magEffectCallCount = 60;		//���̓G�t�F�N�g���ĂԂ܂ł̎���

	//�˗́E���̓o�[�X�g �G�t�F�N�g�֘A
	prefab::CEffect* m_burstEffect = nullptr;

	//�_���[�W�G�t�F�N�g
	prefab::CEffect* m_hitEffect = nullptr;

	//�K�E�Z�̃G�t�F�N�g
	prefab::CEffect* m_SPEffect = nullptr;
};


