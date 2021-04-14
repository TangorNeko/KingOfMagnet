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
	
	Vector3 m_damagePos;
	Vector2 m_screenDamagePos;
	std::wstring damageText;//ダメージ量表示	
};
