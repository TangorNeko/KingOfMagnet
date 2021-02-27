#pragma once
class ShowSprite : public IGameObject
{
	~ShowSprite();
	bool Start();
	void Update();
	void Render(RenderContext& rc, Camera* camera);

	prefab::CSpriteRender* m_spriteRender = nullptr;
};

