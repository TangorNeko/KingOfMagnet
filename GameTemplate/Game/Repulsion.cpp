#include "stdafx.h"
#include "Repulsion.h"
#include "Player.h"
#include "GameScene.h"

namespace
{
	const float REPULSION_AREA_LENGTH = 160.0f;				//斥力床のxz範囲
	const float REPULSION_RANGE = 200.0f;					//斥力床のy範囲
	const Vector3 REPULSION_SPEED = { 0.0f,5.0f,0.0f };		//斥力床の上昇スピード
	const float SOUND_SE_REPULSION_ENTER_VOLUME = 1.5f;		//斥力床を踏んだ瞬間のSEのボリューム
	const float SOUND_SE_REPULSION_STAY_VOLUME = 1.0f;		//斥力床にとどまっている時のSEのボリューム
	const float SOUND_SE_INCREASE_VALUE = 0.1f;				//斥力床のSEのボリュームの増加量
	const float SOUND_SE_DECAY_VALUE = 0.01f;				//斥力床のSEのボリュームの減衰量
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
	//ポーズ中かリザルトシーンならスキップ。
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause || m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		return;
	}

	if (m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		for (auto soundSource : m_repulsionStaySE)
		{
			soundSource->Stop();
		}
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{			
			//恐らくプレイヤーが二人いるから上手く行かない。
			//音を2プレイヤー分用意する。

			int playerNum = player->GetPlayerNum();

				//斥力床の中心とプレイヤーとの距離
				Vector3 diff = player->GetPosition() - m_position;
				//立方体の範囲に入れば斥力を与える
				if (fabsf(diff.x) < REPULSION_AREA_LENGTH && fabsf(diff.z) < REPULSION_AREA_LENGTH)
				{
					if (diff.y < REPULSION_RANGE)//床との距離が近ければ上昇
					{
						Vector3 repulsionValue = REPULSION_SPEED;
						player->ExecuteCharacon(repulsionValue);

						//落下の加速をリセット
						player->ResetFall();
					}

					//SE

					//プレイヤーが斥力床に入ってきた時の音を鳴らす。
					if (m_isPlayEnterSE[playerNum] == false)
					{
						SoundOneShotPlay(L"Assets/sound/マジックウェーブ.wav", SOUND_SE_REPULSION_ENTER_VOLUME);
						m_isPlayEnterSE[playerNum] = true;
					}

					//プレイヤーが斥力床に乗っている時の音を鳴らす。
					if (m_isPlayStaySE[playerNum] == false) {
						
						m_repulsionStaySE[playerNum]->SetVolume(SOUND_SE_REPULSION_STAY_VOLUME);
						m_repulsionStaySE[playerNum]->Play(true);
						m_isPlayStaySE[playerNum] = true;
					}
					//StaySEがまだ鳴っているのならば、音量を少しずつ増加させる。
					m_staySEVolume[playerNum] += SOUND_SE_INCREASE_VALUE;
					if (m_staySEVolume[playerNum] >= SOUND_SE_REPULSION_STAY_VOLUME) {
						m_staySEVolume[playerNum] = SOUND_SE_REPULSION_STAY_VOLUME;
					}
					m_repulsionStaySE[playerNum]->SetVolume(m_staySEVolume[playerNum]);

				}

				//範囲に入っていないとき
				else if (fabsf(diff.x) >= REPULSION_AREA_LENGTH || fabsf(diff.z) >= REPULSION_AREA_LENGTH)
				{
					//EnterSEのFlagをリセット
					if (m_isPlayEnterSE[playerNum] == true)
					{
						m_isPlayEnterSE[playerNum] = false;
					}

					//StaySEを少しずつフェードアウトさせる。
					if (m_isPlayStaySE[playerNum] == true)
					{
						m_staySEVolume[playerNum] -= SOUND_SE_DECAY_VALUE;
						m_repulsionStaySE[playerNum]->SetVolume(m_staySEVolume[playerNum]);
						//音が完全に消えたら、StaySEの再生を停止
						if (m_staySEVolume[playerNum] <= 0.0f)
						{
							m_staySEVolume[playerNum] = 0.0f;
							m_repulsionStaySE[playerNum]->Stop();
							m_isPlayStaySE[playerNum] = false;
						}
					}
				}

			return true;
		}
	);
		
}