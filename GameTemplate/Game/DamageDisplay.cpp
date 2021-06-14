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

	//ダメージを受けたプレイヤーの敵側の画面に描画
	m_damageFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)m_enemyNum);

	m_damageFontRender->SetShadowFlag(true);
	m_damageFontRender->SetShadowColor(FONT_SHADOW_COLOR);
	m_damageFontRender->SetShadowOffset(FONT_SHADOW_OFFSET);
	m_damageFontRender->SetColor(FONT_DAMAGE_COLOR);

	//表示するダメージ量をテキストとしてセット
	m_damageFontRender->SetText(std::to_wstring(m_damage));

	//落ちるように表示するので位置は上にずらしておく
	m_damagePosition.y += DAMAGEPOSITION_Y_SPACER;

	std::random_device device;
	std::mt19937_64 rnd(device());
	
	//一斉にダメージを受けた時に同じ場所に重なりづらいようにある程度ランダムにずらす
	m_damagePosition.y += rnd() % DAMAGEPOSITION_MOD_VALUE;
	m_damagePosition.x += rnd() % DAMAGEPOSITION_MOD_VALUE;
	
	//フォントが右に移動するか左に移動するかランダムに決定。
	m_isxSpeedPositive = rnd() % DAMAGEDIRECTION_MODE_VALUE;
	return true;
}
void DamageDisplay::Update()
{
	m_loopCount++;
	if (m_loopCount > DAMAGEDISPLAY_DELETE_TIME)//50フレーム立ってから消す
	{
		DeleteGO(this);
	}	

	//ダメージ量に比例して移動スピードは上がる。
	if (m_isxSpeedPositive)
	{
		//右方向に移動
		m_xMoveSpeed = m_damage / MOVESPEED_ADJUST_VALUE;
	}
	else
	{
		//左方向に移動
		m_xMoveSpeed = -m_damage / MOVESPEED_ADJUST_VALUE;
	}
	
	//移動速度を1フレームごとに加算。
	m_xPlusValue += m_xMoveSpeed;

	//ダメージに比例して移動スピードは上がる。
	m_yPlusValue -= m_yMoveSpeed * m_damage / FALLSPEED_ADJUST_VALUE;
	//1フレームごとに移動量が大きくなる(跳ねるような感じになる)
	m_yMoveSpeed++;

	//ダメージが発生した座標を画面上の座標に変換
	g_camera3D[m_enemyNum]->CalcScreenPositionFromWorldPosition(m_screenDamagePosition, m_damagePosition);

	//フォントのピボットが機能していないので、少し左にずらして調整する
	m_screenDamagePosition.x += FONT_DAMAGE_X_PIVOT;

	//計算結果のスクリーン上の座標にxとyの移動量をプラス。
	m_damageFontRender->SetPosition({ m_screenDamagePosition.x + m_xPlusValue,m_screenDamagePosition.y + m_yPlusValue });

}