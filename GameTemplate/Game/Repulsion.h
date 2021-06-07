#pragma once
class GameScene;

class Repulsion : public IGameObject
{
	~Repulsion();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;

	Vector3 diff;

	GameScene* m_gameScene = nullptr;

	//SE‚ğ–Â‚ç‚µ‚½‚©‚Ç‚¤‚©(2ƒvƒŒƒCƒ„[•ª)
	bool m_isPlayss1[2] = { false,false };	//ss1

	bool m_isPlayss2[2] = { false,false };	//ss2
	prefab::CSoundSource* m_ss2[2] = { nullptr,nullptr };
	float m_ss2Volume[2] = { 1.0f,1.0f };
public:
	Vector3 m_position;	
	Quaternion m_rot;
};

