#pragma once

class Repulsion;
class BackGround : public IGameObject
{
	~BackGround() override;
	bool Start() override;
	void Update()override;
public:
	/**
	 * @brief 線分がステージのモデルにヒット(交差)しているか?
	 * @param start 線分の開始地点
	 * @param end 線分の終了地点
	 * @param crossPoint ヒットした地点(参照受け取り)
	 * @return ヒットした?
	*/
	bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint) const;

	/**
	 * @brief 受け取った地点(敵の位置)から最も遠いリスポーン地点を返す。
	 * @param enemyPos 敵の座標
	 * @return リスポーン地点の中で受け取った敵の座標から一番遠いものの座標
	*/
	Vector3 CalcRespawnPoint(const Vector3& enemyPos) const;
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//ステージのモデル
	Vector3 m_position = { 0.0f,0.0f,0.0f };				//ステージのモデルの座標
	prefab::CLevel m_level;									//リスポーン地点を読み込む用のモデル。
	prefab::CSpotLight* m_southSpotLight = nullptr;			//ステージの穴を照らすライト
	prefab::CSpotLight* m_northSpotLight = nullptr;			//ステージの穴を照らすライト
	PhysicsStaticObject m_physicsStaticObject;				//CharaConとの当たり判定に使用する静的物理オブジェクト
	std::vector<Vector3> m_respawnPoints;					//リスポーン地点の候補を格納するコンテナ
};

