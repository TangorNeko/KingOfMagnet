#pragma once
#include "MyCapsuleCollider.h"
#include "TriangleCollider.h"

class BackGround;
class Debris;
class Bomb;
class MobiusGauge;
class SampleScene;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
private:

	//開始前カメラ移動
	void OpeningCamera();

	//体力、メビウスゲージの表示
	void DisplayStatus();

	//移動
	void Move();

	//攻撃
	void Attack();

	//必殺技
	void SpecialAttack();
	bool m_SpecialAttackOn = false;//アニメーション用フラグ
	int m_specialShotFlag = false;
	int m_specialShotCount = 0;

	//爆弾を投げる
	void ThrowBomb();

	//保持しているガレキを浮遊させる。
	void HoldDebris();

	//保持している爆弾を浮遊させる。
	void HoldBomb();

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

	//特殊攻撃状態に切り替える
	void TryChangeStatusSpecialAttack();

	//走り状態に切り替えできたら切り替える。
	void TryChangeStatusRun();

	//歩き状態に切り替えできたら切り替える。
	void TryChangeStatusWalk();

	//落下状態に切り替える
	void TryChangeStatusFall();

	//待機状態に切り替えできたら切り替える。
	void TryChangeStatusIdle();

	//被弾状態に切り替える
	void TryChangeStatusHit();
	bool m_HitOn = false;//被弾したかどうか
	int m_Hitcount = 30;//被弾したときに流れるアニメーションのフレーム数

	//死亡状態に切り替える
	void TryChangeStatusDeath();
	
	//勝利状態に切り替える
	void TryChangeStatusWin();

	//アニメーションの状態更新
	void UpdateState();

	//アニメーションを再生
	void AnimationSelect();

	//発射先の計算。
	bool GetShootPoint(Vector3& crossPoint);

	//ライト
	prefab::CSpotLight* m_spotLight = nullptr;


public:

	//自分の体力にダメージを与える
	void Damage(int damage);

	//勝利した時
	void Win();

	//敗北した時
	void Lose();

	//リザルト表示
	//void ResultDisplay();
	void FinalHit();

	//必殺技ゲージをチャージする。
	void ChargeSpecialAttackGauge(int charge);

	//ノックバックをする。
	void KnockBack();

public:
	Vector3 m_position = { 0.0f,0.0f,0.0f }; //キャラクターの座標
	Quaternion m_rot;//キャラクターの回転
	Vector3 m_scale = { 0.8f, 0.8f, 0.8f };//キャラクターの拡大率
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//キャラクターの移動速度
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//キャラクターの向き
	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };//カメラへの向き

	Vector3 m_front;//カメラの前方向
	Vector3 right;//カメラの右方向
	Vector3 cameraPos;//カメラのポジション
	Quaternion qRotY;
	float n;//内積
	float angle;//アークコサイン

	Vector3 m_cameraPos;
	Vector3 m_targetPos = { 0.0f,0.0f,0.0f };
	float gain = 10;//カメラとターゲットとの距離	
	float m_addY = 0.0f;
	int m_cameraLoopCount = 0;
	bool m_opning = true;

	int m_fallLoop = 0;//落下制御用のループカウント
	float m_characterSpeed = 6.0;//キャラクターの移動速度

	bool m_isLock = false; //ロックしているか。

	SampleScene* m_gameScene = nullptr;

	BackGround* m_stageModel;//背景のモデル(当たり判定用)
	CharacterController m_charaCon;//プレイヤーのキャラコン
	TriangleCollider m_triCollider[2];//単純な三角形の当たり判定(発射先の判定に使う)
	MyCapsuleCollider m_collider;//カプセル状の当たり判定(弾の当たり判定に使う)

	prefab::CSkinModelRender* m_skinModelRender = nullptr; //キャラクターのモデル
	prefab::CFontRender* m_bulletNumber = nullptr;//残弾数
	prefab::CFontRender* m_resultFontRender = nullptr;//リザルト表示用のフォント

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

	std::vector<Bomb*> m_holdBombVector;//保持している爆弾が格納されるコンテナ
	int m_selectBombNo = 0;//選択している爆弾の番号

	//アニメーションの数
	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,
		enAnimationClip_Fall,
		enAnimationClip_SpecialAttack,
		enAnimationClip_Hit,
		enAnimationClip_Death,
		enAnimationClip_Winner,
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
		enStatus_SpecialAttack,//必殺技状態
		enStatus_Hit,		//被弾状態
		enStatus_Death,		//死亡状態
		enStatus_Winner,		//勝利状態
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

	//メビウスゲージ
	MobiusGauge* m_mobiusGauge = nullptr;

	//リザルト関連
	Vector2 m_resultPos = { -1200.0f,100.0f };
	bool m_resultFirstTime = true;
	bool m_displayOff = false;
	float m_AttackNum = 0;//攻撃回数	
	float m_TakeAttackNum = 0; //攻撃を受けた回数
	float m_HitRate = 0;//命中率
	int m_BurstNum = 0;//バースト回数
	int m_StealNum = 0;//敵の弾を奪った回数
	int m_LandingNum = 0;//落ちた回数
	int m_ReceivedDamage = 0;//受けたダメージ
	int m_SaveSP = 0;//溜まった必殺技ポイント

	//ファイナルヒット関連
	bool m_Lose = false;
	bool m_LoseCameraFlag = true;
	bool m_doryInOn = true;
	bool m_WinAnimOn = false;
	int m_LoseCameraLoop = 0;
	bool m_FirstTime = true;
	int m_winnerNum = 0;
	int m_loserNum = 0;
	int m_LastCameraStatus = 0;
	float m_coef = 0.0f;
	Vector3 m_LastFront;

	Vector3 m_enemyWaistPos;//ダメージエフェクト関連
	Vector3 m_damegeEffectFront = {0.0f,0.0f,0.0f};

	//斥力・引力エフェクト関連
	prefab::CEffect* m_magEffect[3] = { nullptr,nullptr,nullptr };		//三つのエフェクトを連続で再生し続ける。
	int m_magEffectCallCount = 60;		//磁力エフェクトを呼ぶまでの時間

	//斥力・引力バースト エフェクト関連
	prefab::CEffect* m_burstEffect = nullptr;

	//ダメージエフェクト
	prefab::CEffect* m_hitEffect = nullptr;

	//必殺技のエフェクト
	prefab::CEffect* m_SPEffect = nullptr;
};


