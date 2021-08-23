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
#include "OpeningCamera.h"
#include "FinalHit.h"

namespace
{
	const Vector3 STAGELIGHT_DIRECTION = { -1.0f,-1.0f,1.0f };				//ステージにあるスポットライトの向き
	const Vector3 STAGELIGHT_COLOR = { 0.8f,0.8f,0.8f };					//スポットライトの色
	const Vector3 PLAYER1_STARTPOSITION = { 760.0f,0.0f,400.0f };			//プレイヤー1のスタート座標
	const Vector3 PLAYER2_STARTPOSITION = { -760.0f,0.0f,-400.0f };			//プレイヤー2のスタート座標
	const Vector3 STARTDIRECTION_PLAYER1_TOCAMERA = { 1.0f,0.0f,0.0f };		//プレイヤー1のスタート時のカメラへの向き
	const Vector3 STARTDIRECTION_PLAYER2_TOCAMERA = { -1.0f,0.0f,0.0f };	//プレイヤー2のスタート時のカメラへの向き
	const Vector3 STARTDIRECTION_PLAYER1_MODEL = { -1.0f,0.0f,0.0f };		//プレイヤー1のスタート時のモデルの向き
	const Vector3 STARTDIRECTION_PLAYER2_MODEL = { 1.0f,0.0f,0.0f };		//プレイヤー2のスタート時のモデルの向き
	const Vector3 SPRITE_DELIMITLINE_POSITION = { 0.0f,0.0f,0.0f };			//画面分割線スプライトの座標
	const int SPRITE_DELIMITELINE_WIDTH = 40;								//画面分割線スプライトの幅
	const int SPRITE_DELIMITELINE_HEIGHT = 720;								//画面分割線スプライトの高さ
	const Vector3 SPRITE_HPCOVER_POSITION = { 0.0f,304.0f,0.0f };			//HPバーのカバーの座標
	const int SPRITE_HPCOVER_WIDTH = 1280;									//HPバーのカバーの幅
	const int SPRITE_HPCOVER_HEIGHT = 112;									//HPバーのカバーの高さ
	const Vector3 SPRITE_TIMERBASE_POSITION = { 0.0f,300.0f,0.0f };			//タイマースプライトの座標
	const int SPRITE_TIMERBASE_WIDTH = 272;									//タイマースプライトの幅
	const int SPRITE_TIMERBASE_HEIGHT = 120;								//タイマースプライトの高さ
	const Vector3 SPRITE_STARTCOUNTDOWN_3_TOP_STARTPOSITION
				= { -900.0f,100.0f,0.0f };									//カウントダウン3の上の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_3_MIDDLE_STARTPOSITION
				= { 900.0f,0.0f,0.0f };										//真ん中の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_3_BOTTOM_STARTPOSITION
				= { -900.0f,-100.0f,0.0f };									//下の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_3_RIGHT_STARTPOSITION
				= { 50.0f,-600.0f,0.0f };									//右の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_2_TOP_STARTPOSITION
				= { 900.0f,100.0f,0.0f };									//カウントダウン2の上の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_2_MIDDLE_STARTPOSITION
				= { -900.0f,0.0f,0.0f };									//真ん中の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_2_BOTTOM_STARTPOSITION
				= { 900.0f,-100.0f,0.0f };									//下の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_2_RIGHT_STARTPOSITION
				= { 50.0f,-550.0f,0.0f };									//右の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_2_LEFT_STARTPOSITION
				= { -50.0f,550.0f,0.0f };									//左の棒の初期座標
	const Vector3 SPRITE_STARTCOUNTDOWN_1_STARTPOSITION
				= { 0.0f,600.0f,0.0f };										//カウントダウン1の棒の初期座標
	const int SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH = 100;				//カウントダウンの横棒の幅
	const int SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT = 24;				//カウントダウンの横棒の高さ
	const int SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_WIDTH = 24;				//カウントダウン3の縦棒の幅
	const int SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_HEIGHT = 200;				//カウントダウン3の縦棒の高さ
	const int SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_WIDTH = 24;				//カウントダウン2の縦棒の幅
	const int SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_HEIGHT = 100;				//カウントダウン2の縦棒の高さ
	const int SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_WIDTH = 36;				//カウントダウン1の縦棒の幅
	const int SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_HEIGHT = 200;				//カウントダウン1の縦棒の高さ
	const Vector3 SPRITE_FINALCOUNTDOWN_POSITION = { 0.0f,0.0f,0.0f };		//終了カウントダウンのスプライトの座標
	const int SPRITE_FINALCOUNTDOWN_WIDTH = 300;							//終了カウントダウンのスプライトの幅
	const int SPRITE_FINALCOUNTDOWN_HEGIHT = 300;							//終了カウントダウンのスプライトの高さ
	const Vector3 SPRITE_FINALCOUNTDOWN_SCALE_START = { 3.0f,3.0f,3.0f };	//終了カウントダウンのスプライトの初期拡大率
	const float SPRITE_FINALCOUNTDOWN_SCALE_SHRINK = 1.0f;					//終了カウントダウンの収縮した時の拡大率
	const float SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY = 0.8f;					//終了カウントダウンの表示時の拡大率
	const float SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR = 0.0f;				//終了カウントダウンの消滅時の拡大率
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK = 6;					//終了カウントダウンの収縮の時間
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY = 24;					//終了カウントダウンの表示の時間
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR = 6;				//終了カウントダウンの消滅の時間
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK = 0;					//終了カウントダウンの収縮のディレイ
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY = 6;					//終了カウントダウンの表示のディレイ
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR = 30;				//終了カウントダウンの消滅のディレイ
	const Vector4 SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT
				= { 1.0f,1.0f,1.0f,0.0f };									//終了カウントダウンスプライトの乗算カラー　透明
	const Vector4 SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT
				= { 1.0f,1.0f,1.0f,0.5f };									//終了カウントダウンスプライトの乗算カラー　半透明
	const int SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME = 6;					//終了カウントダウンスプライトを透明にする時間
	const int SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY = 30;					//終了カウントダウンスプライトを透明にするディレイ
	const Vector3 SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_SINGLEDIGIT
				= { 0.0f, 315.0f, 0.0f };									//制限時間スプライト1桁時の1の位の座標
	const Vector3 SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_DOUBLEDIGIT
				= { 27.0f, 315.0f, 0.0f };									//制限時間スプライト2桁時の1の位の座標
	const Vector3 SPRITE_TIMELIMIT_POSITION_TENTHPLACE_OF_DOUBLEDIGIT
				= { -27.0f, 315.0f, 0.0f };									//制限時間スプライト2桁時の10の位の座標
	const Vector3 SPRITE_TIMELIMIT_SCALE = { 0.25f,0.25f,1.0f };			//制限時間スプライトの拡大率
	const int SPRITE_TIMELIMIT_WIDTH = 500;									//制限時間スプライトの幅
	const int SPRITE_TIMELIMIT_HEIGHT = 500;								//制限時間スプライトの高さ
	const float SOUND_BGM_GAME_VOLUME = 0.3f;								//ゲームBGMの音量
	const float SOUND_SE_STARTCOUNTDOWN_VOLUME = 0.8f;						//カウントダウンSEのボリューム
	const float SOUND_SE_STARTHORN_VOLUME = 0.8f;							//スタートのホーンSEのボリューム
	const int GAMEENDTIMER_ROUNDCOUNTER_SHOW = 450;							//ゲーム終了時　ラウンド取得表示のタイマー
	const int GAMEENDTIMER_START_TRANSITION = 650;							//ゲーム終了時　トランジションスタートのタイマー
	const int GAMEENDTIMER_GOTO_RESULT = 700;								//ゲーム終了時　リザルトシーンに移行するタイマー
	const int DRAWTIMER_START_TRANSITION = 45;								//引き分け時　　トランジションスタートのタイマー
	const int DRAWTIMER_GOTO_REMATCH = 0;									//引き分け時　　次の試合へ移行するタイマー
	const Vector2 FONT_SKIP_POSITION = { 270.0f,-320.0f };					//オープニングカメラのスキップフォントの座標
	const Vector2 FONT_SKIP_SCALE = { 0.6f,0.6f };							//オープニングカメラのスキップフォントの拡大率
	const Vector2 FONT_DRAW_POSITION = { -185.0f, 130.0f };					//引き分けフォントの座標
	const Vector2 FONT_DRAW_SCALE = { 2.0f, 2.0f };							//引き分けフォントの拡大率
	const Vector4 FONT_SHADOWCOLOR = { 0,0,0,1 };							//フォントの影の色
	const float FONT_SHADOWOFFSET = 2.0f;									//フォントの影の太さ
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT
				= { 890.0f,0.0f };											//スタートカウントダウン　右移動の移動量
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT
				= { 10.0f,0.0f };											//スタートカウントダウン　右移動(小)の移動量
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT
				= { -890.0f,0.0f };											//スタートカウントダウン　左移動の移動量
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT
				= { -10.0f,0.0f };											//スタートカウントダウン　左移動(小)の移動量
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP
				= { 0.0f,590.0f };											//スタートカウントダウン　上移動の移動量
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP_SHORT
				= { 0.0f,10.0f };											//スタートカウントダウン　上移動(小)の移動量
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM
				= { 0.0f,-590.0f };											//スタートカウントダウン　下移動の移動量
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM_SHORT
				= { 0.0f,-10.0f };											//スタートカウントダウン　下移動(小)の移動量
	const int SPRITE_STARTCOUNTDOWN_MOVETIME_FAST = 12;						//スタートカウントダウン　速い移動の移動時間
	const int SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW = 48;						//スタートカウントダウン　遅い移動の移動時間
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE = 0;					//スタートカウントダウン　組み立て開始のディレイ	
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY = 12;					//スタートカウントダウン　表示のディレイ
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE = 60;				//スタートカウントダウン　分解のディレイ
	const Vector3 SPRITE_START_POSITION = { 0.0f, 0.0f, 0.0f };				//スタートスプライトの座標
	const Vector3 SPRITE_START_SCALE = { 1.5f, 1.5f, 1.5f };				//スタートスプライトの拡大率
	const int SPRITE_START_WIDTH = 324;										//スタートスプライトの幅
	const int SPRITE_START_HEIGHT = 64;										//スタートスプライトの高さ
	const int NUMBER_TIMELIMIT_SPRITE_0 = 0;								//制限時間スプライトの数字 0
	const int NUMBER_TIMELIMIT_SPRITE_1 = 1;								//制限時間スプライトの数字 1
	const int NUMBER_TIMELIMIT_SPRITE_2 = 2;								//制限時間スプライトの数字 2
	const int NUMBER_TIMELIMIT_SPRITE_3 = 3;								//制限時間スプライトの数字 3
	const int NUMBER_TIMELIMIT_SPRITE_4 = 4;								//制限時間スプライトの数字 4
	const int NUMBER_TIMELIMIT_SPRITE_5 = 5;								//制限時間スプライトの数字 5
	const int NUMBER_TIMELIMIT_SPRITE_6 = 6;								//制限時間スプライトの数字 6
	const int NUMBER_TIMELIMIT_SPRITE_7 = 7;								//制限時間スプライトの数字 7
	const int NUMBER_TIMELIMIT_SPRITE_8 = 8;								//制限時間スプライトの数字 8
	const int NUMBER_TIMELIMIT_SPRITE_9 = 9;								//制限時間スプライトの数字 9


	//他でも使いそうなのでどこかに分離?
	const int NUMBER_MAGNET_STATUS = 2;
}

float GameScene::m_maxTimeLimit = 0;
float GameScene::m_P1Sensitivity;
float GameScene::m_P2Sensitivity;

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
	//ライトカメラに平行光源の向きを伝える
	CascadeShadow::GetInstance()->SetLightCameraDirection(STAGELIGHT_DIRECTION);
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

	//スキップフォント
	m_skipFontRender = NewGO<prefab::CFontRender>(4);
	m_skipFontRender->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);
	m_skipFontRender->SetPosition(FONT_SKIP_POSITION);
	m_skipFontRender->SetScale(FONT_SKIP_SCALE);
	m_skipFontRender->SetShadowFlag(true);
	m_skipFontRender->SetShadowColor(FONT_SHADOWCOLOR);
	m_skipFontRender->SetShadowOffset(FONT_SHADOWOFFSET);
	m_skipFontRender->SetText(L"PRESS A TO SKIP");

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
	m_onesPlaceSpriteRender->SetUseSpriteSupporterFlag(false);
	m_onesPlaceSpriteRender->Init(onesPlacePathName, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);

	//タイムリミットの二桁目
	int tensPlace = (int)m_timeLimit / 10;
	char tensPlacePathName[256];
	sprintf_s(tensPlacePathName, "Assets/Image/%d.dds", tensPlace);
	m_tensPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_tensPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_TENTHPLACE_OF_DOUBLEDIGIT);
	m_tensPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
	m_tensPlaceSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_tensPlaceSpriteRender->SetUseSpriteSupporterFlag(false);
	m_tensPlaceSpriteRender->Init(tensPlacePathName, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);

	//音を再生
	m_gameBGM = NewGO<prefab::CSoundSource>(0);
	m_gameBGM->Init(L"Assets/sound/GameBGM.wav", SoundType::enBGM);
	m_gameBGM->SetVolume(SOUND_BGM_GAME_VOLUME);
	m_gameBGM->Play(true);
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, true);

	m_roundCounter = FindGO<RoundCounter>("roundcounter");
	m_roundCounter->MoveToGamePosition();
	return true;
}

void GameScene::Update()
{
	if (m_gameState == enBirdseye)
	{
		if (m_openingCameraPlayer1 == nullptr) 
		{
			m_openingCameraPlayer1 = NewGO<OpeningCamera>(0);
			m_openingCameraPlayer2 = NewGO<OpeningCamera>(0);
			m_openingCameraPlayer1->SetPlayerNum(0);
			m_openingCameraPlayer2->SetPlayerNum(1);
			m_openingCameraPlayer1->SetCameraFront(g_camera3D[0]->GetForward());
			m_openingCameraPlayer2->SetCameraFront(g_camera3D[1]->GetForward());
			m_openingCameraPlayer1->SetPlayerPos(m_player1->GetPosition());
			m_openingCameraPlayer2->SetPlayerPos(m_player2->GetPosition());
		}
		m_roundCounter->RoundAnnounce();
	}

	//スタートカウントダウン
	if (m_gameState == enStartCountDown) {
		if (m_isOpeningCameraOn == true)
		{
			DeleteGO(m_openingCameraPlayer1);
			DeleteGO(m_openingCameraPlayer2);
			m_isOpeningCameraOn = false;
		}
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
		//ゲームが決着した最初のフレーム
		if (m_isGameEndFirstFrame == true)
		{
			m_finalHit = NewGO<FinalHit>(0);
			//弾も消す
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
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_onesPlaceSpriteRender);
			if (int(m_timeLimit) >= 10)
			{
				DeleteGO(m_tensPlaceSpriteRender);
			}

			m_isGameEndFirstFrame = false;

			if (m_player1->IsLose() == true)
			{
				m_finalHit->SetLoserNum(NUMBER_PLAYER1);
				m_roundCounter->SubmitRoundWinner(NUMBER_PLAYER2);
			}
			else if (m_player2->IsLose() == true)
			{
				m_finalHit->SetLoserNum(NUMBER_PLAYER2);
				m_roundCounter->SubmitRoundWinner(NUMBER_PLAYER1);
			}

			m_roundCounter->MoveToResultPosition();
		}

		m_gameEndCount++;

		if (m_gameEndCount == GAMEENDTIMER_ROUNDCOUNTER_SHOW)
		{
			m_roundCounter->StartResultMove();
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
				DeleteGO(m_finalHit);
				DeleteGO(this);
			}
			else
			{
				NewGO<ResultScene>(0, "resultscene");
				ResultScene* resultscene = FindGO<ResultScene>("resultscene");
				resultscene->SetLoserNum(m_finalHit->GetLoserNum());
				DeleteGO(m_roundCounter);
				DeleteGO(m_finalHit);
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
		m_drawFontRender->SetShadowColor(FONT_SHADOWCOLOR);
		m_drawFontRender->SetShadowOffset(FONT_SHADOWOFFSET);
		m_drawFontRender->SetText(L"DRAW!");

		//ボイス再生
		SoundOneShotPlay(L"Assets/sound/Rakutan.wav");
		SoundOneShotPlay(L"Assets/sound/Draw.wav", SOUND_SE_VOICE_VOLUME);
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

		SoundOneShotPlay(L"Assets/sound/CountDown3.wav", SOUND_SE_VOICE_VOLUME);

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

		SoundOneShotPlay(L"Assets/sound/CountDown2.wav", SOUND_SE_VOICE_VOLUME);

		m_startCount2_Flag = true;
	}
	if (m_startCount == 1 && m_startCount1_Flag == false) {

		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		SoundOneShotPlay(L"Assets/sound/CountDown1.wav", SOUND_SE_VOICE_VOLUME);

		m_startCount1_Flag = true;
	}
	else if (m_startCount < 0) {
		//カウントダウンに使用したスプライトを削除
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
			ssCount->Init(L"Assets/sound/CountSE.wav", SoundType::enSE);
			ssCount->SetVolume(SOUND_SE_STARTCOUNTDOWN_VOLUME);
		}
		else if (m_startCount == 0)
		{
			ssCount->Init(L"Assets/sound/AirHorn.wav", SoundType::enSE);
			ssCount->SetVolume(SOUND_SE_STARTHORN_VOLUME);
			SoundOneShotPlay(L"Assets/sound/CountDown0.wav", SOUND_SE_VOICE_VOLUME);

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
			int Number = static_cast<int>(m_timeLimit) % 10;
			TimeLimitChangesSprits(Number, enOnesPlaceOfDoubleDigit);
			//十の位
			Number = static_cast<int>(m_timeLimit) / 10;
			TimeLimitChangesSprits(Number, enTenthPlaceOfDoubleDigit);
		}
		//タイムリミットが一桁の時
		else if (static_cast<int>(m_timeLimit) < 10)
		{
			int Number = static_cast<int>(m_timeLimit);
			TimeLimitChangesSprits(Number, enOnesPlaceOfSingleDigit);
		}
	}
	m_oldTimeLimit = static_cast<int>(m_timeLimit);
}

void GameScene::TimeLimitChangesSprits(int num, TimeLimitSpriteDigit numPlace)
{
	//前フレームの制限時間の秒数と同じ場合return
	if (static_cast<int>(m_timeLimit) == m_oldTimeLimit)
	{
		return;
	}

	//タイムリミットが前フレームと異なるとき(一秒経ったとき)
	
	//numが0～9のどの数字かによって、Initするddsファイルのパスを変える。
	char ddsPath[256];
	sprintf(ddsPath, "Assets/Image/%d.dds", num);

	//数字の位置によって分岐
	//Initで直接変更するとディスクリプタヒープを圧迫するので削除してから作成する
	switch (numPlace)
	{
	case enOnesPlaceOfDoubleDigit://二桁時の一の位
		DeleteGO(m_onesPlaceSpriteRender);
		m_onesPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
		m_onesPlaceSpriteRender->Init(ddsPath, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
		m_onesPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_DOUBLEDIGIT);
		m_onesPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
		m_onesPlaceSpriteRender->SetUseSpriteSupporterFlag(false);
		break;
	case enTenthPlaceOfDoubleDigit://二桁時の十の位
		DeleteGO(m_tensPlaceSpriteRender);
		m_tensPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
		m_tensPlaceSpriteRender->Init(ddsPath, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
		m_tensPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_TENTHPLACE_OF_DOUBLEDIGIT);
		m_tensPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
		m_tensPlaceSpriteRender->SetUseSpriteSupporterFlag(false);
		break;
	case enOnesPlaceOfSingleDigit://一桁時の一の位
		DeleteGO(m_onesPlaceSpriteRender);
		m_onesPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
		m_onesPlaceSpriteRender->Init(ddsPath, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
		m_onesPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_SINGLEDIGIT);
		m_onesPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
		m_onesPlaceSpriteRender->SetUseSpriteSupporterFlag(false);

		//残り9秒になった時は十の位のスプライトを削除
		if (num == NUMBER_TIMELIMIT_SPRITE_9)
		{
			DeleteGO(m_tensPlaceSpriteRender);
		}
		break;
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