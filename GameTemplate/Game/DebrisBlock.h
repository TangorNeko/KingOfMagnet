#pragma once

class GameScene;

class DebrisBlock : public IGameObject
{
	~DebrisBlock();
	bool Start() override;
	void Update() override;

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//モデル

	int m_factoryTimer = 0;//引力バーストされた時の生成タイマー

	GameScene* m_gameScene = nullptr;//ゲームシーン
public:
	Vector3 m_position = { 0.0f,0.0f,0.0f };//座標
};

