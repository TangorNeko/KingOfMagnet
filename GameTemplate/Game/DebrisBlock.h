#pragma once

class GameScene;

class DebrisBlock : public IGameObject
{
	~DebrisBlock();
	bool Start() override;
	void Update() override;

public:
	/**
	 * @brief ガレキブロックの座標を設定
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//モデル
	Vector3 m_position = { 0.0f,0.0f,0.0f };				//座標
	int m_factoryTimer = 0;									//引力バーストされた時の生成タイマー
	GameScene* m_gameScene = nullptr;						//ゲームシーン
};

