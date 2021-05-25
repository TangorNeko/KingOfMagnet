#include "stdafx.h"
#include "Repulsion.h"
#include "Player.h"
#include "GameScene.h"
#include "SampleScene.h"

Repulsion::~Repulsion()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_ss2[0]);
	DeleteGO(m_ss2[1]);
}
bool Repulsion::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/repulsion.tkm");
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rot);
	m_skinModelRender->SetScale(m_scale);
	m_gameScene = FindGO<SampleScene>("gamescene");

	m_ss2[0] = NewGO<prefab::CSoundSource>(0);
	m_ss2[0]->Init(L"Assets/sound/UFO.wav");
	m_ss2[1] = NewGO<prefab::CSoundSource>(0);
	m_ss2[1]->Init(L"Assets/sound/UFO.wav");

	return true;
}
void Repulsion::Update()
{	
	//ポーズ中ならスキップ。
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPause)
	{
		return;
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{			
			//恐らくプレイヤーが二人いるから上手く行かない。
			//音を2プレイヤー分用意する。

				//床との距離
				diff = player->m_position - m_position;
				//立方体の範囲に入れば斥力を与える
				if (fabsf(diff.x) < 160 && fabsf(diff.z) < 160)
				{
					if (diff.y < 200)//床との距離が近ければ上昇
					{
						Vector3 up = Vector3::Up;
						up *= 5;
						player->m_charaCon.Execute(up, 1.0f);
						player->m_fallLoop = 0;
					}

					//SE
					//ss1とss2を両方鳴らす

					//ss1を鳴らす。
					if (m_isPlayss1[player->m_playerNum] == false)
					{
						prefab::CSoundSource* ss1 = NewGO<prefab::CSoundSource>(0);
						ss1->Init(L"Assets/sound/マジックウェーブ.wav");
						ss1->SetVolume(1.5f);
						ss1->Play(false);
						m_isPlayss1[player->m_playerNum] = true;
					}

					//ss2を鳴らす。
					if (m_isPlayss2[player->m_playerNum] == false) {												
						
						m_ss2[player->m_playerNum]->SetVolume(1.5f);
						m_ss2[player->m_playerNum]->Play(true);	
						m_isPlayss2[player->m_playerNum] = true;
					}
					//ss2がまだ鳴っているのならば、音量を1.5fにする。
					m_ss2Volume[player->m_playerNum] += 0.1f;
					if (m_ss2Volume[player->m_playerNum] >= 1.5f) {
						m_ss2Volume[player->m_playerNum] = 1.5f;
					}
					m_ss2[player->m_playerNum]->SetVolume(m_ss2Volume[player->m_playerNum]);

				}

				//範囲に入っていないとき
				else if (fabsf(diff.x) >= 160 || fabsf(diff.z) >= 160)
				{
					//ss1のFlagをリセット
					if (m_isPlayss1[player->m_playerNum] == true)
					{
						m_isPlayss1[player->m_playerNum] = false;
					}

					//ss2を少しずつフェードアウトさせる。
					if (m_isPlayss2[player->m_playerNum] == true)
					{
						m_ss2Volume[player->m_playerNum] -= 0.01f;
						m_ss2[player->m_playerNum]->SetVolume(m_ss2Volume[player->m_playerNum]);
						//音が完全に消えたら、Stop
						if (m_ss2Volume[player->m_playerNum] <= 0.0f)
						{
							m_ss2Volume[player->m_playerNum] = 0.0f;
							m_ss2[player->m_playerNum]->Stop();
							m_isPlayss2[player->m_playerNum] = false;
						}
					}
				}

			return true;
		}
	);
		
}