#include "stdafx.h"
#include "PopRandItem.h"
#include "MagInversion_item.h"
#include "MagAcceleration_item.h"
#include "Grenade_item.h"
#include "Machinegun_item.h"
PopRandItem::~PopRandItem()
{

}
bool PopRandItem::Start()
{
	//item = Grenade;
	
	return true;
}
void PopRandItem::Update()
{
	ItemSelect();
	//enitem = Gun;
	ItemPop();
}
void PopRandItem::ItemSelect()
{
	m_gatya = rand()%1000;
	if (m_gatya < m_MagInversion_probability + m_MagAcceleration_probability + m_Grenade_probability+m_Gun_probability)
	{
		enitem = Gun;
	}
	if (m_gatya < m_MagInversion_probability + m_MagAcceleration_probability+ m_Grenade_probability)
	{
		enitem = Grenade;
	}
	if (m_gatya < m_MagInversion_probability + m_MagAcceleration_probability)
	{
		enitem = MagAcceleration;
	}
	if (m_gatya < m_MagInversion_probability)
	{
		enitem = MagInversion;
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
			//é•óÕîΩì]ÇçÏê¨
			item = NewGO<MagInversion_item>(0, "maginversion_item");
			item->m_position = { ItemSetPosition() };
			item->m_ItemCount++;
			break;
		case MagAcceleration:
			item = NewGO<MagAcceleration_item>(0, "acceleration_item");
			item->m_position = { ItemSetPosition() };
			item->m_ItemCount++;
			break;
		case Grenade:
			item = NewGO<Grenade_item>(0, "grenade_item");
			item->m_position = { ItemSetPosition() };
			item->m_ItemCount++;
			break;
		
		case Gun:
			item = NewGO<Machinegun_item>(0, "gun_item");
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
	RandPosition.x = (rand() % 2000)-1000;
	RandPosition.z = ((rand() * m_gatya) % 2000)-1000;
	RandPosition.y = 0.0f;
	return RandPosition;
}