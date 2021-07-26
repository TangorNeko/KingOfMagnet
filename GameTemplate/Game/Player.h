#pragma once
#include "MyCapsuleCollider.h"
#include "TriangleCollider.h"

class BackGround;
class Debris;
class Bomb;
class MobiusGauge;
class GameScene;

class Player : public IGameObject
{
public:
	~Player() override;
private:
	bool Start() override;
	void Update() override;

	/**
	 * @brief 開始前カメラ移動
	*/
	void OpeningCamera();

	/**
	 * @brief 体力、メビウスゲージの表示
	*/
	void DisplayStatus();

	/**
	 * @brief 移動
	*/
	void Move();

	/**
	 * @brief 攻撃
	*/
	void Attack();

	/**
	 * @brief 必殺技
	*/
	void SpecialAttack();

	/**
	 * @brief 爆弾を投げる
	*/
	void ThrowBomb();

	/**
	 * @brief 保持しているガレキを浮遊させる。
	*/
	void HoldDebris();

	/**
	 * @brief 保持している爆弾を浮遊させる。
	*/
	void HoldBomb();

	/**
	 * @brief 磁力バーストを使用していない時の磁力による動き
	*/
	void MagneticBehavior();

	/**
	 * @brief 磁力バースト
	*/
	void MagneticBurst();

	/**
	 * @brief 磁力の変化
	*/
	void ChangeMagnetPower();

	/**
	 * @brief カメラの移動
	*/
	void Camera();

	/**
	 * @brief 当たり判定
	*/
	void Collision();

	/**
	 * @brief 攻撃状態に切り替えできたら切り替える。
	*/
	void TryChangeStatusAttack();

	/**
	 * @brief 特殊攻撃状態に切り替える
	*/
	void TryChangeStatusSpecialAttack();

	/**
	 * @brief バースト状態に切り替える
	*/
	void TryChangeStatusBurst();

	/**
	 * @brief 走り状態に切り替えできたら切り替える。
	*/
	void TryChangeStatusRun();

	/**
	 * @brief 歩き状態に切り替えできたら切り替える。
	*/
	void TryChangeStatusWalk();

	/**
	 * @brief 落下状態に切り替える
	*/
	void TryChangeStatusFall();

	/**
	 * @brief 待機状態に切り替えできたら切り替える。
	*/
	void TryChangeStatusIdle();

	/**
	 * @brief 被弾状態に切り替える
	*/
	void TryChangeStatusHit();

	/**
	 * @brief アニメーションの状態更新
	*/
	void UpdateState();

	/**
	 * @brief アニメーションを再生
	*/
	void AnimationSelect();

	/**
	 * @brief 発射先の計算。
	 * @param crossPoint 求めた発射先(参照受け取り)
	 * @return 発射先が求まったか
	*/
	bool GetShootPoint(Vector3& crossPoint);

	/**
	 * @brief ファイナルカメラ
	*/
	void FinalHit();

	/**
	 * @brief ノックバックをする。
	*/
	void KnockBack();
public:

	//自分の体力にダメージを与える
	void Damage(int damage);

	//勝利した時
	void Win();

	//敗北した時
	void Lose();

	//必殺技ゲージをチャージする。
	void ChargeSpecialAttackGauge(int charge);

	/**
	 * @brief ダメージエフェクトの正面をセットする
	 * @param front 正面の向き
	*/
	void SetDamageEffectFront(const Vector3& front)
	{
		m_damageEffectFront = front;
	}

	/**
	 * @brief ノックバックフラグをセット
	 * @param flag フラグ
	*/
	void SetKnockBackFlag(bool flag)
	{
		m_isKnockBack = flag;
	}

	/**
	 * @brief 引力弾の攻撃フラグを取得する
	 * @return 攻撃フラグ
	*/
	bool GetGravityAttackFlag()
	{
		return m_isGravityBulletAttack;
	}

	/**
	 * @brief 敵プレイヤーをセットする
	 * @param enemy 敵プレイヤー
	*/
	void SetEnemy(Player* enemy)
	{
		m_enemy = enemy;
	}

	/**
	 * @brief 磁力が出る地点を取得する
	 * @return 磁力が出る座標
	*/
	const Vector3& GetMagPosition()
	{
		return m_magPosition;
	}

	/**
	 * @brief プレイヤー番号を取得
	 * @return プレイヤー番号
	*/
	int GetPlayerNum()
	{
		return m_playerNum;
	}

	/**
	 * @brief プレイヤー番号をセットする
	 * @param playerNum プレイヤー番号
	*/
	void SetPlayerNum(int playerNum)
	{
		m_playerNum = playerNum;
	}

	/**
	 * @brief 磁力の状態を取得する
	 * @return -1:引力　1:斥力
	*/
	int GetMagnetState()
	{
		return m_magnetState;
	}

	/**
	 * @brief 磁力の状態をセットする
	 * @param magPower -1:引力 1:斥力
	*/
	void SetMagnetState(int magPower)
	{
		m_magnetState = magPower;
	}

	/**
	 * @brief バースト中かを取得する
	 * @return バースト中?
	*/
	bool IsBurst()
	{
		return m_isBurst;
	}

	/**
	 * @brief 現在の体力を取得する
	 * @return 現在の体力
	*/
	int GetHP()
	{
		return m_hp;
	}

	/**
	 * @brief 弾が自分の当たり判定に当たっているかを取得する
	 * @param bulletCollider 弾のカプセルコライダー
	 * @return 当たったか?
	*/
	bool IsBulletHitCollider(const MyCapsuleCollider& bulletCollider)
	{
		return m_collider.isHitCapsule(bulletCollider);
	}

	/**
	 * @brief キャラコンを実行させる
	 * @param moveSpeed 移動速度
	*/
	void ExecuteCharacon(Vector3 moveSpeed)
	{
		m_charaCon.Execute(moveSpeed, 1.0f);
	}

	/**
	 * @brief 重力加速をリセット
	*/
	void ResetFall()
	{
		m_fallLoop = 0;
	}

	/**
	 * @brief カメラ感度をセット
	 * @param sensitivity 
	*/
	void SetSensitivity(float sensitivity)
	{
		m_sensitivity = sensitivity;
	}

	/**
	 * @brief カメラの正面をセット
	 * @param front カメラの正面
	*/
	void SetCameraFront(const Vector3& front)
	{
		m_cameraFront = front;
	}

	/**
	 * @brief カメラの正面を取得
	 * @return 
	*/
	const Vector3& GetCameraFront()
	{
		return m_cameraFront;
	}

	/**
	 * @brief カメラへの向きをセット
	 * @param direction カメラへの向き
	*/
	void SetToCameraDirection(const Vector3& direction)
	{
		m_toCameraDir = direction;
	}

	/**
	 * @brief プレイヤーの座標をセット
	 * @param position プレイヤーの座標
	*/
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/**
	 * @brief プレイヤーの座標を取得
	 * @return プレイヤーの座標
	*/
	const Vector3& GetPosition()
	{
		return m_position;
	}

	/**
	 * @brief プレイヤーの移動量をセット
	 * @param moveAmount 移動量
	*/
	void SetMoveAmount(const Vector3& moveAmount)
	{
		m_moveAmount = moveAmount;
	}

	/**
	 * @brief スキンモデルレンダー
	 * @return スキンモデルレンダーを返す
	*/
	prefab::CSkinModelRender* GetSkinModelRender() { return m_skinModelRender; }

	/**
	 * @brief キャラが最後に向いた向きを取得
	 * @return キャラが最後に向いた向き
	*/
	const Vector3& GetLastFrontDir()const  {return m_LastFrontDir;}

	/**
	 * @brief 負けたかどうかを取得
	 * @return 負けた?
	*/
	bool IsLose() { return m_Lose; }
	
public:
	//アニメーションの数
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,
		enAnimationClip_Fall,
		enAnimationClip_SpecialAttack,
		enAnimationClip_Burst,
		enAnimationClip_Hit,
		enAnimationClip_Death,
		enAnimationClip_Winner,
		enAnimationClip_num,			//列挙内で使う要素の数を表すダミー
	};

	//状態の数
	enum EnStatus {
		enStatus_Attack,		//攻撃状態
		enStatus_Run,			//走り状態
		enStatus_Idle,			//待機状態
		enStatus_Walk,			//歩き状態
		enStatus_Move,			//移動アクション状態		
		enStatus_Fall,			//落下状態
		enStatus_SpecialAttack,	//必殺技状態
		enStatus_Burst,			//バースト状態
		enStatus_Hit,			//被弾状態		
		enStatus_Num,			//状態の数。
	};

public:
	//ガレキ、爆弾の操作系
	std::vector<Debris*> m_holdDebrisVector;//保持しているガレキが格納されるコンテナ
	float m_holdDebrisRotateDeg = 0;//保持しているガレキの回転角度

	std::vector<Bomb*> m_holdBombVector;//保持している爆弾が格納されるコンテナ
	int m_selectBombNo = 0;//選択している爆弾の番号

	prefab::CFontRender* m_bulletNumFont = nullptr;//残弾数
	prefab::CFontRender* m_bulletMaxFont = nullptr;//残弾数
private:

	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//キャラクターのモデル
	prefab::CSpriteRender* m_crosshairRender = nullptr;			//照準のスプライト
	prefab::CSpotLight* m_spotLight = nullptr;					//プレイヤーを後ろから照らすライト

	int m_playerNum = -1;										//プレイヤーの番号 1P(0)、2P(1)
	int m_magnetState = -1;										//磁力、引力状態(-1)、斥力(1)
	int m_hp = 1000;											//体力
	float m_magnetCharge = 1000;								//磁力ゲージの現在の量

	Vector3 m_position = { 0.0f,0.0f,0.0f };					//キャラクターの座標
	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };					//磁力が出ている地点
	Quaternion m_rot;											//キャラクターの回転
	Vector3 m_scale = { 0.8f, 0.8f, 0.8f };						//キャラクターの拡大率 **定数化**

	CharacterController m_charaCon;								//プレイヤーのキャラコン
	Vector3 m_moveAmount = { 0.0f,0.0f,0.0f };					//キャラコンに実行させる移動量
	int m_fallLoop = 0;											//落下制御用のループカウント
	float m_characterSpeed = 6.0;								//キャラクターの移動速度

	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };				//プレイヤーからカメラへの向き
	Vector3 m_cameraFront;										//カメラの前方向
	Vector3 m_cameraRight;										//カメラの右方向
	Vector3 m_cameraPos;										//カメラのポジション
	Quaternion m_cameraQRotY;									//プレイヤーカメラのY軸の回転
	float m_sensitivity = 2.0f;									//視点感度
	bool m_isLockon = false;									//カメラロックオンしているか。


	BackGround* m_stageModel = nullptr;							//背景のモデル(当たり判定用)
	TriangleCollider m_triCollider[2];							//単純な三角形の当たり判定(発射先の判定に使う)
	MyCapsuleCollider m_collider;								//カプセル状の当たり判定(弾の当たり判定に使う)
	
	bool m_isBurst = false;										//バーストしているか。
	int m_burstCount = 0;										//バーストを使用してからの経過フレーム
	bool m_isAttacking = false;									//攻撃を発射しているか。
	int m_attackCount = 0;										//攻撃を発射してからの経過フレーム(隙)
	bool m_isSteal = false;										//一回の引力バースト中にすでに敵の弾を奪ったか

	//アニメーション関連
	AnimationClip animationClips[enAnimationClip_num];			//アニメーションクリップ
	EnStatus m_animStatus = enStatus_Idle;						//現在のアニメーションの状態
	bool m_SpecialAttackOn = false;								//アニメーション用フラグ
	bool m_HitOn = false;										//被弾したかどうか

	//必殺技関連
	int m_specialShotFlag = false;								//必殺技を発射中?
	int m_specialShotCount = 0;									//必殺技を発射し始めてからの経過フレーム
	int m_specialAttackGauge = 0;								//必殺技のゲージ量
	int m_oldSpecialAttackGauge = 0;							//1フレーム前の必殺技ゲージ量
	bool m_isGravityBulletAttack = false;						//引力の必殺技の攻撃タイミングを指示する用変数。

	//ノックバック関連
	bool m_isKnockBack = false;									//ノックバック状態?
	int m_knockBackCount = 0;									//ノックバックし始めてからの経過フレーム

	//体力等の情報表示
	prefab::CSpriteRender* m_HPBarSpriteRender = nullptr;		//HPバーのスプライト
	prefab::CSpriteRender* m_HPBarDarkSpriteRender = nullptr;	//HPバーの黒い(体力がなくなった)部分のスプライト
	prefab::CSpriteRender* m_DamageBarSpriteRender = nullptr;	//受けたダメージ分のHPバーのスプライト
	bool m_hpBarRedFlag = false;								//HPバーを赤くするか
	MobiusGauge* m_mobiusGauge = nullptr;						//メビウスゲージ
	prefab::CFontRender* m_chargeSPFontRender = nullptr;		//必殺技ゲージの溜まり具合のフォント

	//エフェクト
	prefab::CEffect* m_magEffect[2] = { nullptr,nullptr };		//磁力のエフェクト　2つのエフェクトを連続で再生し続ける。
	int m_magEffectCallCount = 41;								//磁力エフェクトを呼ぶまでの時間
	prefab::CEffect* m_burstEffect = nullptr;					//磁力バーストのエフェクト
	prefab::CEffect* m_hitEffect = nullptr;						//ダメージエフェクト
	Vector3 m_damageEffectFront = { 0.0f,0.0f,0.0f };			//ダメージを食らった方向　エフェクトをダメージが食らった向きに
	prefab::CEffect* m_SPFireEffect = nullptr;						//必殺技の発射エフェクト
	prefab::CEffect* m_SPGaugeMaxEffect = nullptr;				//必殺ゲージが最大まで溜まった時に出るエフェクト
	prefab::CEffect* m_SPChargeEffectRed = nullptr;				//必殺技を放つ溜め動作のときのエフェクト　斥力
	prefab::CEffect* m_SPChargeEffectBlue = nullptr;			//必殺技を放つ溜め動作のときのエフェクト　引力

	//音
	int m_footstepsTimer = 0;									//足音の鳴る間隔をカウントする

	//勝敗表示
	prefab::CSpriteRender* m_resultWinnerSprite = nullptr;		//勝者を表示するスプライト(○P)
	prefab::CSpriteRender* m_resultWinSprite = nullptr;			//勝利のスプライト(WIN!)

	GameScene* m_gameScene = nullptr;							//ゲームシーン
	Player* m_enemy = nullptr;									//敵プレイヤー

	//ファイナルヒット関連
	bool m_Lose = false;										//自分が負けたかどうか
	bool m_FirstTime = true;									//一度だけループさせるためのフラグ	
	Vector3 m_LastFrontDir;										//キャラが最後に向いた向き			
};



