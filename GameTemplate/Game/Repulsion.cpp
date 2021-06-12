#include "stdafx.h"
#include "Repulsion.h"
#include "Player.h"
#include "GameScene.h"

namespace
{
	const float REPULSION_AREA_LENGTH = 160.0f;
	const float REPULSION_RANGE = 200.0f;
	const Vector3 REPULSION_SPEED = { 0.0f,5.0f,0.0f };
	const float SOUND_SE_REPULSION_ENTER_VOLUME = 1.5f;
	const float SOUND_SE_REPULSION_STAY_VOLUME = 1.5f;
	const float SOUND_SE_INCREASE_VALUE = 0.1f;
	const float SOUND_SE_DECAY_VALUE = 0.01f;
}

Repulsion::~Repulsion()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_repulsionStaySE[NUMBER_PLAYER1]);
	DeleteGO(m_repulsionStaySE[NUMBER_PLAYER2]);
}
bool Repulsion::Start()
{
	//斥力床のモデルを作成
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/repulsion.tkm");
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_qRot);

	//斥力床に乗り続けている時のSEを作成
	m_repulsionStaySE[NUMBER_PLAYER1] = NewGO<prefab::CSoundSource>(0);
	m_repulsionStaySE[NUMBER_PLAYER1]->Init(L"Assets/sound/UFO.wav", SoundType::enSE);
	m_repulsionStaySE[NUMBER_PLAYER2] = NewGO<prefab::CSoundSource>(0);
	m_repulsionStaySE[NUMBER_PLAYER2]->Init(L"Assets/sound/UFO.wav", SoundType::enSE);

	//ゲームシーンを取得
	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}
void Repulsion::Update()
{	
	//ポーズ中ならスキップ。
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause)
	{
		return;
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{			
			//恐らくプレイヤーが二人いるから上手く行かない。
			//音を2プレイヤー分用意する。

				//斥力床の中心とプレイヤーとの距離
				Vector3 diff = player->m_position - m_position;
				//立方体の範囲に入れば斥力を与える
				if (fabsf(diff.x) < REPULSION_AREA_LENGTH && fabsf(diff.z) < REPULSION_AREA_LENGTH)
				{
					if (diff.y < REPULSION_RANGE)//床との距離が近ければ上昇
					{
						Vector3 repulsionValue = REPULSION_SPEED;
						player->m_charaCon.Execute(repulsionValue, 1.0f);
						player->m_fallLoop = 0;
					}

					//SE

					//プレイヤーが斥力床に入ってきた時の音を鳴らす。
					if (m_isPlayEnterSE[player->m_playerNum] == false)
					{
						prefab::CSoundSource* ss1 = NewGO<prefab::CSoundSource>(0);
						ss1->Init(L"Assets/sound/マジックウェーブ.wav", SoundType::enSE);
						ss1->SetVolume(SOUND_SE_REPULSION_ENTER_VOLUME);
						ss1->Play(false);
						m_isPlayEnterSE[player->m_playerNum] = true;
					}

					//プレイヤーが斥力床に乗っている時の音を鳴らす。
					if (m_isPlayStaySE[player->m_playerNum] == false) {												
						
						m_repulsionStaySE[player->m_playerNum]->SetVolume(SOUND_SE_REPULSION_STAY_VOLUME);
						m_repulsionStaySE[player->m_playerNum]->Play(true);	
						m_isPlayStaySE[player->m_playerNum] = true;
					}
					//StaySEがまだ鳴っているのならば、音量を1.5fになるまで少しずつ増加させる。
					m_staySEVolume[player->m_playerNum] += SOUND_SE_INCREASE_VALUE;
					if (m_staySEVolume[player->m_playerNum] >= SOUND_SE_REPULSION_STAY_VOLUME) {
						m_staySEVolume[player->m_playerNum] = SOUND_SE_REPULSION_STAY_VOLUME;
					}
					m_repulsionStaySE[player->m_playerNum]->SetVolume(m_staySEVolume[player->m_playerNum]);

				}

				//範囲に入っていないとき
				else if (fabsf(diff.x) >= REPULSION_AREA_LENGTH || fabsf(diff.z) >= REPULSION_AREA_LENGTH)
				{
					//EnterSEのFlagをリセット
					if (m_isPlayEnterSE[player->m_playerNum] == true)
					{
						m_isPlayEnterSE[player->m_playerNum] = false;
					}

					//StaySEを少しずつフェードアウトさせる。
					if (m_isPlayStaySE[player->m_playerNum] == true)
					{
						m_staySEVolume[player->m_playerNum] -= SOUND_SE_DECAY_VALUE;
						m_repulsionStaySE[player->m_playerNum]->SetVolume(m_staySEVolume[player->m_playerNum]);
						//音が完全に消えたら、StaySEの再生を停止
						if (m_staySEVolume[player->m_playerNum] <= 0.0f)
						{
							m_staySEVolume[player->m_playerNum] = 0.0f;
							m_repulsionStaySE[player->m_playerNum]->Stop();
							m_isPlayStaySE[player->m_playerNum] = false;
						}
					}
				}

			return true;
		}
	);
		
}