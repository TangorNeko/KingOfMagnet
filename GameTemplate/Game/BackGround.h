#pragma once
class BackGround : public IGameObject
{
	~BackGround();
	bool Start() override;
	void Update()override;

	Vector3 m_position = { 0.0f,0.0f,0.0f };

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
};

