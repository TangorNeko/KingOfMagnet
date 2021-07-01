#pragma once
#include "MyCapsuleCollider.h"
#include "TriangleCollider.h"

class BackGround;
class Debris;
class Bomb;
class MobiusGauge;
class GameScene;

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

	//�o�[�X�g��Ԃɐ؂�ւ���
	void TryChangeStatusBurst();

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


	//�A�j���[�V�����̏�ԍX�V
	void UpdateState();

	//�A�j���[�V�������Đ�
	void AnimationSelect();

	//���ː�̌v�Z�B
	bool GetShootPoint(Vector3& crossPoint);

	//�t�@�C�i���J����
	void FinalHit();

	//���C�g
	prefab::CSpotLight* m_spotLight = nullptr;


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

public://���t�@�N�^�����O���ɍ��ꂽ�֐�
	void SetDamageEffectFront(const Vector3& front)
	{
		m_damageEffectFront = front;
	}

	void SetKnockBackFlag(bool flag)
	{
		m_isKnockBack = flag;
	}

	bool GetGravityAttackFlag()
	{
		return m_isGravityBulletAttack;
	}

	void SetEnemy(Player* enemy)
	{
		m_enemy = enemy;
	}

	const Vector3& GetMagPosition()
	{
		return m_magPosition;
	}

	int GetPlayerNum()
	{
		return m_playerNum;
	}

	void SetPlayerNum(int playerNum)
	{
		m_playerNum = playerNum;
	}

	int GetMagPower()
	{
		return m_magPower;
	}

	void SetMagPower(int magPower)
	{
		m_magPower = magPower;
	}

	bool IsBurst()
	{
		return m_isBurst;
	}

	int GetHP()
	{
		return m_hp;
	}

	bool IsBulletHitCollider(const MyCapsuleCollider& bulletCollider)
	{
		return m_collider.isHitCapsule(bulletCollider);
	}

	void ExecuteCharacon(Vector3 moveSpeed)
	{
		m_charaCon.Execute(moveSpeed, 1.0f);
	}

	void ResetFall()
	{
		m_fallLoop = 0;
	}

	void SetSensitivity(float sensitivity)
	{
		m_sensitivity = sensitivity;
	}

public:

	//�A�j���[�V�����̐�
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,
		enAnimationClip_Fall,
		enAnimationClip_SpecialAttack,
		enAnimationClip_Burst,
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
		enStatus_Burst,		//�o�[�X�g���
		enStatus_Hit,		//��e���		
		enStatus_Num,		//��Ԃ̐��B
	};

public:
	//�K���L�A���e�̑���n
	std::vector<Debris*> m_holdDebrisVector;//�ێ����Ă���K���L���i�[�����R���e�i
	float m_holdDebrisRotateDeg = 0;//�ێ����Ă���K���L�̉�]�p�x

	std::vector<Bomb*> m_holdBombVector;//�ێ����Ă��锚�e���i�[�����R���e�i
	int m_selectBombNo = 0;//�I�����Ă��锚�e�̔ԍ�

	//�t�@�C�i���q�b�g�֘A
	bool m_Lose = false;//���������ǂ���
	bool m_WinAnimOn = false;//���҃A�j���[�V�������J�n����
	int m_LoseCameraLoop = 0;//�t�@�C�i���q�b�g�J�����̃��[�v�J�E���g
	bool m_FirstTime = true;//�ŏ��̈�x����
	int m_loserNum = 0;//�s�҂̃v���C���[�ԍ�
	int m_LastCameraStatus = 0;//��ԑJ�ڔԍ�
	float m_coef = 0.0f;//�x�N�^�[�Ɋ|����l(coefficient)
	Vector3 m_LastFrontDir;//�L�������Ō�Ɍ���������
	Vector3 m_winnerVec;//�s�҂��珟�҂Ɍ������x�N�g��
	Vector3 m_winnerWaistPos;//���҂̍��̈ʒu

	prefab::CFontRender* m_bulletNumFont = nullptr;//�c�e��
	prefab::CFontRender* m_bulletNumFont2 = nullptr;//�c�e��

	Vector3 m_position = { 0.0f,0.0f,0.0f }; //�L�����N�^�[�̍��W
	Quaternion m_rot;//�L�����N�^�[�̉�]
	Vector3 m_scale = { 0.8f, 0.8f, 0.8f };//�L�����N�^�[�̊g�嗦 **�萔��**
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//�L�����N�^�[�̈ړ����x
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//�L�����N�^�[�̌���
	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };//�J�����ւ̌���

	Vector3 m_front;//�J�����̑O����
	Vector3 right;//�J�����̉E����
	Vector3 cameraPos;//�J�����̃|�W�V����
	Quaternion qRotY;
	float n;//����
private:
	float m_sensitivity = 2.0f;//���_���x
	Vector3 m_cameraPos;
	Vector3 m_targetPos = { 0.0f,0.0f,0.0f }; //  **�萔��**
	float m_gain = 10;//�J�����ƃ^�[�Q�b�g�Ƃ̋���	
	float m_addY = 0.0f;
	int m_cameraLoopCount = 0;
	bool m_hpBarRedFlag = false;

	int m_fallLoop = 0;//��������p�̃��[�v�J�E���g
	float m_characterSpeed = 6.0;//�L�����N�^�[�̈ړ����x

	bool m_isLock = false; //���b�N���Ă��邩�B

	GameScene* m_gameScene = nullptr;

	BackGround* m_stageModel;//�w�i�̃��f��(�����蔻��p)
	CharacterController m_charaCon;//�v���C���[�̃L�����R��
	TriangleCollider m_triCollider[2];//�P���ȎO�p�`�̓����蔻��(���ː�̔���Ɏg��)
	MyCapsuleCollider m_collider;//�J�v�Z����̓����蔻��(�e�̓����蔻��Ɏg��)


	prefab::CSkinModelRender* m_skinModelRender = nullptr; //�L�����N�^�[�̃��f��

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


	AnimationClip animationClips[enAnimationClip_num];//�A�j���[�V�����N���b�v
	EnStatus m_animStatus = enStatus_Idle;	//���݂̏�ԁB

	Player* m_enemy = nullptr; //�G



	int m_specialAttackGauge = 0;//�K�E�Z�̃Q�[�W
	int m_oldSpecialAttackGauge = 0;
	bool m_isGravityBulletAttack = false;//���͂̕K�E�Z�̍U���^�C�~���O���w������p�ϐ��B

	//�m�b�N�o�b�N�֘A
	bool m_isKnockBack = false;
	int m_isknockBackCount = 0;

	//HP�o�[
	prefab::CSpriteRender* m_HPBarSpriteRender = nullptr;
	prefab::CSpriteRender* m_HPBarDarkSpriteRender = nullptr;
	prefab::CSpriteRender* m_HPBarRedSpriteRender = nullptr;

	//���r�E�X�Q�[�W
	MobiusGauge* m_mobiusGauge = nullptr;




	//�_���[�W�G�t�F�N�g�֘A
	Vector3 m_damageEffectFront = {0.0f,0.0f,0.0f};

	//�˗́E���̓G�t�F�N�g�֘A
	prefab::CEffect* m_magEffect[2] = { nullptr,nullptr };		//2�̃G�t�F�N�g��A���ōĐ���������B
	int m_magEffectCallCount = 41;		//���̓G�t�F�N�g���ĂԂ܂ł̎���

	//�˗́E���̓o�[�X�g �G�t�F�N�g�֘A
	prefab::CEffect* m_burstEffect = nullptr;

	//�_���[�W�G�t�F�N�g
	prefab::CEffect* m_hitEffect = nullptr;

	//�K�E�Z�̃G�t�F�N�g
	prefab::CEffect* m_SPEffect = nullptr;

	//�K�E�Q�[�W���ő�܂ŗ��܂������̃G�t�F�N�g
	prefab::CEffect* m_SPGaugeMaxEffect = nullptr;

	//�K�E�Z����O���̂Ƃ��̃G�t�F�N�g
	prefab::CEffect* m_SPFirstEffectRed = nullptr;
	prefab::CEffect* m_SPFirstEffectBlue = nullptr;

	//����
	int m_footstepsTimer = 0;

	//�K�E�Z�Q�[�W���ǂꂾ�����܂��Ă��邩��\��
	prefab::CFontRender* m_ChargeSPFontRender = nullptr;

	//�����\��
	prefab::CSpriteRender* m_winnerSprite1 = nullptr;
	prefab::CSpriteRender* m_winnerSprite2 = nullptr;
};



