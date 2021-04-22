#pragma once

class GameScene;

class PopRandItem:public IGameObject
{
	~PopRandItem();
	bool Start();
	void Update();
	//アイテムが出る間隔
	int m_PopInterval=200;
	//アイテムが出る確率、千分率
	int m_MagInversion_probability = 250;
	int m_MagAcceleration_probability = 250;
	int m_Grenade_probability = 250;
	int m_GravityGrenade = 100;
	int m_Gun_probability = 250;
public:
	GameScene* m_gameScene = nullptr;	
	int m_Loop = 0;


	enum EnItem {
		MagInversion,		//磁力反転
		MagAcceleration,	//磁力遷移の加速
		Gun,				//マシンガン
		ExplosionGrenade,	//爆破グレネード
		FlashGrenade,		//閃光グレネード
		GravityGrenade,		//引力弾
		MoveActStockRecover,	//ムーブアクションのストックを回復
		ItemNum
	};
	EnItem enitem = Gun;
	
	int m_ItemCount = ItemNum;//アイテム数
	int m_gatya=0;
	int m_positionX = 0;
	int m_positionZ = 0;
	
	Vector3 ItemSetPosition();
	void ItemSelect();
	void ItemPop();
};

