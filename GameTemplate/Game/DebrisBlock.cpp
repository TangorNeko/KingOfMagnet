#include "stdafx.h"
#include "DebrisBlock.h"

#include <random>
#include "Player.h"
#include "Debris.h"

DebrisBlock::~DebrisBlock()
{
	DeleteGO(m_skinModelRender);
}

bool DebrisBlock::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/yuka.tkm");

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

			//引力バーストしているプレイヤーが距離500以内に近づき、タイマーが上がっていれば新しくガレキを生成
			//TODO:数の制限もつける
			if (player->m_magPower == -1 && player->m_isBurst == true && m_factoryTimer <= 0 && toPlayer.Length() < 500.0f)
			{
				Debris* debris = NewGO<Debris>(0, "debris");
				debris->m_debrisState = Debris::enDrop;
				debris->m_debrisShape = Debris::enStone;
				debris->m_position = m_position;

				//生成タイマーをセット
				m_factoryTimer = 180;
				
			}
			return true;
		});

	m_skinModelRender->SetPosition(m_position);
}