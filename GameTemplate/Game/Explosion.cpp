//ExplosionGrenade‚ğ‚Æ‚Á‚½‚Æ‚«‚Ì”š”­ˆ—
#include "stdafx.h"
#include "Explosion.h"
#include "Player.h"

Explosion::~Explosion()
{
}
bool Explosion::Start()
{
	//‰¹‚ğÄ¶
	prefab::CSoundSource* ssExplosion = NewGO<prefab::CSoundSource>(0);;
	ssExplosion->Init(L"Assets/sound/”š”­‰¹.wav");
	ssExplosion->Play(false);
	return true;
}
void Explosion::Update()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//ƒvƒŒƒCƒ„[‚ª‹ß‚¯‚ê‚Î
			Vector3 diff = m_position - player->m_position;		//diff‚Ídifference(·)
			float dis = diff.Length();		//dis‚Ídistance(‹——£)
			dis = fabsf(dis);
			if (dis <= 300.0f) 
			{
				player->Damage(300 - dis);
				player->m_TakeAttackNum++;//UŒ‚‚ğó‚¯‚½‰ñ”
			}
			return true;
		});
	DeleteGO(this);
}