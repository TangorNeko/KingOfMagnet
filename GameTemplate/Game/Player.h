#pragma once
#include "MyCapsuleCollider.h"
#include "TriangleCollider.h"

class BackGround;
class Debris;

class Player : public IGameObject
{
	~Player();
	bool Start() override;
	void Update() override;

	//体力、メビウスゲージの表示
	void DisplayStatus();

	//移動
	void Move();

	//攻撃
	void Attack();

	//必殺技
	void SpecialAttack();

	//保持しているガレキを浮遊させる。
	void HoldDebris();

	//磁力バーストを使用していない時の通常の動き
	void MagneticBehavior();

	//磁力バースト
	void MagneticBurst();

	//磁力の変化
	void ChangeMagnetPower();

	//カメラの移動
	void Camera();

	//当たり判定
	void Collision();

	//攻撃状態に切り替えできたら切り替える。
	void TryChangeStatusAttack();

	//走り状態に切り替えできたら切り替える。
	void TryChangeStatusRun();

	//歩き状態に切り替えできたら切り替える。
	void TryChangeStatusWalk();

	//落下状態に切り替える
	void TryChangeStatusFall();

	//待機状態に切り替えできたら切り替える。
	void TryChangeStatusIdle();

	//アニメーションの状態更新
	void UpdateState();

	//アニメーションを再生
	void AnimationSelect();

	//発射先の計算。
	bool GetShootPoint(Vector3& crossPoint);

public:

	//自分の体力にダメージを与える
	void Damage(int damage);

	//勝利した時
	void Win();

	//敗北した時
	void Lose();

	//必殺技ゲージをチャージする。
	void ChargeSpecialAttackGauge(int charge);

	//ノックバックをする。
	void KnockBack();

public:
	Vector3 m_position = { 0.0f,0.0f,0.0f }; //キャラクターの座標
	Quaternion rot;//キャラクターの回転
	Vector3 m_scale = { 0.8f, 0.8f, 0.8f };//キャラクターの拡大率
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//キャラクターの移動速度
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//キャラクターの向き
	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };//カメラへの向き

	Vector3 front;//カメラの前方向
	Vector3 right;//カメラの右方向
	Vector3 cameraPos;//カメラのポジション
	Quaternion qRotY;
	float n;//内積
	float angle;//アークコサイン

	int m_fallLoop = 0;//落下制御用のループカウント
	float m_characterSpeed = 6.0;//キャラクターの移動速度

	bool m_isLock = false; //ロックしているか。

	BackGround* m_stageModel;//背景のモデル(当たり判定用)
	CharacterController m_charaCon;//プレイヤーのキャラコン
	TriangleCollider m_triCollider[2];//単純な三角形の当たり判定(発射先の判定に使う)
	MyCapsuleCollider m_collider;//カプセル状の当たり判定(弾の当たり判定に使う)

	prefab::CSkinModelRender* m_skinModelRender = nullptr; //キャラクターのモデル
	prefab::CFontRender* m_statusFontRender = nullptr;//ステータス表示用のフォント
	prefab::CSpriteRender* m_resultSpriteRender = nullptr; //勝敗時のスプライト
	prefab::CSpriteRender* m_crosshairRender = nullptr; //照準のスプライト

	int m_playerNum = -1;//プレイヤーの番号 1P(0)、2P(1)
	int m_magPower;//磁力、引力状態(-1)、斥力(1)
	float m_charge = 1000;//磁力ゲージの現在の量
	int m_hp = 1000;//体力
	bool m_isBurst = false;//バーストしているか。
	int m_burstCount = 0;//バーストの持続時間
	bool m_isAttacking = false; //攻撃を発射しているか。
	int m_attackCount = 0;//攻撃の隙で移動速度が落ちている時間。
	bool m_isSteal = false;//一回の引力バースト中にすでに敵の弾を奪ったか

	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };//磁力が出ている原点

	std::vector<Debris*> m_holdDebrisVector;//保持しているガレキが格納されるコンテナ
	float m_holdDebrisRotateDeg = 0;//保持しているガレキの回転角度

	//アニメーションの数
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,
		enAnimationClip_Fall,
		enAnimationClip_num,  //列挙内で使う要素の数を表すダミー
	};

	//状態の数
	enum EnStatus {
		enStatus_Attack,	//攻撃状態
		enStatus_Run,		//走り状態
		enStatus_Idle,		//待機状態
		enStatus_Walk,		//歩き状態
		enStatus_Move,		//移動アクション状態		
		enStatus_Fall,		//落下状態
		enStatus_Num,		//状態の数。
	};

	AnimationClip animationClips[enAnimationClip_num];//アニメーションクリップ
	EnStatus m_animStatus = enStatus_Idle;	//現在の状態。

	Player* m_enemy = nullptr; //敵

	int m_specialAttackGauge = 0;//必殺技のゲージ
	bool m_isGravityBulletAttack = false;//引力の必殺技の攻撃タイミングを指示する用変数。

	//ノックバック関連
	bool m_isKnockBack = false;
	int m_isknockBackCount = 0;

	//HPバー
	prefab::CSpriteRender* m_HPBarSpriteRender = nullptr;
	prefab::CSpriteRender* m_HPBarDarkSpriteRender = nullptr;

	//動かせるかどうか
	bool m_canMove = false;

};

