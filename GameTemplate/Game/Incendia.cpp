//焼夷弾(IncendialyGrenade)による炎上
#include "stdafx.h"
#include "Incendia.h"
#include "Player.h"

Incendia::~Incendia()
{
}
bool Incendia::Start()
{
	prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
	ss->Init(L"Assets/sound/炎.wav");
	ss->Play(false);
	return true;
}
void Incendia::Update()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			
			//ダメージを食らう間隔
			if (m_damageCountdown[player->m_playerNum] > 0)
				m_damageCountdown[player->m_playerNum]--;

			//プレイヤーが近ければ
			Vector3 diff = m_position - player->m_position;		//diffはdifference(差)
			float dis = diff.Length();		//disはdistance(距離)
			dis = fabsf(dis);
			if (dis <= 300.0f)
			{
				//少しずつダメージを受ける。		
				if (m_damageCountdown[player->m_playerNum] <= 0) 
				{
					player->Damage(10);
					player->m_TakeAttackNum++;//攻撃を受けた回数
					//次に炎上ダメージを受けるまでの間隔を設定
					m_damageCountdown[player->m_playerNum] = 60;
				}
			}
			return true;
		}
	);

	//炎が消えるまでのカウント
	m_deleteCountdown--;
	if (m_deleteCountdown % 100 == 0) {
		prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
		ss->Init(L"Assets/sound/炎.wav");
		ss->Play(false);
	}
	if (m_deleteCountdown <= 0)
		DeleteGO(this);
}