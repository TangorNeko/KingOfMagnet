#pragma once

class ShowModel : public IGameObject
{
	~ShowModel();
	bool Start() override;
	void Update()override;

public:

	Vector3 m_position = { 0.0f,0.0f,0.0f };
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };
	Vector3 m_dir = { 0.0f,0.0f,1.0f };
	Vector3 m_toCamera = { 0.0f,100.0f,-100.0f };
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	prefab::CPointLight* m_pointLight = nullptr;
	CharacterController m_charaCon;
	int m_playerNum = -1;
	int m_magPower;//���́A�Ȃ�(0)�A���͏��(-1,-2)�A�˗͏��(1,2)
	int m_stealthCount = 0;
	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };//���͂��o�Ă��錴�_
	float m_charge = 0;
	float m_deg = 0;
	bool m_isLock = false;
public:
	int GetMagPower() { return m_magPower; }
	ShowModel* m_enemy = nullptr;
};
