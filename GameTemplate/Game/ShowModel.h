#pragma once
#include "TriangleCollider.h"

class ShowModel : public IGameObject
{
	~ShowModel();
	bool Start() override;
	void Update()override;


	void ChangeMagnetPower();
	void Collision();
	void DisplayStatus();
	void MoveAction();
	void NormalAttack();
	void Charge();
	void SpecialAttack();
	void Camera();
	void Floor();

public:
	Quaternion rot;
	Vector3 m_position = { 0.0f,0.0f,0.0f };
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };
	Vector3 m_characterDirection = { 0.0f,0.0f,1.0f };
	Vector3 m_toCamera = { 0.0f,100.0f,-100.0f };
	Vector3 Scale = { 0.6,0.6,0.6 };	

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	prefab::CSkinModelRender* floor_skinModelRender = nullptr;
	prefab::CPointLight* m_pointLight = nullptr;
	prefab::CFontRender* m_fontRender = nullptr;
	prefab::CSpriteRender* m_spriteRender = nullptr;
	CharacterController m_charaCon;
	TriangleCollider m_collider;
	int m_playerNum = -1;//プレイヤーの番号 1P(0)、2P(1)
	int m_magPower;//磁力、なし(0)、引力状態(-1,-2)、斥力状態(1,2)
	int m_normalAttackCount = 0;
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
	int GetMagPower() { return m_magPower; }
	void Damage(int damage);
	void Win();
	void Lose();
	ShowModel* m_enemy = nullptr;
};
