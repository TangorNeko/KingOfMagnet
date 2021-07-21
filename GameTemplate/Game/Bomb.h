#pragma once
#include "MyCapsuleCollider.h"

class Player;
class BackGround;
class GameScene;

class Bomb : public IGameObject
{
	~Bomb() override;
	bool Start() override;
	void Update() override;

	/**
	 * @brief 地面に落ちている時の挙動
	*/
	void AsDropBehave();

	/**
	 * @brief 弾として発射されている時の挙動
	*/
	void AsBulletBehave();

	/**
	 * @brief プレイヤーに保持されている時の挙動
	*/
	void AsHoldBehave();

	/**
	 * @brief 何かに当たった後の挙動
	*/
	void AsPopBehave();
public:
	//爆弾の状態
	enum enBombState
	{
		enDrop,//落ちている。
		enBullet,//弾として発射されている
		enHold,//プレイヤーが保持している
		enPop,//何かに当たった　爆発寸前
	};

	//爆弾の種類
	enum enBombShape
	{
		enGrenade,//爆弾
		enFlashGrenade,//閃光弾
		enIncendiaryGrenade,//焼夷弾
	};

	/**
	 * @brief 爆弾の座標を設定
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief 爆弾の座標を取得
	 * @return 
	*/
	const Vector3& GetPosition() const { return m_position; }

	/**
	 * @brief 爆弾の状態を設定
	 * @param state 爆弾の状態
	*/
	void SetBombState(enBombState state) { m_bombState = state; }

	/**
	 * @brief 爆弾の種類を設定
	 * @param shape 種類
	*/
	void SetBombShape(enBombShape shape) { m_bombShape = shape; }

	/**
	 * @brief 爆弾の移動方向を設定
	 * @param direction 移動方向
	*/
	void SetMoveDirection(const Vector3& direction)
	{
		m_moveDirection = direction;
		m_moveDirection.Normalize();
	}
	
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//爆弾のモデル
	Vector3 m_position = Vector3::Zero;						//座標
	Vector3 m_oldPosition = Vector3::Zero;					//前フレームの座標
	Vector3 m_moveDirection = { 1.0f,0.0f,0.0f };			//移動する方向
	enBombState m_bombState = enDrop;						//爆弾の状態
	enBombShape m_bombShape = enGrenade;					//爆弾の種類
	bool m_isOnGround = false;								//地面についているかどうか
	MyCapsuleCollider m_bulletCollider;						//プレイヤーとの当たり判定用のカプセル状の当たり判定
	BackGround* m_stageModel = nullptr;						//当たり判定用のステージのクラス
	GameScene* m_gameScene = nullptr;						//ゲームシーン
	int m_explosionCount = 0;								//爆発までのカウント
	Player* m_parent = nullptr;								//親のプレイヤー(ホールド時、発射時に使用)
};

