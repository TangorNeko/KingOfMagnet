#pragma once
#include "MyCapsuleCollider.h"
class BackGround;

class Character_base : public IGameObject
{
public:
	//bool Start() override;	
	//体力、チャージ、磁力の状態等の表示
	virtual void DisplayStatus()=0;

	//移動アクション
	virtual void MoveAction() = 0;

	//通常攻撃
	virtual void NormalAttack() = 0;	

protected:
	//磁力の定期的な変化
	void ChangeMagnetPower();

	//チャージ
	virtual void Charge() = 0;

	//敵に向く当たり判定を作る
	void Collision();

	//カメラの移動
	void Camera();

	//固有攻撃
	virtual void SpecialAttack()=0;

	//アニメーション
	virtual void AnimationSelect() = 0;

	//プレイヤーにかかる磁力の影響
	void PlayerMagneticMove();

public:
	virtual ~Character_base();
	Quaternion rot;//キャラクターの回転
	Vector3 m_position = { 0.0f,0.0f,0.0f };//キャラクターの座標
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//キャラクターの移動速度
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//キャラクターの向き
	Vector3 m_toCameraDir = { 0.0f,0.0f,-1.0f };
	Vector3 Scale = { 0.3,0.3,0.3 };//キャラクターの拡大率
	Vector3 m_fallScale = { 0.66f,0.66f,0.66f };
	Vector3 front;//カメラの前方向
	Vector3 right;//カメラの右方向
	Vector3 cameraPos;//カメラのポジション
	Quaternion qRotY;
	float n;//内積
	float angle;//アークコサイン
	
	Vector3 m_to_enemy;
	float m_angle_with_enemy;
	Vector3 m_position_with_enemy;

	Vector3 m_repulsionSpeed;

	int m_magStatediff;

	prefab::CSkinModelRender* m_weaponModel = nullptr;//武器を持つ

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//キャラクターのモデル
	prefab::CPointLight* m_pointLight = nullptr;//チャージ確認用のポイントライト(TODO:後からエフェクトに差し替え予定)
	prefab::CFontRender* m_fontRender = nullptr;//体力、チャージ、磁力等確認用のフォント(TODO:後からUIスプライトに差し替え予定)
	prefab::CSpriteRender* m_spriteRender = nullptr;//勝利もしくは敗北時に表示するスプライト
	prefab::CSpriteRender* m_crosshairRender = nullptr;//照準のスプライト
	BackGround* m_stageModel;
	CharacterController m_charaCon;//プレイヤーのキャラコン
	//TriangleCollider m_collider;//単純な三角形の当たり判定(TODO:もっとしっかりした当たり判定を作りたい)
	MyCapsuleCollider m_collider;
	int m_playerNum = -1;//プレイヤーの番号 1P(0)、2P(1)
	int m_magPower;//磁力、なし(0)、引力状態(-1,-2)、斥力状態(1,2)
	int m_normalAttackCount = 0;//通常攻撃のタイマー
	int m_moveActionCount = 0;//移動アクションに使うカウント
	int m_hp = 1000;//体力
	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };//磁力が出ている原点
	float m_charge = 0;//チャージ
	int m_chargelevel = 1;//チャージレベル
	float m_deg = 0;//キャラの向きの角度
	bool m_isLock = false;//ロックオンしているか。
	int m_timer = 0;//磁力変化用のタイマー
	int m_loop = 0;//落下制御用のループカウント
	bool m_isMagPowerIncreasing = false;//磁力が増加しているか減少しているか
	bool m_isSceneStop = false;//動けるか動けないか
	float m_Speed = 6.0;
	//マシンガンを持ったとき
	bool m_MachinegunHave=false;
	bool m_MachinegunDelete = false;	
	int m_Machinegun_loopcount = 0;
	int m_Machinegun_deletetime = 500;
	int m_Machinegun_bulletNum = 100;	
	
	//現在の磁力の状態を取得
	int GetMagPower()const { return m_magPower; }

	//自分の体力にダメージを与える
	void Damage(int damage);

	//勝利した時
	void Win();

	//敗北した時
	void Lose();

	//斥力床が近いとき
	//void NearRepulsionFloor();
	Vector3 m_Yspeed = { 0.0f,0.0f,0.0f };
	//敵のインスタンス
	Character_base* m_enemy = nullptr;

	//サイコキネシスつかってるか(魔法使いだけ)
	bool m_Psycho_on = false;

	int m_anim_num = 0;

	int m_timerAccele = 1;
	int m_AcceleLoop = 0;

	enum {
		enAnimationClip_Attack,
		enAnimationClip_Run,
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Move,	
		enAnimationClip_Fall,
		enAnimationClip_Gun_Run,
		enAnimationClip_Gun_Idle,
		enAnimationClip_Gun_Walk,	
		enAnimationClip_num,  //列挙内で使う要素の数を表すダミー
	};
	enum EnStatus {
		enStatus_Attack,	//攻撃状態
		enStatus_Run,		//走り状態
		enStatus_Idle,		//待機状態
		enStatus_Walk,		//歩き状態
		enStatus_Move,		//移動アクション状態		
		enStatus_Fall,		//落下状態
		enStatus_Num,		//状態の数。
	};
	AnimationClip animationClips[enAnimationClip_num];
	EnStatus status = enStatus_Idle;	//現在の状態。

	int m_hitcount=0;
	int m_oldhitcount=0;
	int m_damage = 0;
	prefab::CFontRender* m_fontRenderDamage= nullptr;
	Vector3 damagePos;
	Vector2 screenDamagePos;
	std::wstring damageText;//ダメージ量表示
	//void DamageDisplay();
};

