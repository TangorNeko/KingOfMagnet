#pragma once
#include "TriangleCollider.h"

class ShowModel : public IGameObject
{
	~ShowModel();
	bool Start() override;
	void Update()override;

public:
	Quaternion rot;
	Vector3 m_position = { 0.0f,0.0f,0.0f };
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };
	Vector3 m_dir = { 0.0f,0.0f,1.0f };
	Vector3 m_toCamera = { 0.0f,100.0f,-100.0f };
	Vector3 Scale = { 0.5,0.5,0.5 };	

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	prefab::CPointLight* m_pointLight = nullptr;
	prefab::CFontRender* m_fontRender = nullptr;
	CharacterController m_charaCon;
	TriangleCollider m_collider;
	int m_playerNum = -1;
	int m_magPower;//磁力、なし(0)、引力状態(-1,-2)、斥力状態(1,2)
	int m_stealthCount = 0;
	int m_hp = 1000;
	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };//磁力が出ている原点
	float m_charge = 0;
	float m_deg = 0;
	bool m_isLock = false;
public:
	int GetMagPower() { return m_magPower; }
	ShowModel* m_enemy = nullptr;
};
