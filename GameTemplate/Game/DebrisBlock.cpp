#include "stdafx.h"
#include "DebrisBlock.h"

#include <random>
#include "Player.h"
#include "GameScene.h"
#include "Debris.h"
#include "Bomb.h"

namespace
{
	const int FACTORY_TIMER_CAN_CREATE = 0;				//生成タイマー:生成可能
	const int FACTORY_TIMER_AFTER_CREATE = 90;			//生成タイマー:生成直後
	const float FACTORY_MAX_CREATE_DISTANCE = 500.0f;	//近くでプレイヤーが引力バーストしていた時弾を生成する最大距離
	const int FACTORY_BULLET_CREATE_NUM = 5;			//一回の生成で作られる弾数
	const int BULLET_LOTTERY_MOD_VALUE = 100;			//ガレキを生成するか爆弾を生成するかのランダム抽選のための余剰演算値
	const int BULLET_LOTTERY_DEBRIS = 85;				//ガレキを生成するか爆弾を生成するかのランダム抽選のガレキ側のしきい値
	const int DEBRIS_SHAPE_LOTTRY_MOD_VALUE = 100;		//ガレキの中で何の種類の弾を生成するかのランダム抽選のための余剰演算値
	const int DEBRIS_SHAPE_LOTTRY_SCRAP = 75;			//ガレキの中で通常弾を生成する時のしきい値
	const int DEBRIS_SHAPE_LOTTRY_SWORD = 90;			//ガレキの中で剣を生成する時のしきい値
	const int BOMG_SHAPE_LOTTERY_MOD_VALUE = 100;		//爆弾の中でどの種類の爆弾を生成するかのランダム抽選のための余剰演算値
	const int BOMG_SHAPE_LOTTERY_GRENADE = 33;			//爆弾の中でグレネードを生成する時のしきい値
	const int BOMG_SHAPE_LOTTERY_FLASHGRENADE = 66;		//爆弾の中で閃光弾を生成する時のしきい値
}
DebrisBlock::~DebrisBlock()
{
	DeleteGO(m_skinModelRender);
}

bool DebrisBlock::Start()
{
	//モデルを作成
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/scrap.tkm");

	//ゲームシーンを検索
	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}

void DebrisBlock::Update()
{
	//ポーズ中かリザルトシーンならスキップ。
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause || m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		return;
	}

	//生成タイマーを減らしていく。
	m_factoryTimer--;
	if (m_factoryTimer < FACTORY_TIMER_CAN_CREATE)
	{
		m_factoryTimer = FACTORY_TIMER_CAN_CREATE;
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			Vector3 toPlayer = player->GetPosition() - m_position;

			//引力バーストしているプレイヤーが距離500以内に近づき、タイマーが上がっていて、ゲームに存在できる最大数未満であれば新しくガレキを生成
			if (player->GetMagnetState() == MAGNETSTATE_GRAVITY && player->IsBurst() == true && m_factoryTimer == FACTORY_TIMER_CAN_CREATE && toPlayer.Length() < FACTORY_MAX_CREATE_DISTANCE)
			{
				for (int i = 0; i < FACTORY_BULLET_CREATE_NUM; i++)//弾を生成する数
				{
					if (m_gameScene->GetBulletNum() < m_gameScene->MAXBULLETNUM)
					{
						//ゲームシーンの弾の数を増やす。
						//弾のStart関数で増やすと生成フレームの後に増やされるので制限数を超えてしまう。
						m_gameScene->BulletIncrease();

						std::random_device device;
						std::mt19937_64 rnd(device());

						//第一抽選、ガレキか、爆弾か?
						int bulletLottery = rnd() % BULLET_LOTTERY_MOD_VALUE;//0~99

						if (bulletLottery <= BULLET_LOTTERY_DEBRIS)
						{
							//ガレキ
							Debris* debris = NewGO<Debris>(0, "debris");
							debris->SetDebrisState(Debris::enDrop);
							debris->SetPosition(m_position);

							//第二抽選、ガレキの形状は?
							int shapeLottery = rnd() % DEBRIS_SHAPE_LOTTRY_MOD_VALUE;//0~99
							if (shapeLottery <= DEBRIS_SHAPE_LOTTRY_SCRAP)
							{
								//スクラップ
								debris->SetDebrisShape(Debris::enScrap);
							}
							else if (shapeLottery <= DEBRIS_SHAPE_LOTTRY_SWORD)
							{
								//剣
								debris->SetDebrisShape(Debris::enSword);
							}
							else
							{
								//必殺技チャージ
								debris->SetDebrisShape(Debris::enSpecialCharger);
							}
						}
						else
						{
							//爆弾
							Bomb* bomb = NewGO<Bomb>(0, "bomb");
							bomb->SetBombState(Bomb::enDrop);
							bomb->SetPosition(m_position);

							//第二抽選、爆弾の形状は?
							int shapeLottery = rnd() % BOMG_SHAPE_LOTTERY_MOD_VALUE;//0~99
							if (shapeLottery <= BOMG_SHAPE_LOTTERY_GRENADE)
							{
								//手榴弾
								bomb->SetBombShape(Bomb::enGrenade);
							}
							else if (shapeLottery <= BOMG_SHAPE_LOTTERY_FLASHGRENADE)
							{
								//フラッシュグレネード
								bomb->SetBombShape(Bomb::enFlashGrenade);
							}
							else
							{
								//焼夷弾
								bomb->SetBombShape(Bomb::enIncendiaryGrenade);
							}
						}
					}
					else
					{
						break;
					}
				}
				//生成タイマーをセット
				m_factoryTimer = FACTORY_TIMER_AFTER_CREATE;
				
			}
			return true;
		});

	m_skinModelRender->SetPosition(m_position);
}