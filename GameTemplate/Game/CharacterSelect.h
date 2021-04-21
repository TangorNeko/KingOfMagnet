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
	prefab::CSpriteRender* m_mage_spriteRender   = nullptr;//���@�g���̉摜
	prefab::CSpriteRender* m_knight_spriteRender = nullptr;//���m�̉摜
	prefab::CSpriteRender* m_tank_spriteRender   = nullptr;//�^���N�̉摜
	prefab::CSpriteRender* m_ninja_spriteRender  = nullptr;//�E�҂̉摜
	prefab::CSpriteRender* m_1p_spriteRender = nullptr;//1p�A�C�R��
	prefab::CSpriteRender* m_2p_spriteRender = nullptr;//2p�A�C�R��
	//�L�����摜��z�u������W
	Vector3 m_top_left = { -100,120,0 };//����
	Vector3 m_top_right = { 100,120,0 };//�E��
	Vector3 m_bottom_left = { -100,-130,0 };//����
	Vector3 m_bottom_right = { 100,-130,0 };//�E��
	Vector3 m_1p_position = { -50,60,0 };//�PP�A�C�R���̍��W
	Vector3 m_2p_position = { 50,60,0 };//�QP�A�C�R���̍��W
	Vector2 m_1p_moveSpeed;//�PP�A�C�R���̃X�s�[�h
	Vector2 m_2p_moveSpeed;//�QP�A�C�R���̃X�s�[�h
	float m_gain = 10.0f;//�X�e�B�b�N��|�����ʂɂ�����{��
	int m_1p_character_num;//0:���m1:���@�g��
	int m_2p_character_num;//0:���m1:���@�g��
	bool m_1p_decision = false;//�L���������肵�����ǂ���
	bool m_2p_decision = false;//����
	
};

