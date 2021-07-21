#pragma once
class DamageDisplay:public IGameObject
{
	~DamageDisplay()override;
	bool Start()override;
	void Update()override;

public:

	/**
	 * @brief �_���[�W�������������W��ݒ肷��
	 * @param damagePos ���W
	*/
	void SetDamagePosition(const Vector3& damagePos) { m_damagePosition = damagePos; }

	/**
	 * @brief �_���[�W���󂯂��v���C���[�̓G�̔ԍ���ݒ肷��
	 * @param playerNum �G�̃v���C���[�̔ԍ�
	*/
	void SetEnemyNum(const int playerNum) { m_enemyNum = playerNum; }

	/**
	 * @brief �\������_���[�W�ʂ�ݒ�
	 * @param damage �_���[�W��
	*/
	void SetDamage(const int damage) { m_damage = damage; }

private:
	prefab::CFontRender* m_damageFontRender = nullptr;	//�_���[�W�ʂ̃t�H���g
	int m_damage = 0;									//�\������_���[�W��
	std::wstring damageText = L"";						//�\������_���[�W�ʂ��e�L�X�g�ɕϊ���������	
	Vector3 m_damagePosition = Vector3::Zero;			//�_���[�W�������������W
	Vector2 m_screenDamagePosition = Vector2::Zero;		//�_���[�W�������������W����ʏ�̍��W�ɕϊ���������
	int m_enemyNum = 0;									//�_���[�W���󂯂��v���C���[�̓G�̔ԍ�(�_���[�W���󂯂����A�G�̃v���C���[�̉�ʂɕ\������̂�)
	int m_loopCount = 0;								//�\�����n�߂Ă��牽�t���[���o������
	bool m_isxSpeedPositive = false;					//�_���[�W�̃t�H���g���E�Ɉړ����邩?(false�Ȃ獶�Ɉړ�)
	float m_xPlusValue = 0.0f;							//�t�H���g���\���������W�Ƀv���X�����x���W�̗�
	float m_xMoveSpeed = 0.0f;							//�t�H���g��x�����̈ړ����x
	float m_yPlusValue = 0.0f;							//�t�H���g���\���������W�Ƀv���X�����y���W�̗�
	float m_yMoveSpeed = -10.0f;						//�t�H���g��y�����̈ړ����x
};
