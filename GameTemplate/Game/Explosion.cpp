//ExplosionGrenade���Ƃ����Ƃ��̔�������
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
	//�����Đ�
	prefab::CSoundSource* ssExplosion = NewGO<prefab::CSoundSource>(0);;
	ssExplosion->Init(L"Assets/sound/������.wav");
	ssExplosion->SetVolume(1.1f);
	ssExplosion->Play(false);

	//�G�t�F�N�g���Đ�
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/����.efk");
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
				//�v���C���[���߂����
				Vector3 diff = m_position - player->m_position;		//diff��difference(��)
				float dis = diff.Length();		//dis��distance(����)
				dis = fabsf(dis);
				if (dis <= 300.0f)
				{
					player->Damage(300 - dis);
					player->m_TakeAttackNum++;//�U�����󂯂���
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