#pragma once
class ShowModel : public IGameObject
{
	~ShowModel();
	bool Start() override;
	void Update()override;

	Vector3 m_position = { 0.0f,0.0f,0.0f };

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
};
