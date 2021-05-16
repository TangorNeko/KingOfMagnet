//焼夷弾(IncendialyGrenade)による炎上
#include "stdafx.h"
#include "Incendia.h"
#include "Player.h"

Incendia::~Incendia()
{
}
bool Incendia::Start()
{
	//音を再生
	prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
	ss->Init(L"Assets/sound/炎.wav");
	ss->Play(false);

	//エフェクトを再生
	prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
	effect->Init(u"Assets/effect/焼夷.efk");
	effect->SetPosition(m_position);
	effect->SetScale({ 25.0f, 15.0f, 25.0f });
	effect->Play();

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
			if (dis <= 250.0f)
			{
				//少しずつダメージを受ける。		
				if (m_damageCountdown[player->m_playerNum] <= 0) 
				{
					player->Damage(30);
					player->m_TakeAttackNum++;//攻撃を受けた回数
					//次に炎上ダメージを受けるまでの間隔を設定
					m_damageCountdown[player->m_playerNum] = 30;
				}
			}
			return true;
		}
	);

	//炎が消えるまでのカウント
	m_deleteCountdown--;
	if (m_deleteCountdown <= 0)
		DeleteGO(this);
}