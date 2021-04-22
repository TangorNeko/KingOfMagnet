#pragma once
#include "DropItem_base.h"
class FlashGrenade_item :public DropItem_base
{
	~FlashGrenade_item();
	bool Start();
	void Update();
public:
	Vector3 topPos;	
};
