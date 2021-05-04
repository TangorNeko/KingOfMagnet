#include "stdafx.h"
#include "SampleScene.h"

#include "Player.h"
#include "BackGround.h"
#include "SkyBoard.h"
#include "Debris.h"
#include "DebrisBlock.h"
#include "Repulsion.h"
#include "TitleScene.h"

SampleScene::~SampleScene()
{
	DeleteGO(m_timeFontRender);
	DeleteGO(m_stageLight);
	DeleteGO(m_backGround);
	DeleteGO(m_sky);
	DeleteGO(m_player1);
	DeleteGO(m_player2);

	QueryGOs<DebrisBlock>("debrisblock",[](DebrisBlock* debrisblock)->bool
		{
			DeleteGO(debrisblock);
			return true;
		});

	QueryGOs<Debris>("debris", [](Debris* debris)->bool
		{
			DeleteGO(debris);
			return true;
		});

	QueryGOs<Repulsion>("repulsion", [](Repulsion* repulsion)->bool
		{
			DeleteGO(repulsion);
			return true;
		});
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


	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 0.0f,160.0f,0.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 50.0f,160.0f,50.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 50.0f,160.0f,-50.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -50.0f,160.0f,50.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -50.0f,160.0f,-50.0f };

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

	//仮
	debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_debrisShape = Debris::enSpecialCharger;
	debris->m_position = { 100.0f,500.0f,100.0f };


	Repulsion* rep = NewGO<Repulsion>(0, "repulsion");
	rep->m_position = { 310.0f,1.0f,0.0f };

	rep = NewGO<Repulsion>(0, "repulsion");
	rep->m_position = { -310.0f,1.0f,0.0f };

	//空を作成。キューブマップじゃなくてただの板ポリ。
	m_sky = NewGO<SkyBoard>(0);

	//すべて作成し終わった所で2画面にする。
	GameObjectManager::GetInstance()->Set2ScreenMode(true);

	//タイムリミット表示
	m_timeFontRender = NewGO<prefab::CFontRender>(4);
	m_timeFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_timeFontRender->SetPosition({ -60.0f, 380.0f });
	m_timeFontRender->SetScale({ 2.0f, 2.0f });
	m_delimitLineSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_delimitLineSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_delimitLineSpriteRender->SetPosition({ 0.0f,0.0f,0.0f });
	m_delimitLineSpriteRender->Init("Assets/Image/Waku.dds", 40, 720);

	m_HPCoverSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_HPCoverSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_HPCoverSpriteRender->SetPosition({ 0.0f,304.0f,0.0f });
	m_HPCoverSpriteRender->Init("Assets/Image/HP_Cover.dds", 1280, 112);

	m_TimerBaseSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_TimerBaseSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_TimerBaseSpriteRender->SetPosition({ 0.0f,300.0f,0.0f });
	m_TimerBaseSpriteRender->Init("Assets/Image/Timer_Base.dds", 272, 120);

	return true;
}

void SampleScene::Update()
{
	//制限時間の表示
	if (m_timeLimit > 0) {
		m_timeLimit -= GameTime::GetInstance().GetFrameDeltaTime();
		m_timeFontRender->SetText(std::to_wstring((int)m_timeLimit));
	}
	else if(m_gameEndFlag == false)
	{
		WinnerJudge();
		m_gameEndFlag = true;
	}

	if (m_gameEndFlag == true)
	{
		m_gameEndCount++;
	}

	if (m_gameEndCount == 300)
	{
		NewGO<TitleScene>(0, "titlescene");
		DeleteGO(this);
	}
}

void SampleScene::WinnerJudge()
{
	if (m_player1->m_hp > m_player2->m_hp)
	{
		//1Pの勝ち
		m_player1->Win();
		m_player2->Lose();

	}
	else if(m_player1->m_hp < m_player2->m_hp)
	{
		//2Pの勝ち
		m_player1->Lose();
		m_player2->Win();
	}
	else
	{
		//引き分け
	}
}