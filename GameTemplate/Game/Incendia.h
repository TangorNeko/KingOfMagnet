#pragma once
class Incendia :public IGameObject
{
	~Incendia();
	bool Start();
	void Update();
public:

	Vector3 m_position;

	//ダメージを受けるまでのカウント(2キャラ分)
	int m_damageCountdown[2] = {0,0};

	//燃え続ける時間
	int m_deleteCountdown = 180;

	//エフェクト
	prefab::CEffect* m_effect = nullptr;
};
