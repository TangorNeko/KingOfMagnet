//磁力状態を反転させるアイテム
#include "stdafx.h"
#include "MagInversion_item.h"

MagInversion_item::~MagInversion_item()
{
	DeleteGO(m_skinModelRender);
}
bool MagInversion_item::Start()
{
	//モデルを作成
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/SandClockRed.tkm");
	m_skinModelRender->SetScale({ 3.0f, 3.0f, 3.0f });
	m_collider.SetStartPoint(m_position);
	topPos = m_position;
	topPos.y += 50;
	m_collider.SetEndPoint(topPos);
	m_collider.SetRadius(15.0f);
	m_skinModelRender->SetPosition(m_position);
	
	return true;
}
void MagInversion_item::Update()
{	
	//各プレイヤーを検索
	QueryGOs<Character_base>("Player", [this](Character_base* player)->bool
	{
		//プレイヤーが近ければ
		if (player->m_collider.isHitCapsule(m_collider))
		{
			//反転させる
			player->m_magPower += 2;
			if (player->m_magPower == 3)
				player->m_magPower = -1;
			if (player->m_magPower == 4)
				player->m_magPower = -2;
			if (player->m_magPower == 0)
				player->m_magPower = 0;
			DeleteGO(this);
		}
		return true;
	});
	ItemMotion();
}