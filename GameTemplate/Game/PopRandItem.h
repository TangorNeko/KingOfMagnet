#pragma once

class PopRandItem:public IGameObject
{
	~PopRandItem();
	bool Start();
	void Update();
	//�A�C�e�����o��Ԋu
	int m_PopInterval=2000;
	//�A�C�e�����o��m���A�番��
	int m_MagInversion_probability = 250;
	int m_MagAcceleration_probability = 250;
	int m_Grenade_probability = 250;
	int m_Gun_probability = 250;
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
	
	int m_ItemCount = ItemNum;//�A�C�e����
	int m_gatya=0;
	int m_positionX = 0;
	int m_positionZ = 0;
	
	Vector3 ItemSetPosition();
	void ItemSelect();
	void ItemPop();
};

