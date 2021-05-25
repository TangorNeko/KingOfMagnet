#include "stdafx.h"
#include "TitleScene.h"
#include "CharacterSelect.h"
#include "SampleScene.h"
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
}
bool TitleScene::Start()
{	
	//背景のモデル
	m_BG_ModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_BG_ModelRender->Init("Assets/modelData/TitleCylinder.tkm");
	m_BG_ModelRender->SetPosition({ 0.0f,-300.0f,-75.0f });

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
	return true;
}
void TitleScene::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {
		SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
		DeleteGO(this);
	}
}