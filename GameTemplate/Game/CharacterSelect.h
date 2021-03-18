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
	prefab::CSpriteRender* m_mage_spriteRender   = nullptr;
	prefab::CSpriteRender* m_knight_spriteRender = nullptr;
	prefab::CSpriteRender* m_tank_spriteRender   = nullptr;
	prefab::CSpriteRender* m_ninja_spriteRender  = nullptr;
	prefab::CSpriteRender* m_1p_spriteRender = nullptr;
	prefab::CSpriteRender* m_2p_spriteRender = nullptr;	
	Vector3 m_top_left = { -100,120,0 };
	Vector3 m_top_right = { 100,120,0 };
	Vector3 m_bottom_left = { -100,-130,0 };
	Vector3 m_bottom_right = { 100,-130,0 };
	Vector3 m_1p_position;
	Vector3 m_2p_position;
	Vector2 m_1p_moveSpeed;
	Vector2 m_2p_moveSpeed;	
	float m_gain = 10.0f;
	int m_1p_character_num;//0:Œ•Žm1:–‚–@Žg‚¢
	int m_2p_character_num;//0:Œ•Žm1:–‚–@Žg‚¢
	bool m_1p_decision = false;
	bool m_2p_decision = false;
	
};

