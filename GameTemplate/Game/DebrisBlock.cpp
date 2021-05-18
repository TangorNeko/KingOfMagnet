#include "stdafx.h"
#include "DebrisBlock.h"

#include <random>
#include "Player.h"
#include "SampleScene.h"
#include "Debris.h"
#include "Bomb.h"

DebrisBlock::~DebrisBlock()
{
	DeleteGO(m_skinModelRender);
}

bool DebrisBlock::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/yuka.tkm");

	m_gameScene = FindGO<SampleScene>("gamescene");

	return true;
}

void DebrisBlock::Update()
{
	//生成タイマーを減らしていく。
	m_factoryTimer--;
	if (m_factoryTimer < 0)
	{
		m_factoryTimer = 0;
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			Vector3 toPlayer = player->m_position - m_position;

			//引力バーストしているプレイヤーが距離500以内に近づき、タイマーが上がっていて、ゲームに存在できる最大数未満であれば新しくガレキを生成
			if (player->m_magPower == -1 && player->m_isBurst == true && m_factoryTimer <= 0 && toPlayer.Length() < 500.0f && m_gameScene->GetBulletNum() < m_gameScene->MAXBULLETNUM)
			{
				//ゲームシーンの弾の数を増やす。
				//弾のStart関数で増やすと生成フレームの後に増やされるので制限数を超えてしまう。
				m_gameScene->BulletIncrease();

				std::random_device device;
				std::mt19937_64 rnd(device());

				//第一抽選、ガレキか、爆弾か?
				int bulletLottery = rnd() % 100;//0~99

				if (bulletLottery <= 90)
				{
					//ガレキ
					Debris* debris = NewGO<Debris>(0, "debris");
					debris->m_debrisState = Debris::enDrop;
					debris->m_position = m_position;

					//第二抽選、ガレキの形状は?
					int shapeLottery = rnd() % 100;//0~99
					if (shapeLottery <= 75)
					{
						//石
						debris->m_debrisShape = Debris::enStone;
					}
					else if (shapeLottery <= 90)
					{
						//剣
						debris->m_debrisShape = Debris::enSword;
					}
					else
					{
						//必殺技チャージ
						debris->m_debrisShape = Debris::enSpecialCharger;
					}
				}
				else
				{
					//爆弾
					Bomb* bomb = NewGO<Bomb>(0, "bomb");
					bomb->m_bombState = Bomb::enDrop;
					bomb->m_position = m_position;

					//第二抽選、爆弾の形状は?
					int shapeLottery = rnd() % 100;//0~99
					if (shapeLottery <= 33)
					{
						//手榴弾
						bomb->m_bombShape = Bomb::enGrenade;
					}
					else if (shapeLottery <= 66)
					{
						//フラッシュグレネード
						bomb->m_bombShape = Bomb::enFlashGrenade;
					}
					else
					{
						//焼夷弾
						bomb->m_bombShape = Bomb::enIncendiaryGrenade;
					}
				}

				//生成タイマーをセット
				m_factoryTimer = 180;
				
			}
			return true;
		});

	m_skinModelRender->SetPosition(m_position);
}