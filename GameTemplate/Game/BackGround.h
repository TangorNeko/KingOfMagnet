#pragma once
#include "PopRandItem.h"
class BackGround : public IGameObject
{
	~BackGround();
	bool Start() override;
	void Update()override;

	Vector3 m_position = { 0.0f,0.0f,0.0f };
	
	PopRandItem* popranditem = nullptr;

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	prefab::CPointLight* m_pLig = nullptr;
	PhysicsStaticObject m_physicsStaticObject;
public:
	bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint);
};

