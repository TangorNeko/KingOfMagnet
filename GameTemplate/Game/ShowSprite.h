#pragma once
class ShowSprite : public IGameObject
{
	~ShowSprite();
	bool Start();
	void Update();

	prefab::CSpriteRender* m_spriteRender = nullptr;
};

