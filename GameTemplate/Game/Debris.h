#pragma once
#include "MyCapsuleCollider.h"

class Player;
class BackGround;
class GameScene;

class Debris : public IGameObject
{
	~Debris();
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
	 * @brief 弾として発射されたガレキがプレイヤーにヒットした時
	 * @param player ヒットしたプレイヤー
	*/
	void PlayerHitAsBullet(Player* player);

	/**
	 * @brief 弾として発射されたガレキがステージにヒットした時
	 * @param crossPoint ヒットした座標
	*/
	void StageHitAsBullet(const Vector3& crossPoint);

	/**
	 * @brief プレイヤーに保持されている時の挙動
	*/
	void AsHoldBehave();

	/**
	 * @brief 何かに当たった後の挙動
	*/
	void AsPopBehave();


public:
	//ガレキの状態
	enum enDebrisState
	{
		enDrop,//落ちている。
		enBullet,//弾として発射されている
		enHold,//プレイヤーが保持している
		enPop,//何かに当たった後
	};

	//ガレキの形
	enum enDebrisShape
	{
		enScrap,//石
		enSword,//剣
		//ここから他のアイテム(魔法の杖)など追加していく?
		enSpecialCharger,
	};

	/**
	 * @brief ガレキの座標を設定
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief ガレキの座標を取得
	 * @return 座標
	*/
	const Vector3& GetPosition() const { return m_position; }

	/**
	 * @brief ガレキの状態を設定
	 * @param state ガレキの状態
	*/
	void SetDebrisState(enDebrisState state) { m_debrisState = state; }

	/**
	 * @brief ガレキの状態を取得
	 * @return ガレキの状態
	*/
	enDebrisState GetDebrisState() { return m_debrisState; }

	/**
	 * @brief ガレキの形状を設定
	 * @param shape ガレキの形状
	*/
	void SetDebrisShape(enDebrisShape shape) { m_debrisShape = shape; }

	/**
	 * @brief ガレキを所有するプレイヤーを取得
	 * @param player プレイヤー
	*/
	void SetParent(Player* player) { m_parent = player; }

	/**
	 * @brief ガレキの移動方向を設定
	 * @param direction 移動方向 
	*/
	void SetMoveDirection(const Vector3& direction) 
	{
		m_moveDirection = direction;
		m_moveDirection.Normalize();
	}

	/**
	 * @brief ガレキが地面に存在するかのフラグを設定
	 * @param flag フラグ
	*/
	void SetOnGroundFlag(bool flag) { m_isOnGround = flag; }
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//ガレキのモデル
	Vector3 m_position;										//座標
	Vector3 m_oldPosition;									//前フレームの座標
	Vector3 m_moveDirection = { 1.0f,0.0f,0.0f };			//移動する方向
	enDebrisState m_debrisState = enDrop;					//ガレキの状態
	enDebrisShape m_debrisShape = enScrap;					//ガレキの形状
	bool m_isOnGround = false;								//地面についているかどうか
	MyCapsuleCollider m_bulletCollider;						//プレイヤーとの当たり判定用のカプセル状の当たり判定
	BackGround* m_stageModel = nullptr;						//当たり判定用のステージのクラス
	GameScene* m_gameScene = nullptr;						//ゲームシーン
	//スペシャルチャージャー用
	int m_specialChargeCount = 0;							//スペシャルゲージを増やすまでのカウント
	Player* m_parent = nullptr;								//親のプレイヤー(ホールド時、発射時に使用)
};

