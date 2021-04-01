#pragma once
class Flash:public IGameObject
{
	~Flash();
	bool Start();
	void Update();
public:
	prefab::CSpriteRender* m_spriteRender = nullptr;
	Vector3 m_position_sprite;
	Vector3 m_scale_sprite;
	int m_affectPlayerNum;
	int m_loopcount;
	int m_deletetime = 500;
};

