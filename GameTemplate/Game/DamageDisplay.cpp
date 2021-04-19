#include "stdafx.h"
#include "DamageDisplay.h"
#include "Character_base.h"

DamageDisplay::~DamageDisplay()
{
	DeleteGO(m_fontRenderDamage);
}
bool DamageDisplay::Start()
{
	m_oldhitcount = m_hitcount;
	m_fontRenderDamage = NewGO<prefab::CFontRender>(1);
	m_fontRenderDamage->SetDrawScreen((prefab::CFontRender::DrawScreen)m_enemyNum);

	m_fontRenderDamage->SetColor({ 0.0f,0.0f,1.0f,1.0f });

	m_fontRenderDamage->SetText(std::to_wstring(m_damage));
	m_damagePos.y += 100.0f;
	m_damagePos.y += rand() % 50;
	m_damagePos.x += rand() % 50;
	m_damagePos.x -= 40.0f;

	//フォントが右に移動するか左に移動するかランダムに決定。
	m_isxSpeedPositive = rand() & 2;
	return true;
}
void DamageDisplay::Update()
{
	m_loopcount++;
	if (m_loopcount > m_deletetime)
	{
		m_damagePos.y += 1.0f;
		m_Alpha -= 0.02;
	}
	if (m_loopcount > m_deletetime+5000)//50フレーム立ってから消す
	{
		DeleteGO(this);
	}	

	//ダメージ量に比例して移動スピードは上がる。
	if (m_isxSpeedPositive)
	{
		//右方向に移動
		m_xmoveSpeed = m_damage / 10;
	}
	else
	{
		//左方向に移動
		m_xmoveSpeed = -m_damage / 10;
	}
	
	//移動速度を1フレームごとに加算。
	m_xplusValue += m_xmoveSpeed;


	//ダメージに比例して移動スピードは上がる。
	m_yplusValue -= m_ymoveSpeed * m_damage / 50;
	//1フレームごとに移動量が大きくなる(跳ねるような感じになる)
	m_ymoveSpeed++;


	g_camera3D[m_enemyNum]->CalcScreenPositionFromWorldPosition(m_screenDamagePos, m_damagePos);

	//計算結果のスクリーン上の座標にxとyの移動量をプラス。
	m_fontRenderDamage->SetPosition({ m_screenDamagePos.x + m_xplusValue,m_screenDamagePos.y + m_yplusValue });
	m_fontRenderDamage->SetColor({ 0.0f,1.0f,1.0f,m_Alpha });

}