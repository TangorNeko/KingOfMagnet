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

	//発射されている途中の挙動
	void AsBulletBehave();

	//着弾した瞬間の挙動
	void AsExplodeBehave();

	//着弾した後の挙動
	void AsGravityBehave();

	//フィニッシュ攻撃中の挙動
	void AsFinishBehave();

private:
	//引力弾の状態
	enum enGravityBulletState
	{
		enBullet,//発射中
		enExplode,//爆発した瞬間
		enGravity,//引き寄せながら周囲のガレキを浮かせている
		enFinish,//フィニッシュ攻撃中
	};

	enGravityBulletState m_gravityBulletState = enBullet;//引力弾の状態
	
	Vector3 m_oldPosition;//前フレームの座標

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//引力弾のモデル

	MyCapsuleCollider m_bulletCollider;//プレイヤーとの当たり判定用のカプセル状の当たり判定

	BackGround* m_stageModel = nullptr;//当たり判定用のステージのクラス

	GameScene* m_gameScene = nullptr;

	const float m_velocity = 30.0f;//弾速

	std::vector<Debris*> m_controlDebrisVector;//コントロールするガレキを格納するコンテナ

	int m_gravityTimeCount = 0;//引力で拘束する時間のカウント

	const float m_bacuumSpeed = 6.5f;//プレイヤーを引き寄せるスピード。

	prefab::CEffect* m_effect = nullptr;	//エフェクト
	prefab::CEffect* m_effect2 = nullptr;	//エフェクト
	prefab::CEffect* m_inflateEffect = nullptr;	//膨張エフェクト
	prefab::CEffect* m_wearingEffect = nullptr;	//発射時に弾が纏うエフェクト

	float angle = 0.0f;//モデルの角度

public:
	Vector3 m_position = { 0.0f,0.0f,0.0f };//座標

	Player* m_parent = nullptr;//親のプレイヤー(爆発時に使用)

	Vector3 m_moveDirection = { 0.0f,0.0f,0.0f };//移動する方向
};

