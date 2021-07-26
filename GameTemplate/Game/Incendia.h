#pragma once

class GameScene;

class Incendia :public IGameObject
{
	~Incendia() override;
	bool Start() override;
	void Update() override;
	
public:

	/**
	 * @brief 爆発の座標を設定する
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	prefab::CEffect* m_effect = nullptr;		//炎上のエフェクト
	int m_damageCountdown[2] = {0,0};			//ダメージを受けるまでのカウント(2キャラ分)	
	int m_deleteCountdown = 180;				//燃え続ける時間
	Vector3 m_position = { 0.0f,0.0f,0.0f };	//爆発の座標
	bool m_deleteFlag = false;					//削除するか?(爆発は終わったか?)
	GameScene* m_gameScene = nullptr;			//ゲームシーン
};
