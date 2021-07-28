//ExplosionGrenade���Ƃ����Ƃ��̔�������
#include "stdafx.h"
#include "Explosion.h"
#include "Player.h"

namespace
{
	const float SOUND_SE_EXPLOSION_VOLUME = 1.1f;					//�����T�E���h�̉���
	const Vector3 EFFECT_EXPLOSION_SCALE = { 35.0f,35.0f,35.0f };	//�����G�t�F�N�g�̊g�嗦
	const float EXPLOSION_RANGE = 300.0f;							//�����̉e���͈�
}

Explosion::~Explosion()
{
	m_effect->Stop();
	DeleteGO(m_effect);
}
bool Explosion::Start()
{
	//�����Đ�
	SoundOneShotPlay(L"Assets/sound/Explosion.wav", SOUND_SE_EXPLOSION_VOLUME);

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