#include "stdafx.h"
#include "GameScene.h"
#include "Player.h"
#include "BackGround.h"
#include "SkyBoard.h"
#include "Debris.h"
#include "DebrisBlock.h"
#include "Bomb.h"
#include "Repulsion.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include <random>

#include "RoundCounter.h"

namespace
{
	const Vector3 STAGELIGHT_DIRECTION = { -1.0f,-1.0f,1.0f };
	const Vector3 STAGELIGHT_COLOR = { 0.8f,0.8f,0.8f };
	const Vector3 PLAYER1_STARTPOSITION = { 760.0f,0.0f,400.0f };
	const Vector3 PLAYER2_STARTPOSITION = { -760.0f,0.0f,-400.0f };
	const Vector3 STARTDIRECTION_PLAYER1_TOCAMERA = { 1.0f,0.0f,0.0f };
	const Vector3 STARTDIRECTION_PLAYER2_TOCAMERA = { -1.0f,0.0f,0.0f };
	const Vector3 STARTDIRECTION_PLAYER1_MODEL = { -1.0f,0.0f,0.0f };
	const Vector3 STARTDIRECTION_PLAYER2_MODEL = { 1.0f,0.0f,0.0f };
	const Vector3 SPRITE_DELIMITLINE_POSITION = { 0.0f,0.0f,0.0f };
	const int SPRITE_DELIMITELINE_WIDTH = 40;
	const int SPRITE_DELIMITELINE_HEIGHT = 720;
	const Vector3 SPRITE_HPCOVER_POSITION = { 0.0f,304.0f,0.0f };
	const int SPRITE_HPCOVER_WIDTH = 1280;
	const int SPRITE_HPCOVER_HEIGHT = 112;
	const Vector3 SPRITE_TIMERBASE_POSITION = { 0.0f,300.0f,0.0f };
	const int SPRITE_TIMERBASE_WIDTH = 272;
	const int SPRITE_TIMERBASE_HEIGHT = 120;
	const Vector3 SPRITE_STARTCOUNTDOWN_3_TOP_STARTPOSITION = { -900.0f,100.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_3_MIDDLE_STARTPOSITION = { 900.0f,0.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_3_BOTTOM_STARTPOSITION = { -900.0f,-100.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_3_RIGHT_STARTPOSITION = { 50.0f,-600.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_2_TOP_STARTPOSITION = { 900.0f,100.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_2_MIDDLE_STARTPOSITION = { -900.0f,0.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_2_BOTTOM_STARTPOSITION = { 900.0f,-100.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_2_RIGHT_STARTPOSITION = { 50.0f,-550.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_2_LEFT_STARTPOSITION = { -50.0f,550.0f,0.0f };
	const Vector3 SPRITE_STARTCOUNTDOWN_1_STARTPOSITION = { 0.0f,600.0f,0.0f };
	const int SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH = 100;
	const int SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT = 24;
	const int SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_WIDTH = 24;
	const int SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_HEIGHT = 200;
	const int SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_WIDTH = 24;
	const int SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_HEIGHT = 100;
	const int SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_WIDTH = 36;
	const int SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_HEIGHT = 200;
	const Vector3 SPRITE_FINALCOUNTDOWN_POSITION = { 0.0f,0.0f,0.0f };
	const int SPRITE_FINALCOUNTDOWN_WIDTH = 300;
	const int SPRITE_FINALCOUNTDOWN_HEGIHT = 300;
	const Vector3 SPRITE_FINALCOUNTDOWN_SCALE_START = { 3.0f,3.0f,3.0f };
	const float SPRITE_FINALCOUNTDOWN_SCALE_SHRINK = 1.0f;
	const float SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY = 0.8f;
	const float SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR = 0.0f;
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK = 6;
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY = 24;
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR = 6;
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK = 0;
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY = 6;
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR = 30;
	const Vector4 SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT = { 1.0f,1.0f,1.0f,0.0f };
	const Vector4 SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT = { 1.0f,1.0f,1.0f,0.5f };
	const int SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME = 6;
	const int SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY = 30;
	const Vector3 SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_SINGLEDIGIT = { 0.0f, 315.0f, 0.0f };
	const Vector3 SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_DOUBLEDIGIT = { 27.0f, 315.0f, 0.0f };
	const Vector3 SPRITE_TIMELIMIT_POSITION_TENTHPLACE_OF_DOUBLEDIGIT = { -27.0f, 315.0f, 0.0f };
	const Vector3 SPRITE_TIMELIMIT_SCALE = { 0.25f,0.25f,1.0f };
	const int SPRITE_TIMELIMIT_WIDTH = 500;
	const int SPRITE_TIMELIMIT_HEIGHT = 500;
	const float SOUND_BGM_GAME_VOLUME = 0.3f;
	const float SOUND_SE_STARTCOUNTDOWN_VOLUME = 0.8f;
	const float SOUND_SE_STARTHORN_VOLUME = 0.8f;
	const int GAMEENDTIMER_ROUNDCOUNTER_SHOW = 450;
	const int GAMEENDTIMER_START_TRANSITION = 650;
	const int GAMEENDTIMER_GOTO_RESULT = 700;
	const int DRAWTIMER_START_TRANSITION = 45;
	const int DRAWTIMER_GOTO_REMATCH = 0;
	const Vector2 FONT_DRAW_POSITION = { -185.0f, 130.0f };
	const Vector2 FONT_DRAW_SCALE = { 2.0f, 2.0f };
	const Vector4 FONT_DRAW_SHADOWCOLOR = { 0,0,0,1 };
	const float FONT_DRAW_SHADOWOFFSET = 2.0f;
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT = { 890.0f,0.0f };
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT = { 10.0f,0.0f };
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT = { -890.0f,0.0f };
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT = { -10.0f,0.0f };
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP = { 0.0f,590.0f };
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP_SHORT = { 0.0f,10.0f };
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM = { 0.0f,-590.0f };
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM_SHORT = { 0.0f,-10.0f };
	const int SPRITE_STARTCOUNTDOWN_MOVETIME_FAST = 12;
	const int SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW = 48;
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE = 0;
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY = 12;
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE = 60;
	const Vector3 SPRITE_START_POSITION = { 0.0f, 0.0f, 0.0f };
	const Vector3 SPRITE_START_SCALE = { 1.5f, 1.5f, 1.5f };
	const int SPRITE_START_WIDTH = 324;
	const int SPRITE_START_HEIGHT = 64;
	const int NUMBER_TIMELIMIT_SPRITE_0 = 0;
	const int NUMBER_TIMELIMIT_SPRITE_1 = 1;
	const int NUMBER_TIMELIMIT_SPRITE_2 = 2;
	const int NUMBER_TIMELIMIT_SPRITE_3 = 3;
	const int NUMBER_TIMELIMIT_SPRITE_4 = 4;
	const int NUMBER_TIMELIMIT_SPRITE_5 = 5;
	const int NUMBER_TIMELIMIT_SPRITE_6 = 6;
	const int NUMBER_TIMELIMIT_SPRITE_7 = 7;
	const int NUMBER_TIMELIMIT_SPRITE_8 = 8;
	const int NUMBER_TIMELIMIT_SPRITE_9 = 9;


	//他でも使いそうなのでどこかに分離?
	const int NUMBER_MAGNET_STATUS = 2;
}

float GameScene::m_maxTimeLimit = 0;

GameScene::~GameScene()
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
}

bool GameScene::Start()
{
	//ステージのライトを作成
	m_stageLight = NewGO<prefab::CDirectionLight>(0);
	m_stageLight->SetDirection(STAGELIGHT_DIRECTION);
	m_stageLight->SetColor(STAGELIGHT_COLOR);

	m_player1 = NewGO<Player>(0, "Player");
	m_player1->SetPosition(PLAYER1_STARTPOSITION);
	m_player1->SetPlayerNum(NUMBER_PLAYER1);

	std::random_device device;
	std::mt19937_64 rnd(device());

	//プレイヤー1の磁力の状態をランダムに決定
	int mag = rnd() % NUMBER_MAGNET_STATUS;
	if (mag)
	{
		m_player1->SetMagnetState(MAGNETSTATE_REPULSION);
	}
	else
	{
		m_player1->SetMagnetState(MAGNETSTATE_GRAVITY);
	}
	m_player1->SetToCameraDirection(STARTDIRECTION_PLAYER1_TOCAMERA);
	m_player1->SetCameraFront(STARTDIRECTION_PLAYER1_MODEL);
	m_player1->SetSensitivity(m_P1Sensitivity);

	m_player2 = NewGO<Player>(0, "Player");
	m_player2->SetPosition(PLAYER2_STARTPOSITION);
	m_player2->SetPlayerNum(NUMBER_PLAYER2);

	//プレイヤー2の磁力の状態をランダムに決定
	mag = rnd() % NUMBER_MAGNET_STATUS;
	if (mag)
	{
		m_player2->SetMagnetState(MAGNETSTATE_REPULSION);
	}
	else
	{
		m_player2->SetMagnetState(MAGNETSTATE_GRAVITY);
	}
	m_player2->SetToCameraDirection(STARTDIRECTION_PLAYER2_TOCAMERA);
	m_player2->SetCameraFront(STARTDIRECTION_PLAYER2_MODEL);
	m_player2->SetSensitivity(m_P2Sensitivity);

	//各プレイヤーに敵を渡す
	m_player2->SetEnemy(m_player1);
	m_player1->SetEnemy(m_player2);
	
	//ステージの作成
	m_backGround = NewGO<BackGround>(0, "background");

	//空を作成。キューブマップじゃなくてただの板ポリ。
	m_sky = NewGO<SkyBoard>(0);

	//すべて作成し終わった所で2画面にする。
	GameObjectManager::GetInstance()->Set2ScreenMode(true);
	
	//画面を区切る線
	m_delimitLineSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_delimitLineSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_delimitLineSpriteRender->SetPosition(SPRITE_DELIMITLINE_POSITION);
	m_delimitLineSpriteRender->Init("Assets/Image/Waku.dds", SPRITE_DELIMITELINE_WIDTH, SPRITE_DELIMITELINE_HEIGHT);

	//HPのカバー
	m_HPCoverSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_HPCoverSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_HPCoverSpriteRender->SetPosition(SPRITE_HPCOVER_POSITION);
	m_HPCoverSpriteRender->Init("Assets/Image/HP_Cover.dds", SPRITE_HPCOVER_WIDTH, SPRITE_HPCOVER_HEIGHT);

	//タイマーの枠
	m_TimerBaseSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_TimerBaseSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_TimerBaseSpriteRender->SetPosition(SPRITE_TIMERBASE_POSITION);
	m_TimerBaseSpriteRender->Init("Assets/Image/Timer_Base.dds", SPRITE_TIMERBASE_WIDTH, SPRITE_TIMERBASE_HEIGHT);

	//カウント3
	m_startCountDown_3_Top = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Top->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_3_Top->SetPosition(SPRITE_STARTCOUNTDOWN_3_TOP_STARTPOSITION);
	m_startCountDown_3_Top->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_3_Middle = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Middle->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_3_Middle->SetPosition(SPRITE_STARTCOUNTDOWN_3_MIDDLE_STARTPOSITION);
	m_startCountDown_3_Middle->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_3_Bottom = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Bottom->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_3_Bottom->SetPosition(SPRITE_STARTCOUNTDOWN_3_BOTTOM_STARTPOSITION);
	m_startCountDown_3_Bottom->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_3_Right = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Right->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_3_Right->SetPosition(SPRITE_STARTCOUNTDOWN_3_RIGHT_STARTPOSITION);
	m_startCountDown_3_Right->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_HEIGHT);
	//カウント2
	m_startCountDown_2_Top = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Top->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Top->SetPosition(SPRITE_STARTCOUNTDOWN_2_TOP_STARTPOSITION);
	m_startCountDown_2_Top->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_2_Middle = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Middle->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Middle->SetPosition(SPRITE_STARTCOUNTDOWN_2_MIDDLE_STARTPOSITION);
	m_startCountDown_2_Middle->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_2_Bottom = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Bottom->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Bottom->SetPosition(SPRITE_STARTCOUNTDOWN_2_BOTTOM_STARTPOSITION);
	m_startCountDown_2_Bottom->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_2_Right = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Right->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Right->SetPosition(SPRITE_STARTCOUNTDOWN_2_RIGHT_STARTPOSITION);
	m_startCountDown_2_Right->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_HEIGHT);
	//
	m_startCountDown_2_Left = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Left->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Left->SetPosition(SPRITE_STARTCOUNTDOWN_2_LEFT_STARTPOSITION);
	m_startCountDown_2_Left->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_HEIGHT);
	//カウント1
	m_startCountDown_1 = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_1->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_1->SetPosition(SPRITE_STARTCOUNTDOWN_1_STARTPOSITION);
	m_startCountDown_1->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_HEIGHT);
	//最後のカウント
	m_finalCountDown = NewGO<prefab::CSpriteRender>(10);
	m_finalCountDown->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_finalCountDown->SetPosition(SPRITE_FINALCOUNTDOWN_POSITION);
	m_finalCountDown->Init("Assets/Image/count3.dds", SPRITE_FINALCOUNTDOWN_WIDTH, SPRITE_FINALCOUNTDOWN_HEGIHT);
	m_finalCountDown->SetMulColor(SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT);

	//制限時間をセット
	m_timeLimit = m_maxTimeLimit;

	//タイムリミットの一桁目
	int onesPlace = (int)m_timeLimit % 10;
	char onesPlacePathName[256];
	sprintf_s(onesPlacePathName, "Assets/Image/%d.dds", onesPlace);
	m_onesPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_onesPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_DOUBLEDIGIT);
	m_onesPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
	m_onesPlaceSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_onesPlaceSpriteRender->Init(onesPlacePathName, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);

	//タイムリミットの二桁目
	int tensPlace = (int)m_timeLimit / 10;
	char tensPlacePathName[256];
	sprintf_s(tensPlacePathName, "Assets/Image/%d.dds", tensPlace);
	m_tensPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_tensPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_TENTHPLACE_OF_DOUBLEDIGIT);
	m_tensPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
	m_tensPlaceSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_tensPlaceSpriteRender->Init(tensPlacePathName, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);

	//音を再生
	m_gameBGM = NewGO<prefab::CSoundSource>(0);
	m_gameBGM->Init(L"Assets/sound/サイバー風BGM.wav", SoundType::enBGM);
	m_gameBGM->SetVolume(SOUND_BGM_GAME_VOLUME);
	m_gameBGM->Play(true);
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, true);

	m_roundCounter = FindGO<RoundCounter>("roundcounter");
	m_roundCounter->Disable();
	m_roundCounter->EnableGameRound();
	return true;
}

void GameScene::Update()
{

	//スタートカウントダウン
	if (m_gameState == enStartCountDown) {
		StartCountDown();
		return;
	}
	TimeLimitCount();

	//時間切れ3秒前からカウントダウンを表示する
	if (m_timeLimit <= 4 && m_timeLimit >= 0 && m_gameState != enResult && m_gameState != enDraw)
	{
		FinalCount();
	}

	//時間切れでも勝敗がついていない場合
	if(m_timeLimit <= 0 && m_gameState != enResult && m_gameState != enDraw)
	{
		//勝者判定
		WinnerJudge();
	}

	//ポーズ関連。
	//ポーズ。
	if (m_gameState == enPlaying && (g_pad[PAD_PLAYER1]->IsTrigger(enButtonStart) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonStart)))
	{
		m_gameState = enPause;

		return;
	}

	//ポーズ復帰。
	if (m_gameState == enPause && (g_pad[PAD_PLAYER1]->IsTrigger(enButtonStart) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonStart)))
	{
		m_gameState = enPlaying;

		return;
	}

	if (m_gameState == enResult)
	{	
		if (m_isGameEndFirstFrame == true)
		{
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_onesPlaceSpriteRender);
			if (int(m_timeLimit) >= 10)
			{
				DeleteGO(m_tensPlaceSpriteRender);
			}

			m_isGameEndFirstFrame = false;

			if (m_player1->m_Lose == true)
			{
				m_player1->m_loserNum = NUMBER_PLAYER1;
				m_player2->m_loserNum = NUMBER_PLAYER1;
				m_roundCounter->SubmitRoundWinner(NUMBER_PLAYER2);
				m_roundCounter->Disable();
			}
			else if (m_player2->m_Lose == true)
			{
				m_player1->m_loserNum = NUMBER_PLAYER2;
				m_player2->m_loserNum = NUMBER_PLAYER2;
				m_roundCounter->SubmitRoundWinner(NUMBER_PLAYER1);
				m_roundCounter->Disable();
			}
		}

		m_gameEndCount++;

		if (m_gameEndCount == GAMEENDTIMER_ROUNDCOUNTER_SHOW)
		{
			m_roundCounter->EnableResultRound();
		}

		if (m_gameEndCount == GAMEENDTIMER_START_TRANSITION)
		{
			TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, false);
		}

		if (m_gameEndCount > GAMEENDTIMER_GOTO_RESULT)
		{
			

			if (m_roundCounter->GetOverAllWinner() == -1)
			{
				NewGO<GameScene>(0, "gamescene");
				m_roundCounter->Disable();
				DeleteGO(this);
			}
			else
			{
				NewGO<ResultScene>(0, "resultscene");
				ResultScene* resultscene = FindGO<ResultScene>("resultscene");
				resultscene->SetLoserNum(m_player1->m_loserNum);
				DeleteGO(m_roundCounter);
				DeleteGO(this);
			}
		}
	}

	//引き分け時
	if (m_gameState == enDraw)
	{
		m_drawFontCount--;

		if (m_drawFontCount == DRAWTIMER_START_TRANSITION)
		{
			//トランジション
			TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, false);
		}

		if (m_drawFontCount == DRAWTIMER_GOTO_REMATCH)
		{
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_onesPlaceSpriteRender);
			if (int(m_timeLimit) >= 10)
			{
				DeleteGO(m_tensPlaceSpriteRender);
			}
			DeleteGO(m_drawFontRender);
			DeleteGO(m_onesPlaceSpriteRender);
			DeleteGO(m_gameBGM);
			NewGO<GameScene>(0, "gamescene");
			DeleteGO(this);
		}
	}
}

void GameScene::WinnerJudge()
{
	if (m_player1->GetHP() > m_player2->GetHP())
	{
		//1Pの勝ち
		m_player1->Win();
		m_player2->Lose();

		//リザルトシーンに移行
		m_gameState = enResult;

	}
	else if(m_player1->GetHP() < m_player2->GetHP())
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
		m_drawFontRender->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);
		m_drawFontRender->SetPosition(FONT_DRAW_POSITION);
		m_drawFontRender->SetScale(FONT_DRAW_SCALE);
		m_drawFontRender->SetShadowFlag(true);
		m_drawFontRender->SetShadowColor(FONT_DRAW_SHADOWCOLOR);
		m_drawFontRender->SetShadowOffset(FONT_DRAW_SHADOWOFFSET);
		m_drawFontRender->SetText(L"DRAW!");
	}
}

void GameScene::StartCountDown() {

	m_measureSecond += GameTime::GetInstance().GetFrameDeltaTime();		//一秒を測る。
	if (m_measureSecond >= 1.0f) {
		m_startCount--;
		m_measureSecond = 0.0f;
		m_playCountSEFlag = true;
	}

	//カウントダウン
	if (m_startCount == 3 && m_startCount3_Flag == false) {
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCount3_Flag = true;
	}
	if (m_startCount == 2 && m_startCount2_Flag == false) {

		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCount2_Flag = true;
	}
	if (m_startCount == 1 && m_startCount1_Flag == false) {

		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCount1_Flag = true;
	}

	else if (m_startCount < 0) {
		DeleteGO(m_startCountDown_3_Top);
		DeleteGO(m_startCountDown_3_Middle);
		DeleteGO(m_startCountDown_3_Bottom);
		DeleteGO(m_startCountDown_3_Right);
		DeleteGO(m_startCountDown_2_Top);
		DeleteGO(m_startCountDown_2_Middle);
		DeleteGO(m_startCountDown_2_Bottom);
		DeleteGO(m_startCountDown_2_Right);
		DeleteGO(m_startCountDown_2_Left);
		DeleteGO(m_startCountDown_1);
		DeleteGO(m_startSprite);
		m_gameState = enPlaying;
	}

	if (m_playCountSEFlag == true) {
		prefab::CSoundSource* ssCount = NewGO<prefab::CSoundSource>(0);;
		if (1 <= m_startCount && m_startCount < 4)
		{
			ssCount->Init(L"Assets/sound/カウント音2.wav", SoundType::enSE);
			ssCount->SetVolume(SOUND_SE_STARTCOUNTDOWN_VOLUME);
		}
		else if (m_startCount == 0)
		{
			ssCount->Init(L"Assets/sound/エアーホーン.wav", SoundType::enSE);
			ssCount->SetVolume(SOUND_SE_STARTHORN_VOLUME);

			//ゲームスタートカウント
			m_startSprite = NewGO<prefab::CSpriteRender>(5);
			m_startSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
			m_startSprite->Init("Assets/Image/Start.DDS" , SPRITE_START_WIDTH, SPRITE_START_HEIGHT);
			m_startSprite->SetPosition(SPRITE_START_POSITION);
			m_startSprite->SetScale(SPRITE_START_SCALE);
		}
		ssCount->Play(false);
		m_playCountSEFlag = false;
	}
}
void GameScene::TimeLimitCount()
{
	//制限時間の表示
	if (0 < m_timeLimit && m_gameState == enPlaying) {
		m_timeLimit -= GameTime::GetInstance().GetFrameDeltaTime();
		//タイムリミットが二桁の時
		if (m_timeLimit >= 10)
		{
			//一の位
			int Number = (int)m_timeLimit % 10;
			TimeLimitChangesSprits(Number, enOnesPlaceOfDoubleDigit);
			//十の位
			Number = (int)m_timeLimit / 10;
			TimeLimitChangesSprits(Number, enTenthPlaceOfDoubleDigit);
		}
		//タイムリミットが一桁の時
		else if ((int)m_timeLimit < 10)
		{
			int Number = (int)m_timeLimit;
			TimeLimitChangesSprits(Number, enOnesPlaceOfSingleDigit);
		}
	}
	m_oldTimeLimit = m_timeLimit;
}

void GameScene::TimeLimitChangesSprits(int num, TimeLimitSpriteDigit numPlace)
{
	//タイムリミットが前フレームと異なるとき(一秒経ったとき)
	if ((int)m_timeLimit != m_oldTimeLimit)
	{
		//numが0～9のどの数字かによって、Initするスプライトを変える。
		switch (num)
		{
		case NUMBER_TIMELIMIT_SPRITE_0:
			if (numPlace == enOnesPlaceOfDoubleDigit)	//numが一の位の時
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/0.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)	//numが十の位の時
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/0.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)		//numが一桁の時(タイムリミットが10以下の時)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/0.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_1:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/1.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/1.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/1.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_2:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/2.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/2.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/2.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_3:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{			
				m_onesPlaceSpriteRender->Init("Assets/Image/3.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/3.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/3.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_4:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/4.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/4.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/4.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_5:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/5.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{			
				m_tensPlaceSpriteRender->Init("Assets/Image/5.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/5.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_6:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/6.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/6.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/6.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_7:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{			
				m_onesPlaceSpriteRender->Init("Assets/Image/7.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/7.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/7.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_8:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/8.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{			
				m_tensPlaceSpriteRender->Init("Assets/Image/8.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/8.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		case NUMBER_TIMELIMIT_SPRITE_9:
			if (numPlace == enOnesPlaceOfDoubleDigit)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/9.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enTenthPlaceOfDoubleDigit)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/9.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			else if (numPlace == enOnesPlaceOfSingleDigit)
			{
				//一桁になったら、一の位表示位置を真ん中に。
				DeleteGO(m_tensPlaceSpriteRender);
				m_onesPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_SINGLEDIGIT);
				m_onesPlaceSpriteRender->Init("Assets/Image/9.dds", SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
			}
			break;
		default:
			break;
		}
	}
}

void GameScene::FinalCount() {

	if (m_timeLimit > 3.0f && m_timeLimit <= 4.0f && m_finalCount3_Flag == false) {
		m_finalCountDown->SetMulColor(SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT);
		m_finalCountDown->SetScale(SPRITE_FINALCOUNTDOWN_SCALE_START);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_SHRINK, SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK, SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR);
		m_finalCountDown->GetSpriteSupporter().SpriteColor(SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT, SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME, SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY);
		m_finalCount3_Flag = true;
	}
	if (m_timeLimit > 2.0f && m_timeLimit <= 3.0f && m_finalCount2_Flag == false) {
		m_finalCountDown->Init("Assets/Image/count2.dds", SPRITE_FINALCOUNTDOWN_WIDTH, SPRITE_FINALCOUNTDOWN_HEGIHT);
		m_finalCountDown->SetMulColor(SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT);
		m_finalCountDown->SetScale(SPRITE_FINALCOUNTDOWN_SCALE_START);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_SHRINK, SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK, SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR);
		m_finalCountDown->GetSpriteSupporter().SpriteColor(SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT, SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME, SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY);
		m_finalCount2_Flag = true;
	}
	if (m_timeLimit > 1.0f && m_timeLimit <= 2.0f && m_finalCount1_Flag == false) {
		m_finalCountDown->Init("Assets/Image/count1.dds", SPRITE_FINALCOUNTDOWN_WIDTH, SPRITE_FINALCOUNTDOWN_HEGIHT);
		m_finalCountDown->SetMulColor(SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT);
		m_finalCountDown->SetScale(SPRITE_FINALCOUNTDOWN_SCALE_START);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_SHRINK, SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK, SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR);
		m_finalCountDown->GetSpriteSupporter().SpriteColor(SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT, SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME, SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY);
		m_finalCount1_Flag = true;
	}

}