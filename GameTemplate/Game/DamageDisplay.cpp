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
	m_damagePos.y += rand() % 20;
	m_damagePos.x += rand() % 50;
	m_damagePos.x -= 40.0f;
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
	if (m_loopcount > m_deletetime+50)//50ƒtƒŒ[ƒ€—§‚Á‚Ä‚©‚çÁ‚·
	{
		DeleteGO(this);
	}	
	g_camera3D[m_enemyNum]->CalcScreenPositionFromWorldPosition(m_screenDamagePos, m_damagePos);
	m_fontRenderDamage->SetPosition(m_screenDamagePos);
	m_fontRenderDamage->SetColor({ 0.0f,1.0f,1.0f,m_Alpha });

}