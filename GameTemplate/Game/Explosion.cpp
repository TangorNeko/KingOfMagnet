//ExplosionGrenadeをとったときの爆発処理
#include "stdafx.h"
#include "Explosion.h"
#include "Player.h"

Explosion::~Explosion()
{
}
bool Explosion::Start()
{
	//音を再生
	prefab::CSoundSource* ssExplosion = NewGO<prefab::CSoundSource>(0);;
	ssExplosion->Init(L"Assets/sound/爆発音.wav");
	ssExplosion->SetVolume(1.1f);
	ssExplosion->Play(false);

	//エフェクトを再生
	prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
	effect->Init(u"Assets/effect/爆発.efk");
	effect->SetPosition(m_position);
	effect->SetScale({ 35.0f, 35.0f, 35.0f });
	effect->Play();

	return true;
}
void Explosion::Update()
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
	DeleteGO(this);
}