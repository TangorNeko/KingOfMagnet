#pragma once
class DamageDisplay:public IGameObject
{
	bool Start();
	void Update();
	~DamageDisplay();

public:

	/**
	 * @brief ダメージが発生した座標を設定する
	 * @param damagePos 座標
	*/
	void SetDamagePosition(const Vector3& damagePos) { m_damagePosition = damagePos; }

	/**
	 * @brief ダメージを受けたプレイヤーの敵の番号を設定する
	 * @param playerNum 敵のプレイヤーの番号
	*/
	void SetEnemyNum(const int playerNum) { m_enemyNum = playerNum; }

	/**
	 * @brief 表示するダメージ量を設定
	 * @param damage ダメージ量
	*/
	void SetDamage(const int damage) { m_damage = damage; }

private:
	prefab::CFontRender* m_damageFontRender = nullptr;	//ダメージ量のフォント
	int m_damage = 0;									//表示するダメージ量
	std::wstring damageText;							//表示するダメージ量をテキストに変換したもの	
	Vector3 m_damagePosition;							//ダメージが発生した座標
	Vector2 m_screenDamagePosition;						//ダメージが発生した座標を画面上の座標に変換したもの
	int m_enemyNum;										//ダメージを受けたプレイヤーの敵の番号(ダメージを受けた時、敵のプレイヤーの画面に表示するので)
	int m_loopCount = 0;								//表示し始めてから何フレーム経ったか
	bool m_isxSpeedPositive;							//ダメージのフォントが右に移動するか?(falseなら左に移動)
	float m_xPlusValue = 0;								//フォントが表示される座標にプラスされるx座標の量
	float m_xMoveSpeed;									//フォントのx方向の移動速度
	float m_yPlusValue = 0;								//フォントが表示される座標にプラスされるy座標の量
	float m_yMoveSpeed = -10;							//フォントのy方向の移動速度
};
