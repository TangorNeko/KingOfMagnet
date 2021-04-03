#pragma once
#include "DropItem_base.h"
class Machinegun_item:public DropItem_base
{
	~Machinegun_item();
	bool Start();
	void Update();
public:
	Vector3 topPos;
};

