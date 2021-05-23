//ExplosionGrenadeをとったときの爆発処理
#include "stdafx.h"
#include "Explosion.h"
#include "Player.h"

Explosion::~Explosion()
{
	m_effect->Stop();
	DeleteGO(m_effect);
}
bool Explosion::Start()
{
	//音を再生
	prefab::CSoundSource* ssExplosion = NewGO<prefab::CSoundSource>(0);;
	ssExplosion->Init(L"Assets/sound/爆発音.wav");
	ssExplosion->SetVolume(1.1f);
	ssExplosion->Play(false);

	//エフェクトを再生
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/爆発.efk");
	m_effect->SetPosition(m_position);
	m_effect->SetScale({ 35.0f, 35.0f, 35.0f });
	m_effect->Play();

	return true;
}
void Explosion::Update()
{
	if (m_deleteFlag == false)
	{
		QueryGOs<Player>("Player", [this](Player* player)->bool
			{
				//プレイヤーが近ければ
				Vector3 diff = m_position - player->m_position;		//diffはdifference(差)
				float dis = diff.Length();		//disはdistance(距離)
				dis = fabsf(dis);
				if (dis <= 300.0f)
				{
					player->Damage(300 - dis);
					player->m_TakeAttackNum++;//攻撃を受けた回数
				}
				return true;
			});
		m_deleteFlag = true;
	}
	else if (m_deleteFlag == true && m_effect->IsPlay() == false)
	{
		DeleteGO(this);
	}
}