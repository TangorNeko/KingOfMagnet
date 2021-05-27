#pragma once
class Flash:public IGameObject
{
	~Flash();
	bool Start();
	void Update();

	prefab::CSpriteRender* m_spriteRender = nullptr;

	Vector3 m_position_sprite;

	float m_Alpha = 1.0;

	bool m_flashFlag = false;

	//エフェクト
	prefab::CEffect* m_effect = nullptr;

	bool m_deleteFlag = false;
public:
	Vector3 m_position = {0.0f,0.0f,0.0f};

	int m_parentNum;
};

