#pragma once
class Explosion :public IGameObject
{
	~Explosion();
	bool Start();
	void Update();
public:

	Vector3 m_position;

	int m_deletetime = 500;

	//エフェクト
	prefab::CEffect* m_effect = nullptr;
};
