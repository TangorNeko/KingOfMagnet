#include "stdafx.h"
#include "DamageDisplay.h"

#include <random>

namespace
{
	const int DAMAGEDISPLAY_DELETE_TIME = 50;
	const Vector4 FONT_DAMAGE_COLOR = { 1.0f,1.0f,0.1f,1.0f };
	const Vector4 FONT_SHADOW_COLOR = { 0.0f,0.0f,0.0f,1.0f };
	const float FONT_SHADOW_OFFSET = 2.0f;
	const float DAMAGEPOSITION_Y_SPACER = 100.0f;
	const int DAMAGEPOSITION_MOD_VALUE = 50;
	const int DAMAGEDIRECTION_MODE_VALUE = 2;
	const float FONT_DAMAGE_X_PIVOT = -40.0f;
	const int MOVESPEED_ADJUST_VALUE = 10;
	const int FALLSPEED_ADJUST_VALUE = 50;
}

DamageDisplay::~DamageDisplay()
{
	DeleteGO(m_damageFontRender);
}
bool DamageDisplay::Start()
{
	m_damageFontRender = NewGO<prefab::CFontRender>(1);

	//�_���[�W���󂯂��v���C���[�̓G���̉�ʂɕ`��
	m_damageFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)m_enemyNum);

	m_damageFontRender->SetShadowFlag(true);
	m_damageFontRender->SetShadowColor(FONT_SHADOW_COLOR);
	m_damageFontRender->SetShadowOffset(FONT_SHADOW_OFFSET);
	m_damageFontRender->SetColor(FONT_DAMAGE_COLOR);

	//�\������_���[�W�ʂ��e�L�X�g�Ƃ��ăZ�b�g
	m_damageFontRender->SetText(std::to_wstring(m_damage));

	//������悤�ɕ\������̂ňʒu�͏�ɂ��炵�Ă���
	m_damagePosition.y += DAMAGEPOSITION_Y_SPACER;

	std::random_device device;
	std::mt19937_64 rnd(device());
	
	//��ĂɃ_���[�W���󂯂����ɓ����ꏊ�ɏd�Ȃ�Â炢�悤�ɂ�����x�����_���ɂ��炷
	m_damagePosition.y += rnd() % DAMAGEPOSITION_MOD_VALUE;
	m_damagePosition.x += rnd() % DAMAGEPOSITION_MOD_VALUE;
	
	//�t�H���g���E�Ɉړ����邩���Ɉړ����邩�����_���Ɍ���B
	m_isxSpeedPositive = rnd() % DAMAGEDIRECTION_MODE_VALUE;
	return true;
}
void DamageDisplay::Update()
{
	m_loopCount++;
	if (m_loopCount > DAMAGEDISPLAY_DELETE_TIME)//50�t���[�������Ă������
	{
		DeleteGO(this);
	}	

	//�_���[�W�ʂɔ�Ⴕ�Ĉړ��X�s�[�h�͏オ��B
	if (m_isxSpeedPositive)
	{
		//�E�����Ɉړ�
		m_xMoveSpeed = m_damage / MOVESPEED_ADJUST_VALUE;
	}
	else
	{
		//�������Ɉړ�
		m_xMoveSpeed = -m_damage / MOVESPEED_ADJUST_VALUE;
	}
	
	//�ړ����x��1�t���[�����Ƃɉ��Z�B
	m_xPlusValue += m_xMoveSpeed;

	//�_���[�W�ɔ�Ⴕ�Ĉړ��X�s�[�h�͏オ��B
	m_yPlusValue -= m_yMoveSpeed * m_damage / FALLSPEED_ADJUST_VALUE;
	//1�t���[�����ƂɈړ��ʂ��傫���Ȃ�(���˂�悤�Ȋ����ɂȂ�)
	m_yMoveSpeed++;

	//�_���[�W�������������W����ʏ�̍��W�ɕϊ�
	g_camera3D[m_enemyNum]->CalcScreenPositionFromWorldPosition(m_screenDamagePosition, m_damagePosition);

	//�t�H���g�̃s�{�b�g���@�\���Ă��Ȃ��̂ŁA�������ɂ��炵�Ē�������
	m_screenDamagePosition.x += FONT_DAMAGE_X_PIVOT;

	//�v�Z���ʂ̃X�N���[����̍��W��x��y�̈ړ��ʂ��v���X�B
	m_damageFontRender->SetPosition({ m_screenDamagePosition.x + m_xPlusValue,m_screenDamagePosition.y + m_yPlusValue });

}