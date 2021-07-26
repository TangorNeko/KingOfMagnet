#pragma once
class GameScene;

class Repulsion : public IGameObject
{
	~Repulsion() override;
	bool Start() override;
	void Update() override;

public:

	/**
	 * @brief 斥力床の座標を設定
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief 斥力床の回転を設定
	 * @param qRot 回転
	*/
	void SetRotation(const Quaternion& qRot)
	{
		m_qRot = qRot;
	}

private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;				//斥力床のモデル
	Vector3 m_position = Vector3::Zero;									//斥力床の座標
	Quaternion m_qRot = Quaternion::Identity;							//斥力床の回転
	prefab::CSoundSource* m_repulsionStaySE[2] = { nullptr,nullptr };	//斥力床に乗り続けている時の効果音、2プレイヤー分
	bool m_isPlayEnterSE[2] = { false,false };							//乗った瞬間のSEを鳴らしたかどうか(2プレイヤー分)
	bool m_isPlayStaySE[2] = { false,false };							//乗り続けている時のSEを鳴らしたかどうか(2プレイヤー分)
	float m_staySEVolume[2] = { 1.0f,1.0f };							//乗り続けている時のSEのボリューム
	GameScene* m_gameScene = nullptr;									//ゲームシーン
};

