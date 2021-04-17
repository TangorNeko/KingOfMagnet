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


	bool m_isxSpeedPositive;//ダメージのフォントが右に移動するか?(falseなら左に移動)
	float m_xplusValue = 0;//フォントが表示される座標にプラスされるx座標の量
	float m_xmoveSpeed;//フォントのx方向の移動速度

	float m_yplusValue = 0;//フォントが表示される座標にプラスされるy座標の量
	float m_ymoveSpeed = -10;//フォントのy方向の移動速度
	
	Vector3 m_damagePos;
	Vector2 m_screenDamagePos;
	std::wstring damageText;//ダメージ量表示	
};
