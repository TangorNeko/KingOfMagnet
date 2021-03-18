#pragma once
#include "TriangleCollider.h"

class ShowModel : public IGameObject
{
	~ShowModel();
	bool Start() override;
	void Update() override;

	//磁力の定期的な変化
	void ChangeMagnetPower();

	//敵に向く当たり判定を作る
	void Collision();

	//体力、チャージ、磁力の状態等の表示
	void DisplayStatus();

	//移動アクション
	void MoveAction();

	//通常攻撃
	void NormalAttack();

	//チャージ
	void Charge();

	//チャージ攻撃
	void SpecialAttack();

	//カメラの移動
	void Camera();
public:
	Quaternion rot;//キャラクターの回転
	Vector3 m_position = { 0.0f,0.0f,0.0f };//キャラクターの座標
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };//キャラクターの移動速度
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };//キャラクターの向き
	Vector3 m_toCamera = { 0.0f,100.0f,-100.0f };//キャラクターからカメラへのベクトル
	Vector3 Scale = { 0.6,0.6,0.6 };//キャラクターの拡大率

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//キャラクターのモデル
	prefab::CPointLight* m_pointLight = nullptr;//チャージ確認用のポイントライト(TODO:後からエフェクトに差し替え予定)
	prefab::CFontRender* m_fontRender = nullptr;//体力、チャージ、磁力等確認用のフォント(TODO:後からUIスプライトに差し替え予定)
	prefab::CSpriteRender* m_spriteRender = nullptr;//勝利もしくは敗北時に表示するスプライト
	CharacterController m_charaCon;//プレイヤーのキャラコン
	AnimationClip m_animClip[2];
	TriangleCollider m_collider;//単純な三角形の当たり判定(TODO:もっとしっかりした当たり判定を作りたい)
	int m_playerNum = -1;//プレイヤーの番号 1P(0)、2P(1)
	int m_magPower;//磁力、なし(0)、引力状態(-1,-2)、斥力状態(1,2)
	int m_normalAttackCount = 0;//通常攻撃のタイマー
	int m_moveActionCount = 0;//移動アクションに使うカウント
	int m_hp = 1000;//体力
	Vector3 m_magPosition = { 0.0f,0.0f,0.0f };//磁力が出ている原点
	float m_charge = 0;//チャージ
	float m_deg = 0;//キャラの向きの角度
	bool m_isLock = false;//ロックオンしているか。
	int m_timer = 0;//磁力変化用のタイマー
	bool m_isMagPowerIncreasing = false;//磁力が増加しているか減少しているか
	bool m_isSceneStop = false;//動けるか動けないか
public:
	//現在の磁力の状態を取得
	int GetMagPower()const { return m_magPower; }

	//自分の体力にダメージを与える
	void Damage(int damage);

	//勝利した時
	void Win();

	//敗北した時
	void Lose();

	//敵のインスタンス
	ShowModel* m_enemy = nullptr;
};
