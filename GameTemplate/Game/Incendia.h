#pragma once

class GameScene;

class Incendia :public IGameObject
{
	~Incendia();
	bool Start();
	void Update();

private:

	//ダメージを受けるまでのカウント(2キャラ分)
	int m_damageCountdown[2] = {0,0};

	//燃え続ける時間
	int m_deleteCountdown = 180;

	//エフェクト
	prefab::CEffect* m_effect = nullptr;

	//ゲームシーン
	GameScene* m_gameScene = nullptr;

	bool m_deleteFlag = false;
public:
	Vector3 m_position;
};
