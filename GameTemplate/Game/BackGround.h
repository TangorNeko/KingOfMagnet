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
	std::vector<Vector3> m_respawnPoints;//���X�|�[���n�_�̌����i�[����R���e�i
public:
	bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint);

	//�󂯎�����n�_(�G�̈ʒu)����ł��������X�|�[���n�_��Ԃ��B
	Vector3 GetRespawnPoint(const Vector3& enemyPos);
};

