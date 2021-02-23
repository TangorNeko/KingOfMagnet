#pragma once
class ShowModel : public IGameObject
{
	~ShowModel();
	bool Start() override;
	void Update()override;

public:

	Vector3 m_position = { 0.0f,0.0f,0.0f };
	Vector3 m_ligPos[2] = { { -50.0f,50.0f,0.0f }, {50.0f,50.0f,0.0f} };
	Vector3 m_toCamera = { 0.0f,100.0f,-100.0f };
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	prefab::CDirectionLight* yellowfromside = nullptr;
	prefab::CSpotLight* m_SpotLig[2] = { nullptr };
	CharacterController m_charaCon;
	float m_deg = 0;
};
