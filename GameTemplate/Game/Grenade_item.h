#pragma once
#include "DropItem_base.h"
class Grenade_item :public DropItem_base
{
	~Grenade_item();
	bool Start();
	void Update();
public:
	Vector3 topPos;
	
	
};
