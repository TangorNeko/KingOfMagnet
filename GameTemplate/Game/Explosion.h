#pragma once
class Explosion :public IGameObject
{
	~Explosion();
	bool Start();
	void Update();

	//�G�t�F�N�g
	prefab::CEffect* m_effect = nullptr;

	bool m_deleteFlag = false;
public:
	Vector3 m_position;
};
