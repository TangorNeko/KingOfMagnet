//ExplosionGrenadeをとったときの爆発処理
#include "stdafx.h"
#include "Explosion.h"
#include "Player.h"

namespace
{
	const float SOUND_SE_EXPLOSION_VOLUME = 1.1f;					//爆発サウンドの音量
	const Vector3 EFFECT_EXPLOSION_SCALE = { 35.0f,35.0f,35.0f };	//爆発エフェクトの拡大率
	const float EXPLOSION_RANGE = 300.0f;							//爆発の影響範囲
}

Explosion::~Explosion()
{
	m_effect->Stop();
	DeleteGO(m_effect);
}
bool Explosion::Start()
{
	//音を再生
	SoundOneShotPlay(L"Assets/sound/Explosion.wav", SOUND_SE_EXPLOSION_VOLUME);

	//エフェクトを再生
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/爆発.efk");
	m_effect->SetPosition(m_position);
	m_effect->SetScale(EFFECT_EXPLOSION_SCALE);
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
				Vector3 diff = m_position - player->GetPosition();		//diffはdifference(差)
				float dis = diff.Length();		//disはdistance(距離)
				dis = fabsf(dis);
				if (dis <= EXPLOSION_RANGE)
				{
					player->Damage(static_cast<int>(EXPLOSION_RANGE - dis));
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