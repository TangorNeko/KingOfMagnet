#pragma once

class GameScene;

class Flash:public IGameObject
{
	~Flash() override;
	bool Start() override;
	void Update() override;

public:

	/**
	 * @brief 爆発の座標を設定する
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief フラッシュを使用したプレイヤーを設定
	 * @param playerNum フラッシュを使用したプレイヤーのプレイヤー番号
	*/
	void SetParentNum(const int playerNum) { m_parentNum = playerNum; }

private:
	prefab::CSpriteRender* m_spriteRender = nullptr;	//フラッシュのスプライト
	float m_Alpha = 1.0f;								//フラッシュのスプライトの透過率
	int m_parentNum = -1;								//フラッシュを使用したプレイヤーのプレイヤー番号
	bool m_flashFlag = false;							//フラッシュは発動した?
	prefab::CEffect* m_effect = nullptr;				//フラッシュ爆発のエフェクト
	Vector3 m_position = Vector3::Zero;					//爆発の座標
	bool m_deleteFlag = false;							//削除するか?(爆発は終わったか?)
	GameScene* m_gameScene = nullptr;					//ゲームシーン
};

