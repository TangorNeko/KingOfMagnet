#pragma once
#include "MyCapsuleCollider.h"

class Player;
class BackGround;

class Debris : public IGameObject
{
	~Debris();
	bool Start() override;
	void Update() override;

	//地面に落ちている時の挙動
	void AsDropBehave();

	//弾として発射されている時の挙動
	void AsBulletBehave();

	//プレイヤーに保持されている時の挙動
	void AsHoldBehave();

public:
	//ガレキの状態
	enum enDebrisState
	{
		enDrop,//落ちている。
		enBullet,//弾として発射されている
		enHold,//プレイヤーが保持している
	};

	enDebrisState m_debrisState = enDrop;

	//ガレキの形
	enum enDebrisShape
	{
		enStone,//石
		enSword,//剣
		enGrenade,//爆弾
		//ここから他のアイテム(魔法の杖)など追加していく?
	};

	enDebrisShape m_debrisShape = enStone;

	Vector3 m_position;//座標
	Vector3 m_oldPosition;//前フレームの座標

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//ガレキのモデル

	Player* m_parent = nullptr;//親のプレイヤー(ホールド時、発射時に使用)

	MyCapsuleCollider m_bulletCollider;//プレイヤーとの当たり判定用のカプセル状の当たり判定

	BackGround* m_stageModel = nullptr;//当たり判定用のステージのクラス

	Vector3 m_moveDirection = { 0.0f,0.0f,0.0f };//移動する方向
	const float m_velocity = 50.0f;//弾速
};

