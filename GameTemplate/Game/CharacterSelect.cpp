#include "stdafx.h"
#include "CharacterSelect.h"
#include "Character_base.h"
#include "BackGround.h"
#include "Mage.h"
#include "Knight.h"
CharacterSelect::~CharacterSelect() {
	//キャラ画像
	DeleteGO(m_mage_spriteRender);
	DeleteGO(m_knight_spriteRender);
	DeleteGO(m_tank_spriteRender);
	DeleteGO(m_ninja_spriteRender);
	//アイコン画像
	DeleteGO(m_1p_spriteRender);
	DeleteGO(m_2p_spriteRender);
}
bool CharacterSelect::Start() {
	//キャラの画像をそれぞれの位置に配置
	m_mage_spriteRender = NewGO<prefab::CSpriteRender>(0, "m_mage_spriteRender");
	m_mage_spriteRender->SetPosition({ m_top_left });
	m_mage_spriteRender->Init("Assets/Image/Mage.dds",256,256);
	m_knight_spriteRender = NewGO<prefab::CSpriteRender>(0, "m_knight_spriteRender");
	m_knight_spriteRender->SetPosition({ m_top_right });
	m_knight_spriteRender->Init("Assets/Image/Knight.dds", 256, 256);
	m_tank_spriteRender = NewGO<prefab::CSpriteRender>(0, "m_tank_spriteRender");
	m_tank_spriteRender->SetPosition({ m_bottom_left });
	m_tank_spriteRender->Init("Assets/Image/Tank.dds", 256, 256);
	m_ninja_spriteRender = NewGO<prefab::CSpriteRender>(0, "m_ninja_spriteRender");
	m_ninja_spriteRender->SetPosition({ m_bottom_right });
	m_ninja_spriteRender->Init("Assets/Image/Ninja.dds", 256, 256);
	//アイコンの画像を配置
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
	if (m_1p_decision == false) {//選択していないとき
		//左スティックでアイコンを動かす
		m_1p_moveSpeed.x = m_gain * g_pad[0]->GetLStickXF();
		m_1p_position.x += m_1p_moveSpeed.x;
		m_1p_moveSpeed.y = m_gain * g_pad[0]->GetLStickYF();
		m_1p_position.y += m_1p_moveSpeed.y;
		
		m_1p_spriteRender->SetPosition({ m_1p_position });
	}
	if ((m_1p_position.x >= 0 && m_1p_position.x < 256) && (m_1p_position.y > 0 && m_1p_position.y < 256-40)) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_1p_character_num = 0;//右上を選ぶと０を代入
			m_1p_decision = true;//決定フラグ
		}
	}
	if ((m_1p_position.x > -256 && m_1p_position.x < 0) && (m_1p_position.y > 0 && m_1p_position.y < 256 - 40)) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_1p_character_num = 1;//左上を選ぶと１を代入
			m_1p_decision = true;//決定フラグ
		}
	}
	if (g_pad[0]->IsTrigger(enButtonB)) {//Bを押すと選択解除
		m_1p_decision = false;
	}
	///////////////////////////////////////////////////////////////////////////////
	//ここから2P
	// 	   1Pと同じなので注釈を省略
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

	//アイコンが画面外に出ないようにする
	if (m_1p_position.x > screen_half_height - icon_half) { m_1p_position.x = screen_half_height - icon_half; }
	if (m_1p_position.x < -screen_half_height + icon_half) { m_1p_position.x = -screen_half_height + icon_half; }
	if (m_1p_position.y > screen_half_width - icon_half) { m_1p_position.y = screen_half_width - icon_half; }
	if (m_1p_position.y < -screen_half_width + icon_half) { m_1p_position.y = -screen_half_width+ icon_half; }
	
	if (m_1p_decision == true && m_2p_decision == true) {//両プレイヤーがキャラ選択しているとき
		if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {//どちらかがAを押すと
			GameObjectManager::GetInstance()->Set2ScreenMode(true);//２画面にする
			//ステージのライトを作成
			prefab::CDirectionLight* stageLight = NewGO<prefab::CDirectionLight>(0);
			stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
			stageLight->SetColor({ 0.5f,0.5f,0.5f });
			//プレイヤー1を作成
			Character_base* showm = nullptr;
			if (m_1p_character_num == 0) 
			{//キャラナンバーが０なら剣士を作成				
				showm = NewGO<Knight>(0, "Player");
			}
			else if(m_1p_character_num==1)//１なら魔法使いを作成		
			{				
				showm = NewGO<Mage>(0, "Player");
			}
			showm->m_position = { 0.0f,0.0f,-500.0f };
			showm->m_playerNum = 0;
			showm->m_magPower = 2;

			//プレイヤー2を作成
			Character_base* showm2 = nullptr;
			if (m_2p_character_num == 0) {
				showm2 = NewGO<Knight>(0, "Player");
			}
			else
				showm2 = NewGO<Mage>(0, "Player");
			showm2->m_position = { 0.0f,0.0f,500.0f };
			showm2->m_playerNum = 1;
			showm2->m_magPower = -2;
			showm2->m_toCamera = { 0.0f,100.0f,100.0f };

			//各プレイヤーに敵を渡す
			showm2->m_enemy = showm;
			showm->m_enemy = showm2;

			//ステージの表示
			NewGO<BackGround>(0, "background");	
			DeleteGO(this);
		}
	}
}