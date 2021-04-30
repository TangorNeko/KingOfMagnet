#pragma once
class GameScene;

class Repulsion : public IGameObject
{
	~Repulsion();
	bool Start();
	void Update();
public:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;

	Vector3 m_position;
	Vector3 m_scale = Vector3::One;
	Vector3 diff;
	Vector3 diff1;
	Quaternion m_rot;


	GameScene* m_gameScene = nullptr;
};

