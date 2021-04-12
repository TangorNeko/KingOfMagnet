#pragma once

class PopRandItem:public IGameObject
{
	~PopRandItem();
	bool Start();
	void Update();
	//アイテムが出る間隔
	int m_PopInterval=500;
	//アイテムが出る確率、千分率
	int m_MagInversion_probability = 100;
	int m_MagAcceleration_probability = 100;
	int m_Grenade_probability = 100;
	int m_Gun_probability = 500;
public:
	
	int m_Loop = 0;

	enum enItem {
		MagInversion,
		MagAcceleration,
		Grenade,
		Gun,
		ItemNum
	};
	enItem enitem = Gun;
	
	int m_ItemCount = ItemNum;//アイテム数
	int m_gatya=0;
	int m_positionX = 0;
	int m_positionZ = 0;
	
	Vector3 ItemSetPosition();
	void ItemSelect();
	void ItemPop();
};

