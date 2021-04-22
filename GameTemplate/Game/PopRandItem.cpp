//ランダムにアイテムを出現させる
#include "stdafx.h"
#include "PopRandItem.h"
#include "MagInversion_item.h"
#include "MagAcceleration_item.h"
#include "FlashGrenade_item.h"
#include "Machinegun_item.h"
#include "GravityGrenade_item.h"
#include "GameScene.h"
PopRandItem::~PopRandItem()
{
	DeleteGO(this);
}
bool PopRandItem::Start()
{
	//item = Grenade;

	m_gameScene = FindGO<GameScene>("gamescene");
	
	return true;
}
void PopRandItem::Update()
{
	ItemSelect();
	if (m_gameScene->GetGameEndFlag() == false)
	{
		ItemPop();
	}
}
void PopRandItem::ItemSelect()
{
	//千分率で確率を調整
	m_gatya = rand()%1000;
	if (m_gatya < m_MagInversion_probability + m_MagAcceleration_probability + m_Grenade_probability + m_Gun_probability + m_GravityGrenade)
	{
		enitem = Gun;//マシンガン
	}
	if (m_gatya < m_MagInversion_probability + m_MagAcceleration_probability + m_Grenade_probability+m_Gun_probability)
	{
		enitem = GravityGrenade;
	}
	if (m_gatya < m_MagInversion_probability + m_MagAcceleration_probability+ m_Grenade_probability)
	{
		enitem = FlashGrenade;	}
	if (m_gatya < m_MagInversion_probability + m_MagAcceleration_probability)
	{
		enitem = MagAcceleration;//磁力加速
	}
	if (m_gatya < m_MagInversion_probability)
	{
		enitem = MagInversion;//磁力反転
	}
	
	
	
}

void PopRandItem::ItemPop()
{
	m_Loop++;
	if (m_Loop > m_PopInterval)
	{
		DropItem_base* item;
		m_Loop = 0;
		switch (enitem)
		{
		case MagInversion:
			//磁力反転を作成
			item = NewGO<MagInversion_item>(0, "maginversion_item");
			item->m_position = { ItemSetPosition() };
			item->m_ItemCount++;
			break;
		case MagAcceleration:
			item = NewGO<MagAcceleration_item>(0, "acceleration_item");
			item->m_position = { ItemSetPosition() };
			item->m_ItemCount++;
			break;
		case FlashGrenade:
			item = NewGO<FlashGrenade_item>(0, "falshgrenade_item");
			item->m_position = { ItemSetPosition() };
			item->m_ItemCount++;
			break;
		
		case Gun:
			item = NewGO<Machinegun_item>(0, "gun_item");
			item->m_position = { ItemSetPosition() };
			item->m_scale = { 0.5f,0.5f,0.5f };
			item->m_ItemCount++;
			break;

		case GravityGrenade:
			item = NewGO<GravityGrenade_item>(0, "gravitygrenade_item");
			item->m_position = { ItemSetPosition() };
			item->m_scale = { 0.5f,0.5f,0.5f };
			item->m_ItemCount++;
			break;
		}
	}	
}
Vector3 PopRandItem::ItemSetPosition()
{
	Vector3 RandPosition;
	//ランダムな座標にアイテムを置く
	RandPosition.x = (rand() % 2000)-1000;
	RandPosition.z = ((rand() * m_gatya) % 2000)-1000;
	RandPosition.y = 0.0f;
	return RandPosition;
}