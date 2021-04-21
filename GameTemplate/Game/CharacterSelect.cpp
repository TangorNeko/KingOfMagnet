#include "stdafx.h"
#include "CharacterSelect.h"
#include "Character_base.h"
#include "Mage.h"
#include "Knight.h"

#include "GameScene.h"


CharacterSelect::~CharacterSelect() {
	//�L�����摜
	DeleteGO(m_mage_spriteRender);
	DeleteGO(m_knight_spriteRender);
	DeleteGO(m_tank_spriteRender);
	DeleteGO(m_ninja_spriteRender);
	//�A�C�R���摜
	DeleteGO(m_1p_spriteRender);
	DeleteGO(m_2p_spriteRender);
}
bool CharacterSelect::Start() {
	//�L�����̉摜�����ꂼ��̈ʒu�ɔz�u
	m_mage_spriteRender = NewGO<prefab::CSpriteRender>(0, "m_mage_spriteRender");
	m_mage_spriteRender->SetPosition({ m_top_left });
	m_mage_spriteRender->Init("Assets/Image/Mage.dds",256,256);
	m_knight_spriteRender = NewGO<prefab::CSpriteRender>(0, "m_knight_spriteRender");
	m_knight_spriteRender->SetPosition({ m_top_right });
	m_knight_spriteRender->Init("Assets/Image/Knight.dds", 256, 256);
	m_tank_spriteRender = NewGO<prefab::CSpriteRender>(0, "m_tank_spriteRender");
	m_tank_spriteRender->SetPosition({ m_bottom_left });
	m_tank_spriteRender->Init("Assets/Image/TankX.dds", 256, 256);
	m_ninja_spriteRender = NewGO<prefab::CSpriteRender>(0, "m_ninja_spriteRender");
	m_ninja_spriteRender->SetPosition({ m_bottom_right });
	m_ninja_spriteRender->Init("Assets/Image/NinjaX.dds", 256, 256);
	//�A�C�R���̉摜��z�u
	m_1p_spriteRender = NewGO<prefab::CSpriteRender>(1, "m_1p_spriteRender");
	m_1p_spriteRender->SetPosition({ m_top_right });
	m_1p_spriteRender->Init("Assets/Image/1p.dds", 64, 64);
	m_2p_spriteRender = NewGO<prefab::CSpriteRender>(1, "m_2p_spriteRender");
	m_2p_spriteRender->SetPosition({ m_top_left });
	m_2p_spriteRender->Init("Assets/Image/2p.dds", 64, 64);

	return true;
}
void CharacterSelect::Update() {
	///////////////////////////////////////////////////////////////////////
	//1P
	if (m_1p_decision == false) {//�I�����Ă��Ȃ��Ƃ�
		//���X�e�B�b�N�ŃA�C�R���𓮂���
		m_1p_moveSpeed.x = m_gain * g_pad[0]->GetLStickXF();
		m_1p_position.x += m_1p_moveSpeed.x;
		m_1p_moveSpeed.y = m_gain * g_pad[0]->GetLStickYF();
		m_1p_position.y += m_1p_moveSpeed.y;
		
		m_1p_spriteRender->SetPosition({ m_1p_position });
	}
	if ((m_1p_position.x >= 0 && m_1p_position.x < 256) && (m_1p_position.y > 0 && m_1p_position.y < 256-40)) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_1p_character_num = 0;//�E���I�ԂƂO����
			m_1p_decision = true;//����t���O
		}
	}
	if ((m_1p_position.x > -256 && m_1p_position.x < 0) && (m_1p_position.y > 0 && m_1p_position.y < 256 - 40)) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_1p_character_num = 1;//�����I�ԂƂP����
			m_1p_decision = true;//����t���O
		}
	}
	if (g_pad[0]->IsTrigger(enButtonB)) {//B�������ƑI������
		m_1p_decision = false;
	}
	///////////////////////////////////////////////////////////////////////////////
	//��������2P
	// 	   1P�Ɠ����Ȃ̂Œ��߂��ȗ�
	///////////////////////////////////////////////////////////////////////////////
	if (m_2p_decision == false) {
		m_2p_moveSpeed.x = m_gain * g_pad[1]->GetLStickXF();
		m_2p_moveSpeed.y = m_gain * g_pad[1]->GetLStickYF();
		m_2p_position.x += m_2p_moveSpeed.x;
		m_2p_position.y += m_2p_moveSpeed.y;
		m_2p_spriteRender->SetPosition({ m_2p_position });
	}
	if ((m_2p_position.x >= 0 && m_2p_position.x < 256) && (m_2p_position.y > 0 && m_2p_position.y < 256 - 40)) {
		m_2p_character_num = 0;
		if (g_pad[1]->IsTrigger(enButtonA)) {
			m_2p_decision = true;
		}
	}
	if ((m_2p_position.x > -256 && m_2p_position.x < 0) && (m_2p_position.y > 0 && m_2p_position.y < 256 - 40)) {
		m_2p_character_num = 1;
		if (g_pad[1]->IsTrigger(enButtonA)) {
			m_2p_decision = true;
		}
	}
	if (g_pad[1]->IsTrigger(enButtonB)) {
		m_2p_decision = false;
	}
	///////////////////////////////////////////////////////////////////////////////////

	//�A�C�R������ʊO�ɏo�Ȃ��悤�ɂ���
	if (m_1p_position.x > screen_half_height - icon_half) { m_1p_position.x = screen_half_height - icon_half; }
	if (m_1p_position.x < -screen_half_height + icon_half) { m_1p_position.x = -screen_half_height + icon_half; }
	if (m_1p_position.y > screen_half_width - icon_half) { m_1p_position.y = screen_half_width - icon_half; }
	if (m_1p_position.y < -screen_half_width + icon_half) { m_1p_position.y = -screen_half_width+ icon_half; }

	if (m_2p_position.x > screen_half_height - icon_half) { m_2p_position.x = screen_half_height - icon_half; }
	if (m_2p_position.x < -screen_half_height + icon_half) { m_2p_position.x = -screen_half_height + icon_half; }
	if (m_2p_position.y > screen_half_width - icon_half) { m_2p_position.y = screen_half_width - icon_half; }
	if (m_2p_position.y < -screen_half_width + icon_half) { m_2p_position.y = -screen_half_width + icon_half; }
	
	if (m_1p_decision == true && m_2p_decision == true) {//���v���C���[���L�����I�����Ă���Ƃ�
		if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {//�ǂ��炩��A��������
			
			//�L�����I�����m�肵���̂ŃQ�[���V�[�����쐬�B
			GameScene* gamescene = NewGO<GameScene>(0, "gamescene");

			//�v���C���[1���쐬
			Character_base* player1 = nullptr;
			if (m_1p_character_num == 0) 
			{//�L�����i���o�[���O�Ȃ猕�m���쐬				
				player1 = NewGO<Knight>(0, "Player");
			}
			else if(m_1p_character_num==1)//�P�Ȃ疂�@�g�����쐬		
			{				
				player1 = NewGO<Mage>(0, "Player");
			}
			player1->m_position = { 0.0f,0.0f,-500.0f };
			player1->m_playerNum = 0;
			player1->m_magPower = 2;

			//�v���C���[2���쐬
			Character_base* player2 = nullptr;
			if (m_2p_character_num == 0) {
				player2 = NewGO<Knight>(0, "Player");
			}
			else
				player2 = NewGO<Mage>(0, "Player");
			player2->m_position = { 0.0f,0.0f,500.0f };
			player2->m_playerNum = 1;
			player2->m_magPower = -2;

			//�Q�[���V�[���Ƀv���C���[��o�^�B
			gamescene->RegisterPlayer(player1, player2);
			
			DeleteGO(this);
		}
	}
}