#pragma once
#include "Dropitem_base.h"
class GravityGrenade_item : public DropItem_base
{
	~GravityGrenade_item();
	bool Start();
	void Update();
public:
	Vector3 topPos;
};

