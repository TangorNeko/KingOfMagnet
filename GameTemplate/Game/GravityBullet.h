#pragma once
#include "MyCapsuleCollider.h"

class Debris;
class Player;
class BackGround;
class GameScene;

class GravityBullet : public IGameObject
{
	~GravityBullet();
	bool Start() override;
	void Update() override;

	/**
	 * @brief 発射されている途中の挙動
	*/
	void AsBulletBehave();

	/**
	 * @brief 着弾した瞬間の挙動
	*/
	void AsExplodeBehave();

	/**
	 * @brief 着弾した後の挙動
	*/
	void AsGravityBehave();

	/**
	 * @brief フィニッシュ攻撃中の挙動
	*/
	void AsFinishBehave();

public:

	/**
	 * @brief 引力弾の座標を設定
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief 引力弾の座標を取得
	 * @return 座標
	*/
	Vector3 GetPosition() { return m_position; }

	/**
	 * @brief 引力弾の移動方向を設定
	 * @param moveDirection 移動方向
	*/
	void SetMoveDirection(const Vector3& moveDirection) { m_moveDirection = moveDirection; }

	/**
	 * @brief 引力弾を発射したプレイヤーを設定
	 * @param parent プレイヤー
	*/
	void SetParent(Player* parent) { m_parent = parent; }


private:
	//引力弾の状態
	enum enGravityBulletState
	{
		enBullet,//発射中
		enExplode,//爆発した瞬間
		enGravity,//引き寄せながら周囲のガレキを浮かせている
		enFinish,//フィニッシュ攻撃中
	};

	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//引力弾のモデル
	prefab::CEffect* m_gravityEffect = nullptr;				//エフェクト
	prefab::CEffect* m_gravityEffect2 = nullptr;			//エフェクト
	prefab::CEffect* m_inflateEffect = nullptr;				//膨張エフェクト
	prefab::CEffect* m_wearingEffect = nullptr;				//発射時に弾が纏うエフェクト
	Vector3 m_position = { 0.0f,0.0f,0.0f };				//座標
	Vector3 m_oldPosition;									//前フレームの座標
	Vector3 m_moveDirection = { 0.0f,0.0f,0.0f };			//移動する方向
	float angle = 0.0f;										//モデルの角度
	enGravityBulletState m_gravityBulletState = enBullet;	//引力弾の状態
	std::vector<Debris*> m_controlDebrisVector;				//コントロールするガレキを格納するコンテナ
	int m_gravityTimeCount = 0;								//引力で拘束する時間のカウント
	MyCapsuleCollider m_bulletCollider;						//プレイヤーとの当たり判定用のカプセル状の当たり判定
	BackGround* m_stageModel = nullptr;						//当たり判定用のステージのクラス
	GameScene* m_gameScene = nullptr;						//ゲームシーン
	Player* m_parent = nullptr;								//親のプレイヤー(爆発時に使用)
};

