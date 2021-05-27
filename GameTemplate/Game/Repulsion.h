#pragma once
class SampleScene;

class Repulsion : public IGameObject
{
	~Repulsion();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;

	Vector3 diff;

	SampleScene* m_gameScene = nullptr;

	//SEを鳴らしたかどうか(2プレイヤー分)
	bool m_isPlayss1[2] = { false,false };	//ss1

	bool m_isPlayss2[2] = { false,false };	//ss2
	prefab::CSoundSource* m_ss2[2] = { nullptr,nullptr };
	float m_ss2Volume[2] = { 1.0f,1.0f };
public:
	Vector3 m_position;	
	Quaternion m_rot;
};

