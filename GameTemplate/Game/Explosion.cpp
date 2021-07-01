//ExplosionGrenade���Ƃ����Ƃ��̔�������
#include "stdafx.h"
#include "Explosion.h"
#include "Player.h"

namespace
{
	const float SOUND_SE_EXPLOSION_VOLUME = 1.1f;
	const Vector3 EFFECT_EXPLOSION_SCALE = { 35.0f,35.0f,35.0f };
	const float EXPLOSION_RANGE = 300.0f;
}

Explosion::~Explosion()
{
	m_effect->Stop();
	DeleteGO(m_effect);
}
bool Explosion::Start()
{
	//�����Đ�
	prefab::CSoundSource* explosionSound = NewGO<prefab::CSoundSource>(0);;
	explosionSound->Init(L"Assets/sound/������.wav", SoundType::enSE);
	explosionSound->SetVolume(SOUND_SE_EXPLOSION_VOLUME);
	explosionSound->Play(false);

	//�G�t�F�N�g���Đ�
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/����.efk");
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
				//�v���C���[���߂����
				Vector3 diff = m_position - player->GetPosition();		//diff��difference(��)
				float dis = diff.Length();		//dis��distance(����)
				dis = fabsf(dis);
				if (dis <= EXPLOSION_RANGE)
				{
					player->Damage(EXPLOSION_RANGE - dis);
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