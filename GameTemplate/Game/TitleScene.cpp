#include "stdafx.h"
#include "TitleScene.h"
#include "CharacterSelect.h"
#include "SampleScene.h"
TitleScene::~TitleScene() 
{
	DeleteGO(m_title_fontRender);
	DeleteGO(m_start_fontRender);
	DeleteGO(m_exit_fontRender);
	DeleteGO(m_option_fontRender);
	DeleteGO(m_titleSpriteRender);
}
bool TitleScene::Start()
{
	//文字表示
	/*m_title_fontRender = NewGO<prefab::CFontRender>(0);
	m_title_fontRender->SetPosition({ -200.0f, 200.0f });
	m_title_fontRender->SetText(L"磁界之王");
	m_title_fontRender->SetScale({ 2.0f,2.0f });*/

	m_option_fontRender = NewGO<prefab::CFontRender>(0);	
	m_option_fontRender->SetPosition({ m_optionX, m_optionY });//上
	m_option_fontRender->SetText(L"OPTION");

	m_start_fontRender = NewGO<prefab::CFontRender>(1);
	m_start_fontRender->SetPosition({ m_startX, m_startY });//中
	m_start_fontRender->SetText(L"START");

	m_exit_fontRender = NewGO<prefab::CFontRender>(0);	
	m_exit_fontRender->SetPosition({ m_exitX, m_exitY });//下
	m_exit_fontRender->SetText(L"EXIT");

	m_dot_fontRender = NewGO<prefab::CFontRender>(0);
	m_dot_fontRender->SetPosition({ 500.0f, m_startY });//下
	m_dot_fontRender->SetText(L"・");

	//タイトルロゴ
	m_titleSpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_titleSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_titleSpriteRender->SetPosition({ -200.0f,0.0f,0.0f });
	m_titleSpriteRender->SetScale({ 0.7f,0.7f,1.0 });
	m_titleSpriteRender->Init("Assets/Image/Zikai_Rogo.dds", 960, 720);

	return true;
}
void TitleScene::Update()
{
	
	if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[1]->IsTrigger(enButtonDown)) {
		m_menuselect--;
		if (m_menuselect <= -1) {
			m_menuselect = 2;//下まで行くと上からになる
		}
	}
	if (g_pad[0]->IsTrigger(enButtonDown) || g_pad[1]->IsTrigger(enButtonUp)) {
		m_menuselect++;
		
		if (m_menuselect >= 3) {
			m_menuselect = 0;//上まで行くと下からになる
		}
	}
	m_optionX = 400.0f;
	m_startX = 400.0f;
	m_exitX = 400.0f;
	//選択している部分が赤になる
	switch (m_menuselect)
	{
	case 0://上が赤
		m_option_fontRender->SetColor({ red });
		m_start_fontRender->SetColor({ blue });
		m_exit_fontRender->SetColor({ blue });	
		m_optionX -= 50.0f;
		m_dot_fontRender->SetPosition({ 500.0f, m_optionY });
		break; 
	case 1://中が赤
		m_option_fontRender->SetColor({ blue });
		m_start_fontRender->SetColor({ red });
		m_exit_fontRender->SetColor({ blue });
		m_startX -= 50.0f;
		m_dot_fontRender->SetPosition({ 500.0f, m_startY });
		break;
	case 2://下が赤
		m_option_fontRender->SetColor({ blue });
		m_start_fontRender->SetColor({ blue });
		m_exit_fontRender->SetColor({ red });
		m_exitX -= 50.0f;
		m_dot_fontRender->SetPosition({ 500.0f, m_exitY });
		break;
	default:
		break;
	}
	m_option_fontRender->SetPosition({ m_optionX,m_optionY });
	m_start_fontRender->SetPosition({ m_startX,m_startY });
	m_exit_fontRender->SetPosition({ m_exitX,m_exitY });
	//スタートでAボタンを押すとキャラ選択画面に遷移する
	if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {
		if (m_menuselect == 1) {
			//NewGO<CharacterSelect>(0, "characterselect");
			SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
			DeleteGO(this);
		}
	}
}