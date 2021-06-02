#include "stdafx.h"
#include "TitleScene.h"
#include "CharacterSelect.h"
#include "SampleScene.h"
#include "GameOption.h"
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
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::Circle, 60,true);
	//背景のモデル
	m_BG_ModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_BG_ModelRender->Init("Assets/modelData/TitleCylinder.tkm");
	m_BG_ModelRender->SetPosition({ 60.0f,-300.0f,-75.0f });
	m_BG_ModelRender->SetScale({ 1.2f,1.2f ,1.2f });

	//背景のモデルを照らすポイントライト
	m_BGLight = NewGO<prefab::CPointLight>(0);
	m_BGLight->SetRange(1500.0f);
	m_BGLight->SetPosition(Vector3::Zero);
	m_BGLight->SetColor(Vector3::One);

	//モニター
	m_Monitor_SpriteRender =NewGO<prefab::CSpriteRender>(0);
	m_Monitor_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Monitor_SpriteRender->SetPosition({ -265,-20,0 });
	m_Monitor_SpriteRender->Init("Assets/Image/Title_Monitor.dds", 752, 620);

	//モニターライン
	m_MonitorLine_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_MonitorLine_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_MonitorLine_SpriteRender->SetPosition({ -290,-21,0 });
	m_MonitorLine_SpriteRender->Init("Assets/Image/Title_MonitorLine.dds", 704, 560);

	//タイトルロゴ
	m_Rogo_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Rogo_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Rogo_SpriteRender->SetPosition({ -320,10,0 });
	m_Rogo_SpriteRender->Init("Assets/Image/Zikai_Rogo.dds", 660, 495);

	//オプション
	m_Option_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Option_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Option_SpriteRender->SetPosition({ m_topPos });
	m_Option_SpriteRender->Init("Assets/Image/Title_Option.dds", 500, 188);
	m_Option_SpriteRender->SetMulColor({ m_semitrans });
	m_Option_SpriteRender->SetScale({ m_smallScale });
	//始める
	m_Start_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Start_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Start_SpriteRender->SetPosition({ m_centerPos });
	m_Start_SpriteRender->Init("Assets/Image/Title_Start.dds", 500, 188);
	
	//やめる
	m_Exit_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Exit_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Exit_SpriteRender->SetPosition({ m_bottomPos });
	m_Exit_SpriteRender->Init("Assets/Image/Title_Exit.dds", 500, 188);
	m_Exit_SpriteRender->SetMulColor({ m_semitrans });
	m_Exit_SpriteRender->SetScale({ m_smallScale });

	//矢印
	m_Arrow_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Arrow_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Arrow_SpriteRender->SetPosition({ 600,0,0 });
	m_Arrow_SpriteRender->Init("Assets/Image/Title_Arrow.dds", 48, 88);
	//カメラ位置の初期化
	g_camera3D[0]->SetPosition({ 0.0f,50.0f,200.0f });
	g_camera3D[0]->SetTarget({ 0.0f,50.0f,0.0f });
	//BGMを再生
	ssBGM = NewGO<prefab::CSoundSource>(0);
	ssBGM->Init(L"Assets/sound/タイトル曲.wav", SoundType::enBGM);
	ssBGM->SetVolume(0.2f);
	ssBGM->Play(true);	

	//オプションは最初から作っておく
	//オプションの優先度は少なくともTitleSceneより後にしなければ、項目から抜けた際に同時にオプションからも抜けてしまう
	m_option = NewGO<GameOption>(10, "gameoption");
	return true;
}
void TitleScene::Update()
{
	//自動回転
	Quaternion rot = m_BG_ModelRender->GetRotation();
	Vector3 AxisY = { 0.0f,  1.0f,  0.0f };
	rot.Apply(AxisY);
	rot.SetRotationDeg(Vector3::AxisY, -0.2f);
	m_BG_ModelRender->SetRotation(m_BG_ModelRender->GetRotation() * rot);

	if (m_selectFlag == false) {

		//決定
		if ((g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) && m_selectMoveFlag == false) {
			m_commandTimer = 0;
			m_selectFlag = true;
		}

		//コマンド移動 上
		if ((g_pad[0]->IsPress(enButtonUp)) && m_selectMoveFlag == false) {
			m_selectMoveFlag = true;
			m_comandUpDown = false;
			m_commandTimer = 0;

			//SE
			prefab::CSoundSource* ss2 = NewGO<prefab::CSoundSource>(0);
			ss2->Init(L"Assets/sound/タイトル画面SE1.wav", SoundType::enSE);
			ss2->SetVolume(0.8f);
			ss2->Play(false);
		}
		//コマンド移動 下
		if ((g_pad[0]->IsPress(enButtonDown)) && m_selectMoveFlag == false) {
			m_selectMoveFlag = true;
			m_comandUpDown = true;
			m_commandTimer = 0;

			//SE
			prefab::CSoundSource* ss3 = NewGO<prefab::CSoundSource>(0);
			ss3->Init(L"Assets/sound/タイトル画面SE1.wav", SoundType::enSE);
			ss3->SetVolume(0.8f);
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
		if (g_pad[0]->IsTrigger(enButtonB) && m_titleCommand == TitleScene::TC_Option && m_option->GetSelectingState() == GameOption::enItem && m_option->isQueuing() == false) {
			m_commandTimer = 0;
			m_selectFlag = false;

			m_option->Close();

			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ -50.0f,0.0f }, 12, 0, true);
			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ 125.0f,0.0f }, 12, 6, true);
			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ -20.0f,0.0f }, 6, 18, true);
		}
	}
}

void TitleScene::CommandMove() {

	if (m_commandTimer == 0) {
		if (m_comandUpDown == false) {
			//上
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({50.0f,-50.0f }, 12, 0,true);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor({1.0f,1.0f,1.0f,0.0f}, 12, 0);
				//
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			case TitleScene::TC_Option:
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,-50.0f }, 12, 0, true);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			case TitleScene::TC_Exit:
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,-50.0f }, 12, 0, true);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			}
		}
		else {
			//下
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,50.0f }, 12, 0, true);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			case TitleScene::TC_Option:
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,50.0f }, 12, 0, true);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			case TitleScene::TC_Exit:
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,50.0f }, 12, 0, true);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			}
		}
	}

	if (m_commandTimer == 12) {
		//上
		if (m_comandUpDown == false) {
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x + 50.0f ,m_bottomPos.y + 50.0f }, 1, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 12, 1);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Exit;
				break;
			case TitleScene::TC_Option:
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x + 50.0f ,m_bottomPos.y + 50.0f }, 1, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 12, 1);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Start;
				break;
			case TitleScene::TC_Exit:
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x + 50.0f ,m_bottomPos.y + 50.0f }, 1, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 12, 1);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Option;
				break;
			}
		}
		else {
			//下
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x + 50.0f ,m_topPos.y - 50.0f }, 1, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 12, 1);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Option;
				break;
			case TitleScene::TC_Option:
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x + 50.0f ,m_topPos.y - 50.0f }, 1, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 12, 1);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Exit;
				break;
			case TitleScene::TC_Exit:
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x + 50.0f ,m_topPos.y - 50.0f }, 1, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 12, 1);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Start;
				break;
			}
		}
	}

	m_commandTimer++;

	if (m_commandTimer >= 30) {
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
			m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,0.0f }, 12, 0, true);
			m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ -125.0f,0.0f }, 12, 6, true);
			m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ 20.0f,0.0f }, 6, 18, true);
			break;
		case TitleScene::TC_Option:
			//設定
			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,0.0f }, 12, 0, true);
			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ -125.0f,0.0f }, 12, 6, true);
			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ 20.0f,0.0f }, 6, 18, true);
			break;
		case TitleScene::TC_Exit:
			//終了
			m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,0.0f }, 12, 0, true);
			m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ -125.0f,0.0f }, 12, 6, true);
			m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ 20.0f,0.0f }, 6, 18, true);
			break;
		}

	}

	m_commandTimer++;


	if (m_commandTimer == 40 && m_titleCommand == TitleScene::TC_Start)
	{
		TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, 50, false);
	}

	if (m_commandTimer == 60) {
		switch (m_titleCommand)
		{
		case TitleScene::TC_Start:
			//開始
			//BGMを消す
			DeleteGO(ssBGM);
			//switch文の中で宣言するためのスコープ
			{
				SampleScene* gameScene = NewGO<SampleScene>(0, "gamescene");
				gameScene->Set1PSensitivity(m_option->GetP1Sensitivity());
				gameScene->Set2PSensitivity(m_option->GetP2Sensitivity());
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
	if (m_commandTimer == 20)
	{
		//SE
		prefab::CSoundSource* ss1 = NewGO<prefab::CSoundSource>(0);
		ss1->Init(L"Assets/sound/ガキーン!.wav", SoundType::enSE);
		ss1->SetVolume(1.2f);
		ss1->Play(false);
	}
}