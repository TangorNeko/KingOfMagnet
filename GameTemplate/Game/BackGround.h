#pragma once
#include "PopRandItem.h"
class Repulsion;
class BackGround : public IGameObject
{
	~BackGround();
	bool Start() override;
	void Update()override;

	Vector3 m_position = { 0.0f,0.0f,0.0f };

	prefab::CLevel m_level;
	std::vector<Repulsion*> m_repulsion;

	//PopRandItem* popranditem = nullptr;

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	prefab::CPointLight* m_pLig = nullptr;
	PhysicsStaticObject m_physicsStaticObject;
	std::vector<Vector3> m_respawnPoints;//リスポーン地点の候補を格納するコンテナ
public:
	bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint);

	//受け取った地点(敵の位置)から最も遠いリスポーン地点を返す。
	Vector3 GetRespawnPoint(const Vector3& enemyPos);
};

