#include "stdafx.h"
#include "PopRandItem.h"
#include "MagInversion_item.h"
#include "MagAcceleration_item.h"
PopRandItem::~PopRandItem()
{

}
bool PopRandItem::Start()
{
	item = MagAcceleration;

	return true;
}
void PopRandItem::Update()
{
	ItemSelect();
	ItemPop();
}
void PopRandItem::ItemSelect()
{
	m_gatya = rand()%1000;
	/*if (m_gatya < m_MagInversion_probability)
	{
		item = MagInversion;
	}
	if (m_gatya < m_MagInversion_probability + m_MagAcceleration_probability)
	{
		item = MagAcceleration;
	}*/
}

void PopRandItem::ItemPop()
{
	m_Loop++;
	if (m_Loop > m_PopInterval)
	{
		MagInversion_item* inversion;
		MagAcceleration_item* acceleration;

		m_Loop = 0;
		switch (item)
		{
		case MagInversion:
			//é•óÕîΩì]ÇçÏê¨
			inversion = NewGO<MagInversion_item>(0, "maginversion_item");
			inversion->m_position = { ItemSetPosition() };
			inversion->m_ItemCount++;
			break;
		case MagAcceleration:
			acceleration = NewGO<MagAcceleration_item>(0, "acceleration_item_item");
			acceleration->m_position = { ItemSetPosition() };
			acceleration->m_ItemCount++;
			break;
		
		/*default:
			break;*/
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