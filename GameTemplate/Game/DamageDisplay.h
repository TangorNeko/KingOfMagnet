#pragma once
class DamageDisplay:public IGameObject
{
	bool Start();
	void Update();
	~DamageDisplay();
public:
	int m_hitcount = 0;
	int m_oldhitcount = 0;
	int m_damage = 0;
	int m_enemyNum;
	int m_loopcount=0;
	int m_deletetime = 10;
	float m_Alpha = 1.0f;
	prefab::CFontRender* m_fontRenderDamage = nullptr;


	bool m_isxSpeedPositive;//�_���[�W�̃t�H���g���E�Ɉړ����邩?(false�Ȃ獶�Ɉړ�)
	float m_xplusValue = 0;//�t�H���g���\���������W�Ƀv���X�����x���W�̗�
	float m_xmoveSpeed;//�t�H���g��x�����̈ړ����x

	float m_yplusValue = 0;//�t�H���g���\���������W�Ƀv���X�����y���W�̗�
	float m_ymoveSpeed = -10;//�t�H���g��y�����̈ړ����x
	
	Vector3 m_damagePos;
	Vector2 m_screenDamagePos;
	std::wstring damageText;//�_���[�W�ʕ\��	
};
