#include "stdafx.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOption.h"
#include "RoundCounter.h"

namespace
{
	const Vector3 MODEL_TITLEBACKGROUND_POSITION = { 60.0f,-300.0f,-75.0f };
	const Vector3 MODEL_TITLEBACKGROUND_SCALE = { 1.2f,1.2f ,1.2f };
	const float MODEL_TITLEBACKGROUND_ROTATIONRATE = -0.2f;

	const float POINTLIGHT_TITLEBACKGROUND_RANGE = 1500.0f;
	const Vector3 POINTLIGHT_COLOR_WHITE = { 1.0f,1.0f,1.0f };

	const Vector3 SPRITE_MONITOR_POSITION = { -265.0f,-20.0f,0.0f };
	const int SPRITE_MONITOR_WIDTH = 752;
	const int SPRITE_MONITOR_HEIGHT = 620;

	const Vector3 SPRITE_MONITORLINE_POSITION = { -290.0f,-21.0f,0.0f };
	const int SPRITE_MONITORLINE_WIDTH = 704;
	const int SPRITE_MONITORLINE_HEIGHT = 560;

	const Vector3 SPRITE_TITLELOGO_POSITION = { -320.0f,10.0f,0.0f };
	const int SPRITE_TITLELOGO_WIDTH = 660;
	const int SPRITE_TITLELOGO_HEIGHT = 495;

	const int SPRITE_MENUBAR_WIDTH = 500;
	const int SPRITE_MENUBAR_HEIGHT = 188;
	const Vector3 SPRITE_MENUBAR_SCALE_MOVINGTOBACK = { 0.5f,0.5f,1.0f };
	const Vector3 SPRITE_MENUBAR_SCALE_NOTSELECTED = { 0.7f,0.7f,1.0f };
	const Vector3 SPRITE_MENUBAR_SCALE_SELECTED = { 1.0f,1.0f,1.0f };
	const Vector3 SPRITE_MENUBAR_POSITION_TOP = { 440.0f,200.0f,0.0f };
	const Vector3 SPRITE_MENUBAR_POSITION_CENTER = { 320.0f,0.0f,0.0f };
	const Vector3 SPRITE_MENUBAR_POSITION_BOTTOM = { 440.0f,-200.0f,0.0f };
	const Vector3 SPRITE_MENUBAR_POSITION_CENTER_BACK = { 500.0f,0.0f,0.0f };

	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT1 = { 50.0f,0.0f,1.0f };
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT2 = { -125.0f,0.0f,1.0f };
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT3 = { 20.0f,0.0f,1.0f };
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT1 = { -50.0f,0.0f,1.0f };
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT2 = { 125.0f,0.0f,1.0f };
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT3 = { -20.0f,0.0f,1.0f };
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1 = 12;
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2 = 12;
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3 = 6;
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1 = 0;
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2 = 6;
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3 = 8;
	const int SPRITE_MENUBAR_MOVINGFRONT_MOVETIME = 24;
	const int SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME = 12;
	const int SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME = 12;
	const int SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY = 1;
	const int SPRITE_MENUBAR_MOVING_DELAY = 0;

	const Vector4 SPRITE_MENUBAR_COLOR_WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
	const Vector4 SPRITE_MENUBAR_COLOR_SEMITRANSPARENT = { 1.0f, 1.0f, 1.0f, 0.7f };
	const Vector4 SPRITE_MENUBAR_COLOR_TRANSPARENT = { 1.0f, 1.0f, 1.0f, 0.0f };

	const Vector3 SPRITE_ARROW_POSITION = { 600.0f,0.0f,0.0f };
	const int SPRITE_ARROW_WIDTH = 48;
	const int SPRITE_ARROW_HEIGHT = 88;

	const Vector3 CAMERA_TITLE_POSITION = { 0.0f,50.0f,200.0f };
	const Vector3 CAMERA_TITLE_TARGETPOSITION = { 0.0f,50.0f,0.0f };

	const float SOUND_BGM_TITLE_VOLUME = 0.2f;
	const float SOUND_SE_SCROLL_VOLUME = 0.8f;
	const float SOUND_SE_SELECT_VOLUME = 1.2f;

	const int COMMANDTIMER_COMMANDSTART = 0;
	const int COMMANDTIMER_SPRITEMOVE_BACKTOFRONT = 12;
	const int COMMANDTIMER_PLAY_SELECT_SE = 20;
	const int COMMANDTIME_SPRITEMOVE_FINISH = 30;
	const int COMMANDTIMER_START_TRANSITION = 40;
	const int COMMANDTIMER_EXECUTE_COMMAND = 60;
}

TitleScene::~TitleScene() 
{
	DeleteGO(m_BG_ModelRender);
	DeleteGO(m_BGLight);
	DeleteGO(m_Monitor_SpriteRender);
	DeleteGO(m_MonitorLine_SpriteRender);
	DeleteGO(m_Rogo_SpriteRender);
	DeleteGO(m_Option_SpriteRender);
	DeleteGO(m_Start_SpriteRender);
	DeleteGO(m_Exit_SpriteRender);
	DeleteGO(m_Arrow_SpriteRender);

	DeleteGO(m_option);
}
bool TitleScene::Start()
{	
	//トランジション
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::Circle, TRANSITION_TIME_NORMAL,true);
	//背景のモデル
	m_BG_ModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_BG_ModelRender->Init("Assets/modelData/TitleCylinder.tkm");
	m_BG_ModelRender->SetPosition(MODEL_TITLEBACKGROUND_POSITION);
	m_BG_ModelRender->SetScale(MODEL_TITLEBACKGROUND_SCALE);

	//背景のモデルを照らすポイントライト
	m_BGLight = NewGO<prefab::CPointLight>(0);
	m_BGLight->SetRange(POINTLIGHT_TITLEBACKGROUND_RANGE);
	m_BGLight->SetPosition(Vector3::Zero);
	m_BGLight->SetColor(POINTLIGHT_COLOR_WHITE);

	//モニター
	m_Monitor_SpriteRender =NewGO<prefab::CSpriteRender>(0);
	m_Monitor_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Monitor_SpriteRender->SetPosition(SPRITE_MONITOR_POSITION);
	m_Monitor_SpriteRender->Init("Assets/Image/Title_Monitor.dds", SPRITE_MONITOR_WIDTH, SPRITE_MONITOR_HEIGHT);

	//モニターライン
	m_MonitorLine_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_MonitorLine_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_MonitorLine_SpriteRender->SetPosition(SPRITE_MONITORLINE_POSITION);
	m_MonitorLine_SpriteRender->Init("Assets/Image/Title_MonitorLine.dds", SPRITE_MONITORLINE_WIDTH, SPRITE_MONITORLINE_HEIGHT);

	//タイトルロゴ
	m_Rogo_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Rogo_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Rogo_SpriteRender->SetPosition(SPRITE_TITLELOGO_POSITION);
	m_Rogo_SpriteRender->Init("Assets/Image/Zikai_Rogo.dds", SPRITE_TITLELOGO_WIDTH, SPRITE_TITLELOGO_HEIGHT);

	//オプション
	m_Option_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Option_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Option_SpriteRender->SetPosition(SPRITE_MENUBAR_POSITION_TOP);
	m_Option_SpriteRender->Init("Assets/Image/Title_Option.dds", SPRITE_MENUBAR_WIDTH, SPRITE_MENUBAR_HEIGHT);
	m_Option_SpriteRender->SetMulColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT);
	m_Option_SpriteRender->SetScale(SPRITE_MENUBAR_SCALE_NOTSELECTED);

	//始める
	m_Start_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Start_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Start_SpriteRender->SetPosition(SPRITE_MENUBAR_POSITION_CENTER);
	m_Start_SpriteRender->Init("Assets/Image/Title_Start.dds", SPRITE_MENUBAR_WIDTH, SPRITE_MENUBAR_HEIGHT);
	
	//やめる
	m_Exit_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Exit_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Exit_SpriteRender->SetPosition(SPRITE_MENUBAR_POSITION_BOTTOM);
	m_Exit_SpriteRender->Init("Assets/Image/Title_Exit.dds", SPRITE_MENUBAR_WIDTH, SPRITE_MENUBAR_HEIGHT);
	m_Exit_SpriteRender->SetMulColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT);
	m_Exit_SpriteRender->SetScale(SPRITE_MENUBAR_SCALE_NOTSELECTED);

	//矢印
	m_Arrow_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Arrow_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Arrow_SpriteRender->SetPosition(SPRITE_ARROW_POSITION);
	m_Arrow_SpriteRender->Init("Assets/Image/Title_Arrow.dds", SPRITE_ARROW_WIDTH, SPRITE_ARROW_HEIGHT);

	//カメラ位置の初期化
	g_camera3D[0]->SetPosition(CAMERA_TITLE_POSITION);
	g_camera3D[0]->SetTarget(CAMERA_TITLE_TARGETPOSITION);

	//BGMを再生
	m_titleBGM = NewGO<prefab::CSoundSource>(0);
	m_titleBGM->Init(L"Assets/sound/タイトル曲.wav", SoundType::enBGM);
	m_titleBGM->SetVolume(SOUND_BGM_TITLE_VOLUME);
	m_titleBGM->Play(true);	

	//オプションは最初から作っておく
	//オプションの優先度は少なくともTitleSceneより後にしなければ、項目から抜けた際に同時にオプションからも抜けてしまう
	m_option = NewGO<GameOption>(10, "gameoption");
	return true;
}
void TitleScene::Update()
{
	//自動回転
	Quaternion titleBackGroundRot = m_BG_ModelRender->GetRotation();
	Vector3 AxisY = Vector3::AxisY;
	titleBackGroundRot.Apply(AxisY);
	titleBackGroundRot.SetRotationDeg(Vector3::AxisY, MODEL_TITLEBACKGROUND_ROTATIONRATE);
	m_BG_ModelRender->SetRotation(m_BG_ModelRender->GetRotation() * titleBackGroundRot);

	if (m_selectFlag == false) {

		//決定
		if ((g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA)) && m_selectMoveFlag == false) {
			m_commandTimer = COMMANDTIMER_COMMANDSTART;
			m_selectFlag = true;
		}

		//コマンド移動 上
		if ((g_pad[PAD_PLAYER1]->IsPress(enButtonUp) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp)) && m_selectMoveFlag == false) {
			m_selectMoveFlag = true;
			m_comandUpDown = false;
			m_commandTimer = COMMANDTIMER_COMMANDSTART;

			//SE
			prefab::CSoundSource* ss2 = NewGO<prefab::CSoundSource>(0);
			ss2->Init(L"Assets/sound/タイトル画面SE1.wav", SoundType::enSE);
			ss2->SetVolume(SOUND_SE_SCROLL_VOLUME);
			ss2->Play(false);
		}
		//コマンド移動 下
		if ((g_pad[PAD_PLAYER1]->IsPress(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonDown)) && m_selectMoveFlag == false) {
			m_selectMoveFlag = true;
			m_comandUpDown = true;
			m_commandTimer = COMMANDTIMER_COMMANDSTART;

			//SE
			prefab::CSoundSource* ss3 = NewGO<prefab::CSoundSource>(0);
			ss3->Init(L"Assets/sound/タイトル画面SE1.wav", SoundType::enSE);
			ss3->SetVolume(SOUND_SE_SCROLL_VOLUME);
			ss3->Play(false);
		}

		if (m_selectMoveFlag == true) {
			//コマンド演出
			CommandMove();
		}

	}
	else {
		//決定したのでコマンド演出
		CommandSelectMove();

		//オプションの時は戻れるように。
		if ((g_pad[PAD_PLAYER1]->IsTrigger(enButtonB) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonB)) && m_titleCommand == TitleScene::TC_Option && m_option->GetSelectingState() == GameOption::enItem && m_option->isQueuing() == false) {
			m_commandTimer = COMMANDTIMER_COMMANDSTART;
			m_selectFlag = false;

			m_option->Close();

			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT1, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1, true);
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT2, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2, true);
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT3, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3, true);
		}
	}
}

void TitleScene::CommandMove() {

	if (m_commandTimer == COMMANDTIMER_COMMANDSTART) {
		if (m_comandUpDown == false) {
			//上
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			case TitleScene::TC_Option:
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			case TitleScene::TC_Exit:
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			}
		}
		else {
			//下
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			case TitleScene::TC_Option:
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			case TitleScene::TC_Exit:
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			}
		}
	}

	//透明化した項目が裏から出てくる挙動
	if (m_commandTimer == COMMANDTIMER_SPRITEMOVE_BACKTOFRONT) {
		//上
		if (m_comandUpDown == false) {
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Exit;
				break;
			case TitleScene::TC_Option:
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Start;
				break;
			case TitleScene::TC_Exit:
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Option;
				break;
			}
		}
		else {
			//下
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Option;
				break;
			case TitleScene::TC_Option:
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Exit;
				break;
			case TitleScene::TC_Exit:
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Start;
				break;
			}
		}
	}

	m_commandTimer++;

	if (m_commandTimer >= COMMANDTIME_SPRITEMOVE_FINISH) {
		//コマンド演出終了
		m_selectMoveFlag = false;
	}
}

void TitleScene::CommandSelectMove() {

	if (m_commandTimer == 0) {
		switch (m_titleCommand)
		{
		case TitleScene::TC_Start:
			//開始
			m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT1, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1, true);
			m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT2, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2, true);
			m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT3, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3, true);
			break;
		case TitleScene::TC_Option:
			//設定
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT1, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1, true);
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT2, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2, true);
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT3, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3, true);
			break;
		case TitleScene::TC_Exit:
			//終了
			m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT1, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1, true);
			m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT2, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2, true);
			m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT3, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3, true);
			break;
		}

	}

	m_commandTimer++;

	if (m_commandTimer == COMMANDTIMER_PLAY_SELECT_SE)
	{
		//SE
		prefab::CSoundSource* ss1 = NewGO<prefab::CSoundSource>(0);
		ss1->Init(L"Assets/sound/ガキーン!.wav", SoundType::enSE);
		ss1->SetVolume(SOUND_SE_SELECT_VOLUME);
		ss1->Play(false);
	}
	
	if (m_commandTimer == COMMANDTIMER_START_TRANSITION && m_titleCommand == TitleScene::TC_Start)
	{
		TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, false);
	}

	if (m_commandTimer == COMMANDTIMER_EXECUTE_COMMAND) {
		switch (m_titleCommand)
		{
		case TitleScene::TC_Start:
			//開始
			//BGMを消す
			DeleteGO(m_titleBGM);
			//試合のラウンドの計測を開始
			NewGO<RoundCounter>(0, "roundcounter");
			//switch文の中で宣言するためのスコープ
			{
				GameScene* gameScene = NewGO<GameScene>(0, "gamescene");
				gameScene->SetP1Sensitivity(m_option->GetP1Sensitivity());
				gameScene->SetP2Sensitivity(m_option->GetP2Sensitivity());
			}
			DeleteGO(this);
			break;
		case TitleScene::TC_Option:
			//設定
			m_option->Open();
			break;
		case TitleScene::TC_Exit:
			//終了
			exit(EXIT_SUCCESS);
			break;
		}
	}
}