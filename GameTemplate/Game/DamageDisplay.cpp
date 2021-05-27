#include "stdafx.h"
#include "DamageDisplay.h"
#include "Character_base.h"

#include <random>

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

	std::random_device device;
	std::mt19937_64 rnd(device());
	m_damagePos.y += rnd() % 50;
	m_damagePos.x += rnd() % 50;
	m_damagePos.x -= 40.0f;

	//�t�H���g���E�Ɉړ����邩���Ɉړ����邩�����_���Ɍ���B
	m_isxSpeedPositive = rnd() % 2;
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
	if (m_loopcount > m_deletetime)//50�t���[�������Ă������
	{
		DeleteGO(this);
	}	

	//�_���[�W�ʂɔ�Ⴕ�Ĉړ��X�s�[�h�͏オ��B
	if (m_isxSpeedPositive)
	{
		//�E�����Ɉړ�
		m_xmoveSpeed = m_damage / 10;
	}
	else
	{
		//�������Ɉړ�
		m_xmoveSpeed = -m_damage / 10;
	}
	
	//�ړ����x��1�t���[�����Ƃɉ��Z�B
	m_xplusValue += m_xmoveSpeed;


	//�_���[�W�ɔ�Ⴕ�Ĉړ��X�s�[�h�͏オ��B
	m_yplusValue -= m_ymoveSpeed * m_damage / 50;
	//1�t���[�����ƂɈړ��ʂ��傫���Ȃ�(���˂�悤�Ȋ����ɂȂ�)
	m_ymoveSpeed++;


	g_camera3D[m_enemyNum]->CalcScreenPositionFromWorldPosition(m_screenDamagePos, m_damagePos);

	//�v�Z���ʂ̃X�N���[����̍��W��x��y�̈ړ��ʂ��v���X�B
	m_fontRenderDamage->SetPosition({ m_screenDamagePos.x + m_xplusValue,m_screenDamagePos.y + m_yplusValue });
	m_fontRenderDamage->SetColor({ 0.0f,1.0f,1.0f,m_Alpha });

}