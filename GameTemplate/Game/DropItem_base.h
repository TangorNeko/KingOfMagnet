#pragma once
#include "Character_base.h"
class DropItem_base :public IGameObject

{
protected:
	public:	

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//キャラクターのモデル
	MyCapsuleCollider m_collider;
	Vector3 m_position;
	Quaternion m_qrot;
	float angle = 1.0f;//アイテムの回転スピード
	
	int m_ExistenceMax;//マップに存在する数の上限
	int m_ItemCount=0;//アイテムの数

	bool m_Upmotion=true;
	void ItemMotion();//アイテムの動き
};

