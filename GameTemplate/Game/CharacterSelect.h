#pragma once
class CharacterSelect:public IGameObject
{
	const int screen_half_width = 360;
	const int screen_half_height = 640;
	const int icon_half = 32;
	
	bool Start() override;
	void Update() override;
public:
	~CharacterSelect();
	prefab::CSpriteRender* m_mage_spriteRender   = nullptr;//魔法使いの画像
	prefab::CSpriteRender* m_knight_spriteRender = nullptr;//剣士の画像
	prefab::CSpriteRender* m_tank_spriteRender   = nullptr;//タンクの画像
	prefab::CSpriteRender* m_ninja_spriteRender  = nullptr;//忍者の画像
	prefab::CSpriteRender* m_1p_spriteRender = nullptr;//1pアイコン
	prefab::CSpriteRender* m_2p_spriteRender = nullptr;//2pアイコン
	//キャラ画像を配置する座標
	Vector3 m_top_left = { -100,120,0 };//左上
	Vector3 m_top_right = { 100,120,0 };//右上
	Vector3 m_bottom_left = { -100,-130,0 };//左下
	Vector3 m_bottom_right = { 100,-130,0 };//右下
	Vector3 m_1p_position = { -50,60,0 };//１Pアイコンの座標
	Vector3 m_2p_position = { 50,60,0 };//２Pアイコンの座標
	Vector2 m_1p_moveSpeed;//１Pアイコンのスピード
	Vector2 m_2p_moveSpeed;//２Pアイコンのスピード
	float m_gain = 10.0f;//スティックを倒した量にかかる倍率
	int m_1p_character_num;//0:剣士1:魔法使い
	int m_2p_character_num;//0:剣士1:魔法使い
	bool m_1p_decision = false;//キャラを決定したかどうか
	bool m_2p_decision = false;//同上
	
};

