//ExplosionGrenade���Ƃ����Ƃ��̔�������
#include "stdafx.h"
#include "Explosion.h"
#include "Player.h"

Explosion::~Explosion()
{
}
bool Explosion::Start()
{
	//�����Đ�
	prefab::CSoundSource* ssExplosion = NewGO<prefab::CSoundSource>(0);;
	ssExplosion->Init(L"Assets/sound/������.wav");
	ssExplosion->SetVolume(1.1f);
	ssExplosion->Play(false);

	//�G�t�F�N�g���Đ�
	prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
	effect->Init(u"Assets/effect/����.efk");
	effect->SetPosition(m_position);
	effect->SetScale({ 35.0f, 35.0f, 35.0f });
	effect->Play();

	return true;
}
void Explosion::Update()
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
	DeleteGO(this);
}