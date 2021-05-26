#include "stdafx.h"
#include "SampleScene.h"
#include "Player.h"
#include "BackGround.h"
#include "SkyBoard.h"
#include "Debris.h"
#include "DebrisBlock.h"
#include "Bomb.h"
#include "Repulsion.h"
#include "TitleScene.h"
#include "ResultScene.h"

SampleScene::~SampleScene()
{	
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

	QueryGOs<Bomb>("bomb", [](Bomb* bomb)->bool
		{
			DeleteGO(bomb);
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

	DeleteGO(ssBGM);
}

bool SampleScene::Start()
{
	//ステージのライトを作成
	m_stageLight = NewGO<prefab::CDirectionLight>(0);
	m_stageLight->SetDirection({ -1.0f,-1.0f,1.0f });
	m_stageLight->SetColor({ 0.8f,0.8f,0.8f });

	m_player1 = NewGO<Player>(0, "Player");
	m_player1->m_position = { 760.0f,0.0f,400.0f };
	//m_player1->m_position = { 250.0f,0.0f,250.0f };//ミニステージ
	m_player1->m_playerNum = 0;
	m_player1->m_magPower = 1;
	m_player1->m_toCameraDir = { 1.0f,0.0f,0.0f };
	m_player1->m_characterDirection = { -1.0f,0.0f,0.0f };

	m_player2 = NewGO<Player>(0, "Player");
	m_player2->m_position = { -760.0f,0.0f,-400.0f };
	//m_player2->m_position = { -250.0f,0.0f,-250.0f };//ミニステージ
	m_player2->m_playerNum = 1;
	m_player2->m_magPower = -1;
	m_player2->m_toCameraDir = { -1.0f,0.0f,0.0f };
	m_player2->m_characterDirection = { 1.0f,0.0f,0.0f };

	//各プレイヤーに敵を渡す
	m_player2->m_enemy = m_player1;
	m_player1->m_enemy = m_player2;
	
	//ステージの表示
	m_backGround = NewGO<BackGround>(0, "background");

	//ガレキブロック。
	DebrisBlock* debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -700.0f,0.0f,600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 700.0f,0.0f,-600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 0.0f,160.0f,0.0f };

	Repulsion* rep = NewGO<Repulsion>(0, "repulsion");
	rep->m_position = { 310.0f,1.0f,0.0f };
	rep->m_rot.SetRotationDeg(Vector3::AxisY,-90.0f);
	rep = NewGO<Repulsion>(0, "repulsion");
	rep->m_position = { -310.0f,1.0f,0.0f };
	rep->m_rot.SetRotationDeg(Vector3::AxisY,90.0f);
	//空を作成。キューブマップじゃなくてただの板ポリ。
	m_sky = NewGO<SkyBoard>(0);

	//すべて作成し終わった所で2画面にする。
	GameObjectManager::GetInstance()->Set2ScreenMode(true);
	
	//画面を区切る線
	m_delimitLineSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_delimitLineSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_delimitLineSpriteRender->SetPosition({ 0.0f,0.0f,0.0f });
	m_delimitLineSpriteRender->Init("Assets/Image/Waku.dds", 40, 720);

	//HPのカバー
	m_HPCoverSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_HPCoverSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_HPCoverSpriteRender->SetPosition({ 0.0f,304.0f,0.0f });//y304
	m_HPCoverSpriteRender->Init("Assets/Image/HP_Cover.dds", 1280, 112);

	//タイマーの枠
	m_TimerBaseSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_TimerBaseSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_TimerBaseSpriteRender->SetPosition({ 0.0f,300.0f,0.0f });
	m_TimerBaseSpriteRender->Init("Assets/Image/Timer_Base.dds", 272, 120);

	//ゲームスタートカウント
	//m_startCountFontRender = NewGO<prefab::CFontRender>(4);
	//m_startCountFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	//m_startCountFontRender->SetPosition({ -185.0f, 130.0f });
	//m_startCountFontRender->SetScale({ 3.0f, 3.0f });

	//カウント3
	m_CountDown_3_1 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_3_1->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_3_1->SetPosition({ -900.0f,100.0f,0.0f });
	m_CountDown_3_1->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_CountDown_3_2 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_3_2->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_3_2->SetPosition({ 900.0f,0.0f,0.0f });
	m_CountDown_3_2->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_CountDown_3_3 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_3_3->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_3_3->SetPosition({ -900.0f,-100.0f,0.0f });
	m_CountDown_3_3->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_CountDown_3_4 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_3_4->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_3_4->SetPosition({ 50.0f,-600.0f,0.0f });
	m_CountDown_3_4->Init("Assets/Image/Count/CountLine.dds", 24, 200);
	//カウント2
	m_CountDown_2_1 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_2_1->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_2_1->SetPosition({ 900.0f,100.0f,0.0f });
	m_CountDown_2_1->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_CountDown_2_2 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_2_2->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_2_2->SetPosition({ -900.0f,0.0f,0.0f });
	m_CountDown_2_2->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_CountDown_2_3 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_2_3->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_2_3->SetPosition({ 900.0f,-100.0f,0.0f });
	m_CountDown_2_3->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_CountDown_2_4 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_2_4->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_2_4->SetPosition({ 50.0f,-550.0f,0.0f });
	m_CountDown_2_4->Init("Assets/Image/Count/CountLine.dds", 24, 100);
	//
	m_CountDown_2_5 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_2_5->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_2_5->SetPosition({ -50.0f,550.0f,0.0f });
	m_CountDown_2_5->Init("Assets/Image/Count/CountLine.dds", 24, 100);
	//カウント1
	m_CountDown_1_1 = NewGO<prefab::CSpriteRender>(3);
	m_CountDown_1_1->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_CountDown_1_1->SetPosition({ 0.0f,600.0f,0.0f });
	m_CountDown_1_1->Init("Assets/Image/Count/CountLine.dds", 36, 200);

	//
	m_onesPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_onesPlaceSpriteRender->SetPosition({ 27.0f, 315.0f, 0.0f });
	m_onesPlaceSpriteRender->SetScale({0.25f,0.25f,1.0f});
	m_onesPlaceSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_onesPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);

	m_tensPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_tensPlaceSpriteRender->SetPosition({ -27.0f, 315.0f, 0.0f });
	m_tensPlaceSpriteRender->SetScale({ 0.25f,0.25f,1.0f });
	m_tensPlaceSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_tensPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);

	//音を再生
	ssBGM = NewGO<prefab::CSoundSource>(0);
	ssBGM->Init(L"Assets/sound/サイバー風BGM.wav");
	ssBGM->SetVolume(0.3f);
	//ssBGM->Play(true);
	return true;
}

void SampleScene::Update()
{

	//デバッグ用。ゲームシーンに存在する弾+グレネードの数を出力
	char buff[256];
	sprintf_s(buff, "現在のゲームシーンに存在する弾数 = %d\n", m_bulletNum);
	OutputDebugStringA(buff);

	//スタートダウンカウントダウン
	if (m_gameState == enStartCountDown) {
		StartCountDown();
		return;
	}
	TimeLimitCount();

	//時間切れでも勝敗がついていない場合
	if(m_timeLimit <= 0 && m_gameState != enResult && m_gameState != enDraw)
	{
		//勝者判定
		WinnerJudge();
	}

	//ポーズ関連。
	//ポーズ。
	if (m_gameState == enPlaying && (g_pad[0]->IsTrigger(enButtonStart) || g_pad[1]->IsTrigger(enButtonStart)))
	{
		m_gameState = enPause;

		return;
	}

	//ポーズ復帰。
	if (m_gameState == enPause && (g_pad[0]->IsTrigger(enButtonStart) || g_pad[1]->IsTrigger(enButtonStart)))
	{
		m_gameState = enPlaying;

		return;
	}

	if (m_gameState == enResult)
	{	
		if (m_GEfirstLoop == true)
		{
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_onesPlaceSpriteRender);
			if (int(m_timeLimit) >= 10)
				DeleteGO(m_tensPlaceSpriteRender);
			//m_player1->m_displayOff = true;
			//m_player2->m_displayOff = true;
			m_GEfirstLoop = false;
			if (m_player1->m_Lose == true)
			{
				m_player1->m_loserNum = 0;
				m_player2->m_loserNum = 0;
			}
			else if (m_player2->m_Lose == true)
			{
				m_player1->m_loserNum = 1;
				m_player2->m_loserNum = 1;
			}
		}
		if (g_pad[0]->IsTrigger(enButtonA)|| g_pad[1]->IsTrigger(enButtonA))
		{
			m_gameEndCount++;
		}
		m_gameEndLoop++;
		if (m_gameEndCount == 2||m_gameEndLoop>1500)
		{
			NewGO<ResultScene>(0, "resultscene");
			ResultScene* resultscene = FindGO<ResultScene>("resultscene");
			resultscene->m_loserNum = m_player1->m_loserNum;
			DeleteGO(this);
		}
	}

	//引き分け時
	if (m_gameState == enDraw)
	{
		m_drawFontCount--;

		if (m_drawFontCount == 0)
		{
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_onesPlaceSpriteRender);
			if (int(m_timeLimit) >= 10)
				DeleteGO(m_tensPlaceSpriteRender);
			DeleteGO(m_drawFontRender);
			NewGO<SampleScene>(0, "gamescene");
			DeleteGO(this);
		}
	}

	//エフェクト試す用
	//if (g_pad[0]->IsTrigger(enButtonB)) {
	//	Player* pl = FindGO<Player>("Player");

	//	prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
	//	effect->Init(u"Assets/effect/Blackhole3.efk");
	//	effect->SetPosition(pl->m_position);
	//	effect->SetScale({ 30.0f, 30.0f, 30.0f });
	//	effect->Play();
	//	//必殺技試す用
	//	pl->ChargeSpecialAttackGauge(10);

	//}
}

void SampleScene::WinnerJudge()
{
	if (m_player1->m_hp > m_player2->m_hp)
	{
		//1Pの勝ち
		m_player1->Win();
		m_player2->Lose();

		//リザルトシーンに移行
		m_gameState = enResult;

	}
	else if(m_player1->m_hp < m_player2->m_hp)
	{
		//2Pの勝ち
		m_player1->Lose();
		m_player2->Win();

		//リザルトシーンに移行
		m_gameState = enResult;
	}
	else
	{
		//引き分け
		m_gameState = enDraw;

		m_drawFontRender = NewGO<prefab::CFontRender>(4);
		m_drawFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
		m_drawFontRender->SetPosition({ -185.0f, 130.0f });
		m_drawFontRender->SetScale({ 3.0f, 3.0f });
		m_drawFontRender->SetText(L"Draw!");
	}
}

void SampleScene::StartCountDown() {

	m_measureSecond += GameTime::GetInstance().GetFrameDeltaTime();		//一秒を測る。
	if (m_measureSecond >= 1.0f) {
		m_startCount -= 1;
		m_measureSecond = 0.0f;
		m_playCountSEFlag = true;
	}
	if (m_startCount >= 1 && m_startCount < 4) {	
		//3・2・1のカウント表示
		//m_startCountFontRender->SetText(L"  " + std::to_wstring(m_startCount));
	}

	//カウントダウン
	if (m_startCount == 3 && m_count3_Flag == false) {
		m_CountDown_3_1->m_spriteSupporter.SpriteMove({ 890.0f,0.0f }, 12, 0, true);
		m_CountDown_3_1->m_spriteSupporter.SpriteMove({ 10.0f,0.0f }, 48, 12, true);
		m_CountDown_3_1->m_spriteSupporter.SpriteMove({ 890.0f,0.0f }, 12, 60, true);

		m_CountDown_3_2->m_spriteSupporter.SpriteMove({ -890.0f,0.0f }, 12, 0, true);
		m_CountDown_3_2->m_spriteSupporter.SpriteMove({ -10.0f,0.0f }, 48, 12, true);
		m_CountDown_3_2->m_spriteSupporter.SpriteMove({ -890.0f,0.0f }, 12, 60, true);

		m_CountDown_3_3->m_spriteSupporter.SpriteMove({ 890.0f,0.0f }, 12, 0, true);
		m_CountDown_3_3->m_spriteSupporter.SpriteMove({ 10.0f,0.0f }, 48, 12, true);
		m_CountDown_3_3->m_spriteSupporter.SpriteMove({ 890.0f,0.0f }, 12, 60, true);

		m_CountDown_3_4->m_spriteSupporter.SpriteMove({ 0.0f,590.0f }, 12, 0, true);
		m_CountDown_3_4->m_spriteSupporter.SpriteMove({ 0.0f,10.0f }, 48, 12, true);
		m_CountDown_3_4->m_spriteSupporter.SpriteMove({ 0.0f,590.0f }, 12, 60, true);

		m_count3_Flag = true;
	}
	if (m_startCount == 2 && m_count2_Flag == false) {

		m_CountDown_2_1->m_spriteSupporter.SpriteMove({ -890.0f,0.0f }, 12, 0, true);
		m_CountDown_2_1->m_spriteSupporter.SpriteMove({ -10.0f,0.0f }, 48, 12, true);
		m_CountDown_2_1->m_spriteSupporter.SpriteMove({ -890.0f,0.0f }, 12, 60, true);

		m_CountDown_2_2->m_spriteSupporter.SpriteMove({ 890.0f,0.0f }, 12, 0, true);
		m_CountDown_2_2->m_spriteSupporter.SpriteMove({ 10.0f,0.0f }, 48, 12, true);
		m_CountDown_2_2->m_spriteSupporter.SpriteMove({ 890.0f,0.0f }, 12, 60, true);

		m_CountDown_2_3->m_spriteSupporter.SpriteMove({ -890.0f,0.0f }, 12, 0, true);
		m_CountDown_2_3->m_spriteSupporter.SpriteMove({ -10.0f,0.0f }, 48, 12, true);
		m_CountDown_2_3->m_spriteSupporter.SpriteMove({ -890.0f,0.0f }, 12, 60, true);

		m_CountDown_2_4->m_spriteSupporter.SpriteMove({ 0.0f,590.0f }, 12, 0, true);
		m_CountDown_2_4->m_spriteSupporter.SpriteMove({ 0.0f,10.0f }, 48, 12, true);
		m_CountDown_2_4->m_spriteSupporter.SpriteMove({ 0.0f,590.0f }, 12, 60, true);

		m_CountDown_2_5->m_spriteSupporter.SpriteMove({ 0.0f,-590.0f }, 12, 0, true);
		m_CountDown_2_5->m_spriteSupporter.SpriteMove({ 0.0f,-10.0f }, 48, 12, true);
		m_CountDown_2_5->m_spriteSupporter.SpriteMove({ 0.0f,-590.0f }, 12, 60, true);

		m_count2_Flag = true;
	}
	if (m_startCount == 1 && m_count1_Flag == false) {

		m_CountDown_1_1->m_spriteSupporter.SpriteMove({ 0.0f,-590.0f }, 12, 0, true);
		m_CountDown_1_1->m_spriteSupporter.SpriteMove({ 0.0f,-10.0f }, 48, 12, true);
		m_CountDown_1_1->m_spriteSupporter.SpriteMove({ 0.0f,-590.0f }, 12, 60, true);

		m_count1_Flag = true;
	}
	else if ( m_startCount == 0) {
		//m_startCountFontRender->SetText(L"Start!");
	}

	else if (m_startCount < 0) {
		DeleteGO(m_CountDown_3_1);
		DeleteGO(m_CountDown_3_2);
		DeleteGO(m_CountDown_3_3);
		DeleteGO(m_CountDown_3_4);
		DeleteGO(m_CountDown_2_1);
		DeleteGO(m_CountDown_2_2);
		DeleteGO(m_CountDown_2_3);
		DeleteGO(m_CountDown_2_4);
		DeleteGO(m_CountDown_2_5);
		DeleteGO(m_CountDown_1_1);
		m_gameState = enPlaying;
	}

	if (m_playCountSEFlag == true) {
		prefab::CSoundSource* ssCount = NewGO<prefab::CSoundSource>(0);;
		if (m_startCount >= 1 && m_startCount < 4) 
		{
			ssCount->Init(L"Assets/sound/カウント音2.wav");
			ssCount->SetVolume(0.8f);
		}
		else if (m_startCount == 0)
		{
			ssCount->Init(L"Assets/sound/エアーホーン.wav");
			ssCount->SetVolume(0.8f);
		}
		ssCount->Play(false);
		m_playCountSEFlag = false;
	}
}
void SampleScene::TimeLimitCount()
{
	//制限時間の表示
	if (m_timeLimit > 0 && m_gameState == enPlaying) {
		m_timeLimit -= GameTime::GetInstance().GetFrameDeltaTime();
		//タイムリミットが二桁の時
		if (m_timeLimit >= 10)
		{
			//一の位
			int Number = (int)m_timeLimit % 10;
			TimeLimitChangesSprits(Number,1);
			//十の位
			Number = (int)m_timeLimit / 10;
			TimeLimitChangesSprits(Number,10);
		}
		//タイムリミットが一桁の時
		else if ((int)m_timeLimit < 10)
		{
			int Number = (int)m_timeLimit;
			TimeLimitChangesSprits(Number, 0);
		}
	}
	m_oldTimeLimit = m_timeLimit;
}

void SampleScene::TimeLimitChangesSprits(int num, int numPlace)
{
	//タイムリミットが前フレームと異なるとき(一秒経ったとき)
	if ((int)m_timeLimit != m_oldTimeLimit)
	{
		//numが0～9のどの数字かによって、Initするスプライトを変える。
		switch (num)
		{
		case 0:
			if (numPlace == 1)	//numが一の位の時
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/0.dds", 500, 500);
			}
			else if (numPlace == 10)	//numが十の位の時
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/0.dds", 500, 500);
			}
			else if (numPlace == 0)		//numが一桁の時(タイムリミットが10以下の時)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/0.dds", 500, 500);
			}
			break;
		case 1:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/1.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/1.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/1.dds", 500, 500);
			}
			break;
		case 2:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/2.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/2.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/2.dds", 500, 500);
			}
			break;
		case 3:
			if (numPlace == 1)
			{			
				m_onesPlaceSpriteRender->Init("Assets/Image/3.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/3.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/3.dds", 500, 500);
			}
			break;
		case 4:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/4.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/4.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/4.dds", 500, 500);
			}
			break;
		case 5:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/5.dds", 500, 500);
			}
			else if (numPlace == 10)
			{			
				m_tensPlaceSpriteRender->Init("Assets/Image/5.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/5.dds", 500, 500);
			}
			break;
		case 6:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/6.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/6.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/6.dds", 500, 500);
			}
			break;
		case 7:
			if (numPlace == 1)
			{			
				m_onesPlaceSpriteRender->Init("Assets/Image/7.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/7.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/7.dds", 500, 500);
			}
			break;
		case 8:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/8.dds", 500, 500);
			}
			else if (numPlace == 10)
			{			
				m_tensPlaceSpriteRender->Init("Assets/Image/8.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/8.dds", 500, 500);
			}
			break;
		case 9:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				//一桁になったら、十の位表示位置を真ん中に。
				DeleteGO(m_tensPlaceSpriteRender);
				m_onesPlaceSpriteRender->SetPosition({ 0.0f, 315.0f, 0.0f });
				m_onesPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);
			}
			break;
		default:
			break;
		}
	}
}