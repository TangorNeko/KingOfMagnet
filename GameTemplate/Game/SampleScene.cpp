#include "stdafx.h"
#include "SampleScene.h"

#include "Player.h"
#include "BackGround.h"
#include "SkyBoard.h"
#include "Debris.h"
#include "DebrisBlock.h"
class GameTime;

SampleScene::~SampleScene()
{
	DeleteGO(m_stageLight);
	DeleteGO(m_backGround);
	DeleteGO(m_sky);
	DeleteGO(m_player1);
	DeleteGO(m_player2);
}

bool SampleScene::Start()
{
	//ステージのライトを作成
	m_stageLight = NewGO<prefab::CDirectionLight>(0);
	m_stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
	m_stageLight->SetColor({ 0.5f,0.5f,0.5f });

	m_player1 = NewGO<Player>(0, "Player");
	m_player1->m_position = { 860.0f,0.0f,550.0f };
	m_player1->m_playerNum = 0;
	m_player1->m_magPower = 1;
	m_player1->m_toCameraDir = { 1.0f,0.0f,0.0f };

	m_player2 = NewGO<Player>(0, "Player");
	m_player2->m_position = { -860.0f,0.0f,-550.0f };
	m_player2->m_playerNum = 1;
	m_player2->m_magPower = -1;
	m_player2->m_toCameraDir = { -1.0f,0.0f,0.0f };

	//各プレイヤーに敵を渡す
	m_player2->m_enemy = m_player1;
	m_player1->m_enemy = m_player2;

	//ステージの表示
	m_backGround = NewGO<BackGround>(0, "background");

	//ガレキブロック。
	DebrisBlock* debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -700.0f,0.0f,600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -750.0f,0.0f,600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -700.0f,0.0f,550.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -750.0f,0.0f,550.0f };


	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 700.0f,0.0f,-600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 750.0f,0.0f,-600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 700.0f,0.0f,-550.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 750.0f,0.0f,-550.0f };

	//ガレキ。
	Debris* debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_position = { 0.0f,0.0f,0.0f };

	debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_debrisShape = Debris::enSword;
	debris->m_position = { 200.0f,0.0f,0.0f };

	debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_position = { 300.0f,0.0f,0.0f };

	debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_position = { 300.0f,0.0f,100.0f };

	//空を作成。キューブマップじゃなくてただの板ポリ。
	m_sky = NewGO<SkyBoard>(0);

	//すべて作成し終わった所で2画面にする。
	GameObjectManager::GetInstance()->Set2ScreenMode(true);

	//タイムリミット表示
	m_timeFontRender = NewGO<prefab::CFontRender>(0);
	m_timeFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)0);
	m_timeFontRender->SetPosition({ 0.0f, 0.0f });

	return true;
}

void SampleScene::Update()
{
	//制限時間の表示
	if (m_timeLimit > 0) {
		m_timeLimit -= GameTime::GetInstance().GetFrameDeltaTime();
		m_timeFontRender->SetText(std::to_wstring((int)m_timeLimit));
	}
}