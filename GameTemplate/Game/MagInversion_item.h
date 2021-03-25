#pragma once
#include "DropItem_base.h"
class MagInversion_item:public DropItem_base
{
	~MagInversion_item();
	bool Start();
	void Update();
public:
	Vector3 topPos;
};

