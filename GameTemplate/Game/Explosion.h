#pragma once
class Explosion :public IGameObject
{
	~Explosion() override;
	bool Start() override;
	void Update() override;

public:

	/**
	 * @brief 爆発の座標を設定する
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	prefab::CEffect* m_effect = nullptr;	//爆発のエフェクト
	Vector3 m_position = Vector3::Zero;		//爆発の座標
	bool m_deleteFlag = false;				//削除するか?(爆発は終わったか?)
};
