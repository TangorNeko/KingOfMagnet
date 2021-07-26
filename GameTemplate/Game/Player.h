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
	~Player() override;
private:
	bool Start() override;
	void Update() override;

	/**
	 * @brief �J�n�O�J�����ړ�
	*/
	void OpeningCamera();

	/**
	 * @brief �̗́A���r�E�X�Q�[�W�̕\��
	*/
	void DisplayStatus();

	/**
	 * @brief �ړ�
	*/
	void Move();

	/**
	 * @brief �U��
	*/
	void Attack();

	/**
	 * @brief �K�E�Z
	*/
	void SpecialAttack();

	/**
	 * @brief ���e�𓊂���
	*/
	void ThrowBomb();

	/**
	 * @brief �ێ����Ă���K���L�𕂗V������B
	*/
	void HoldDebris();

	/**
	 * @brief �ێ����Ă��锚�e�𕂗V������B
	*/
	void HoldBomb();

	/**
	 * @brief ���̓o�[�X�g���g�p���Ă��Ȃ����̎��͂ɂ�铮��
	*/
	void MagneticBehavior();

	/**
	 * @brief ���̓o�[�X�g
	*/
	void MagneticBurst();

	/**
	 * @brief ���͂̕ω�
	*/
	void ChangeMagnetPower();

	/**
	 * @brief �J�����̈ړ�
	*/
	void Camera();

	/**
	 * @brief �����蔻��
	*/
	void Collision();

	/**
	 * @brief �U����Ԃɐ؂�ւ��ł�����؂�ւ���B
	*/
	void TryChangeStatusAttack();

	/**
	 * @brief ����U����Ԃɐ؂�ւ���
	*/
	void TryChangeStatusSpecialAttack();

	/**
	 * @brief �o�[�X�g��Ԃɐ؂�ւ���
	*/
	void TryChangeStatusBurst();

	/**
	 * @brief �����Ԃɐ؂�ւ��ł�����؂�ւ���B
	*/
	void TryChangeStatusRun();

	/**
	 * @brief ������Ԃɐ؂�ւ��ł�����؂�ւ���B
	*/
	void TryChangeStatusWalk();

	/**
	 * @brief ������Ԃɐ؂�ւ���
	*/
	void TryChangeStatusFall();

	/**
	 * @brief �ҋ@��Ԃɐ؂�ւ��ł�����؂�ւ���B
	*/
	void TryChangeStatusIdle();

	/**
	 * @brief ��e��Ԃɐ؂�ւ���
	*/
	void TryChangeStatusHit();

	/**
	 * @brief �A�j���[�V�����̏�ԍX�V
	*/
	void UpdateState();

	/**
	 * @brief �A�j���[�V�������Đ�
	*/
	void AnimationSelect();

	/**
	 * @brief ���ː�̌v�Z�B
	 * @param crossPoint ���߂����ː�(�Q�Ǝ󂯎��)
	 * @return ���ː悪���܂�����
	*/
	bool GetShootPoint(Vector3& crossPoint);

	/**
	 * @brief �t�@�C�i���J����
	*/
	void FinalHit();

	/**
	 * @brief �m�b�N�o�b�N������B
	*/
	void KnockBack();
public:

	//�����̗̑͂Ƀ_���[�W��^����
	void Damage(int damage);

	//����������
	void Win();

	//�s�k������
	void Lose();

	//�K�E�Z�Q�[�W���`���[�W����B
	void ChargeSpecialAttackGauge(int charge);

	/**
	 * @brief �_���[�W�G�t�F�N�g�̐��ʂ��Z�b�g����
	 * @param front ���ʂ̌���
	*/
	void SetDamageEffectFront(const Vector3& front)
	{
		m_damageEffectFront = front;
	}

	/**
	 * @brief �m�b�N�o�b�N�t���O���Z�b�g
	 * @param flag �t���O
	*/
	void SetKnockBackFlag(bool flag)
	{
		m_isKnockBack = flag;
	}

	/**
	 * @brief ���͒e�̍U���t���O���擾����
	 * @return �U���t���O
	*/
	bool GetGravityAttackFlag()
	{
		return m_isGravityBulletAttack;
	}

	/**
	 * @brief �G�v���C���[���Z�b�g����
	 * @param enemy �G�v���C���[
	*/
	void SetEnemy(Player* enemy)
	{
		m_enemy = enemy;
	}

	/**
	 * @brief ���͂��o��n�_���擾����
	 * @return ���͂��o����W
	*/
	const Vector3& GetMagPosition()
	{
		return m_magPosition;
	}

	/**
	 * @brief �v���C���[�ԍ����擾
	 * @return �v���C���[�ԍ�
	*/
	int GetPlayerNum()
	{
		return m_playerNum;
	}

	/**
	 * @brief �v���C���[�ԍ����Z�b�g����
	 * @param playerNum �v���C���[�ԍ�
	*/
	void SetPlayerNum(int playerNum)
	{
		m_playerNum = playerNum;
	}

	/**
	 * @brief ���͂̏�Ԃ��擾����
	 * @return -1:���́@1:�˗�
	*/
	int GetMagnetState()
	{
		return m_magnetState;
	}

	/**
	 * @brief ���͂̏�Ԃ��Z�b�g����
	 * @param magPower -1:���� 1:�˗�
	*/
	void SetMagnetState(int magPower)
	{
		m_magnetState = magPower;
	}

	/**
	 * @brief �o�[�X�g�������擾����
	 * @return �o�[�X�g��?
	*/
	bool IsBurst()
	{
		return m_isBurst;
	}

	/**
	 * @brief ���݂̗̑͂��擾����
	 * @return ���݂̗̑�
	*/
	int GetHP()
	{
		return m_hp;
	}

	/**
	 * @brief �e�������̓����蔻��ɓ������Ă��邩���擾����
	 * @param bulletCollider �e�̃J�v�Z���R���C�_�[
	 * @return ����������?
	*/
	bool IsBulletHitCollider(const MyCapsuleCollider& bulletCollider)
	{
		return m_collider.isHitCapsule(bulletCollider);
	}

	/**
	 * @brief �L�����R�������s������
	 * @param moveSpeed �ړ����x
	*/
	void ExecuteCharacon(Vector3 moveSpeed)
	{
		m_charaCon.Execute(moveSpeed, 1.0f);
	}

	/**
	 * @brief �d�͉��������Z�b�g
	*/
	void ResetFall()
	{
		m_fallLoop = 0;
	}

	/**
	 * @brief �J�������x���Z�b�g
	 * @param sensitivity 
	*/
	void SetSensitivity(float sensitivity)
	{
		m_sensitivity = sensitivity;
	}

	/**
	 * @brief �J�����̐��ʂ��Z�b�g
	 * @param front �J�����̐���
	*/
	void SetCameraFront(const Vector3& front)
	{
		m_cameraFront = front;
	}

	/**
	 * @brief �J�����̐��ʂ��擾
	 * @return 
	*/
	const Vector3& GetCameraFront()
	{
		return m_cameraFront;
	}

	/**
	 * @brief �J�����ւ̌������Z�b�g
	 * @param direction �J�����ւ̌���
	*/
	void SetToCameraDirection(const Vector3& direction)
	{
		m_toCameraDir = direction;
	}

	/**
	 * @brief �v���C���[�̍��W���Z�b�g
	 * @param position �v���C���[�̍��W
	*/
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/**
	 * @brief �v���C���[�̍��W���擾
	 * @return �v���C���[�̍��W
	*/
	const Vector3& GetPosition()
	{
		return m_position;
	}

	/**
	 * @brief �v���C���[�̈ړ��ʂ��Z�b�g
	 * @param moveAmount �ړ���
	*/
	void SetMoveAmount(const Vector3& moveAmount)
	{
		m_moveAmount = moveAmount;
	}

	/**
	 * @brief �X�L�����f�������_�[
	 * @return �X�L�����f�������_�[��Ԃ�
	*/
	prefab::CSkinModelRender* GetSkinModelRender() { return m_skinModelRender; }

	/**
	 * @brief �L�������Ō�Ɍ������������擾
	 * @return �L�������Ō�Ɍ���������
	*/
	const Vector3& GetLastFrontDir()const  {return m_LastFrontDir;}

	/**
	 * @brief ���������ǂ������擾
	 * @return ������?
	*/
	bool IsLose() { return m_Lose; }
	
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
		enAnimationClip_num,			//�񋓓��Ŏg���v�f�̐���\���_�~�[
	};

	//��Ԃ̐�
	enum EnStatus {
		enStatus_Attack,		//�U�����
		enStatus_Run,			//������
		enStatus_Idle,			//�ҋ@���
		enStatus_Walk,			//�������
		enStatus_Move,			//�ړ��A�N�V�������		
		enStatus_Fall,			//�������
		enStatus_SpecialAttack,	//�K�E�Z���
		enStatus_Burst,			//�o�[�X�g���
		enStatus_Hit,			//��e���		
		enStatus_Num,			//��Ԃ̐��B
	};

public:
	//�K���L�A���e�̑���n
	std::vector<Debris*> m_holdDebrisVector;//�ێ����Ă���K���L���i�[�����R���e�i
	float m_holdDebrisRotateDeg = 0;//�ێ����Ă���K���L�̉�]�p�x

	std::vector<Bomb*> m_holdBombVector;//�ێ����Ă��锚�e���i�[�����R���e�i
	int m_selectBombNo = 0;//�I�����Ă��锚�e�̔ԍ�

	prefab::CFontRender* m_bulletNumFont = nullptr;//�c�e��
	prefab::CFontRender* m_bulletMaxFont = nullptr;//�c�e��
private:

	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//�L�����N�^�[�̃��f��
	prefab::CSpriteRender* m_crosshairRender = nullptr;			//�Ə��̃X�v���C�g
	prefab::CSpotLight* m_spotLight = nullptr;					//�v���C���[����납��Ƃ炷���C�g

	int m_playerNum = -1;										//�v���C���[�̔ԍ� 1P(0)�A2P(1)
	int m_magnetState = -1;										//���́A���͏��(-1)�A�˗�(1)
	int m_hp = 1000;											//�̗�
	float m_magnetCharge = 1000;								//���̓Q�[�W�̌��݂̗�

	Vector3 m_position = { 0.0f,0.0f,0.0f };					//�L�����N�^�[�̍��W
	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };					//���͂��o�Ă���n�_
	Quaternion m_rot;											//�L�����N�^�[�̉�]
	Vector3 m_scale = { 0.8f, 0.8f, 0.8f };						//�L�����N�^�[�̊g�嗦 **�萔��**

	CharacterController m_charaCon;								//�v���C���[�̃L�����R��
	Vector3 m_moveAmount = { 0.0f,0.0f,0.0f };					//�L�����R���Ɏ��s������ړ���
	int m_fallLoop = 0;											//��������p�̃��[�v�J�E���g
	float m_characterSpeed = 6.0;								//�L�����N�^�[�̈ړ����x

	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };				//�v���C���[����J�����ւ̌���
	Vector3 m_cameraFront;										//�J�����̑O����
	Vector3 m_cameraRight;										//�J�����̉E����
	Vector3 m_cameraPos;										//�J�����̃|�W�V����
	Quaternion m_cameraQRotY;									//�v���C���[�J������Y���̉�]
	float m_sensitivity = 2.0f;									//���_���x
	bool m_isLockon = false;									//�J�������b�N�I�����Ă��邩�B


	BackGround* m_stageModel = nullptr;							//�w�i�̃��f��(�����蔻��p)
	TriangleCollider m_triCollider[2];							//�P���ȎO�p�`�̓����蔻��(���ː�̔���Ɏg��)
	MyCapsuleCollider m_collider;								//�J�v�Z����̓����蔻��(�e�̓����蔻��Ɏg��)
	
	bool m_isBurst = false;										//�o�[�X�g���Ă��邩�B
	int m_burstCount = 0;										//�o�[�X�g���g�p���Ă���̌o�߃t���[��
	bool m_isAttacking = false;									//�U���𔭎˂��Ă��邩�B
	int m_attackCount = 0;										//�U���𔭎˂��Ă���̌o�߃t���[��(��)
	bool m_isSteal = false;										//���̈��̓o�[�X�g���ɂ��łɓG�̒e��D������

	//�A�j���[�V�����֘A
	AnimationClip animationClips[enAnimationClip_num];			//�A�j���[�V�����N���b�v
	EnStatus m_animStatus = enStatus_Idle;						//���݂̃A�j���[�V�����̏��
	bool m_SpecialAttackOn = false;								//�A�j���[�V�����p�t���O
	bool m_HitOn = false;										//��e�������ǂ���

	//�K�E�Z�֘A
	int m_specialShotFlag = false;								//�K�E�Z�𔭎˒�?
	int m_specialShotCount = 0;									//�K�E�Z�𔭎˂��n�߂Ă���̌o�߃t���[��
	int m_specialAttackGauge = 0;								//�K�E�Z�̃Q�[�W��
	int m_oldSpecialAttackGauge = 0;							//1�t���[���O�̕K�E�Z�Q�[�W��
	bool m_isGravityBulletAttack = false;						//���͂̕K�E�Z�̍U���^�C�~���O���w������p�ϐ��B

	//�m�b�N�o�b�N�֘A
	bool m_isKnockBack = false;									//�m�b�N�o�b�N���?
	int m_knockBackCount = 0;									//�m�b�N�o�b�N���n�߂Ă���̌o�߃t���[��

	//�̗͓��̏��\��
	prefab::CSpriteRender* m_HPBarSpriteRender = nullptr;		//HP�o�[�̃X�v���C�g
	prefab::CSpriteRender* m_HPBarDarkSpriteRender = nullptr;	//HP�o�[�̍���(�̗͂��Ȃ��Ȃ���)�����̃X�v���C�g
	prefab::CSpriteRender* m_DamageBarSpriteRender = nullptr;	//�󂯂��_���[�W����HP�o�[�̃X�v���C�g
	bool m_hpBarRedFlag = false;								//HP�o�[��Ԃ����邩
	MobiusGauge* m_mobiusGauge = nullptr;						//���r�E�X�Q�[�W
	prefab::CFontRender* m_chargeSPFontRender = nullptr;		//�K�E�Z�Q�[�W�̗��܂��̃t�H���g

	//�G�t�F�N�g
	prefab::CEffect* m_magEffect[2] = { nullptr,nullptr };		//���͂̃G�t�F�N�g�@2�̃G�t�F�N�g��A���ōĐ���������B
	int m_magEffectCallCount = 41;								//���̓G�t�F�N�g���ĂԂ܂ł̎���
	prefab::CEffect* m_burstEffect = nullptr;					//���̓o�[�X�g�̃G�t�F�N�g
	prefab::CEffect* m_hitEffect = nullptr;						//�_���[�W�G�t�F�N�g
	Vector3 m_damageEffectFront = { 0.0f,0.0f,0.0f };			//�_���[�W��H����������@�G�t�F�N�g���_���[�W���H�����������
	prefab::CEffect* m_SPFireEffect = nullptr;						//�K�E�Z�̔��˃G�t�F�N�g
	prefab::CEffect* m_SPGaugeMaxEffect = nullptr;				//�K�E�Q�[�W���ő�܂ŗ��܂������ɏo��G�t�F�N�g
	prefab::CEffect* m_SPChargeEffectRed = nullptr;				//�K�E�Z������ߓ���̂Ƃ��̃G�t�F�N�g�@�˗�
	prefab::CEffect* m_SPChargeEffectBlue = nullptr;			//�K�E�Z������ߓ���̂Ƃ��̃G�t�F�N�g�@����

	//��
	int m_footstepsTimer = 0;									//�����̖�Ԋu���J�E���g����

	//���s�\��
	prefab::CSpriteRender* m_resultWinnerSprite = nullptr;		//���҂�\������X�v���C�g(��P)
	prefab::CSpriteRender* m_resultWinSprite = nullptr;			//�����̃X�v���C�g(WIN!)

	GameScene* m_gameScene = nullptr;							//�Q�[���V�[��
	Player* m_enemy = nullptr;									//�G�v���C���[

	//�t�@�C�i���q�b�g�֘A
	bool m_Lose = false;										//���������������ǂ���
	bool m_FirstTime = true;									//��x�������[�v�����邽�߂̃t���O	
	Vector3 m_LastFrontDir;										//�L�������Ō�Ɍ���������			
};



