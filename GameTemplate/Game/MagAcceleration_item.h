#pragma once
#include "DropItem_base.h"
class MagAcceleration_item :public DropItem_base
{
	~MagAcceleration_item();
	bool Start();
	void Update();
public:
	Vector3 topPos;
};
