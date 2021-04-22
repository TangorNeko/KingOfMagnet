#pragma once
#include "Character_base.h"
#include "GameScene.h"

class DropItem_base :public IGameObject

{
	
protected:
	public:	

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//キャラクターのモデル
	MyCapsuleCollider m_collider;
	Vector3 m_position;
	Vector3 m_scale = { 1.0f,1.0f,1.0f};
	Quaternion m_qrot;
	float angle = 1.0f;//アイテムの回転スピード
	
	int m_ExistenceMax;//マップに存在する数の上限
	int m_ItemCount=0;//アイテムの数

	bool m_Upmotion=true;
	void ItemMotion();//アイテムの動き

	float m_startpositionY;

	GameScene* m_gameScene = nullptr;
};

