#pragma once

class PopRandItem:public IGameObject
{
	~PopRandItem();
	bool Start();
	void Update();
public:
	int m_PopInterval=500;
	int m_Loop = 0;

	enum Item {
		MagInversion,
		MagAcceleration,
		ItemNum
	};
	Item item = MagInversion;
	int m_MagInversion_probability = 100;
	int m_MagAcceleration_probability = 100;
	int m_ItemCount = ItemNum;//ÉAÉCÉeÉÄêî
	int m_gatya=0;
	int m_positionX = 0;
	int m_positionZ = 0;
	
	Vector3 ItemSetPosition();
	void ItemSelect();
	void ItemPop();
};

