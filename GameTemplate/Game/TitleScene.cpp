#include "stdafx.h"
#include "TitleScene.h"
#include "CharacterSelect.h"
#include "SampleScene.h"
TitleScene::~TitleScene() 
{
	DeleteGO(m_title_fontRender);
	DeleteGO(m_start_fontRender);
	DeleteGO(m_description_fontRender);
	DeleteGO(m_option_fontRender);
}
bool TitleScene::Start() 
{
	//文字表示
	m_title_fontRender = NewGO<prefab::CFontRender>(0);
	m_title_fontRender->SetPosition({ -200.0f, 200.0f });
	m_title_fontRender->SetText(L"磁界之王");
	m_title_fontRender->SetScale({ 2.0f,2.0f });

	m_start_fontRender = NewGO<prefab::CFontRender>(0);
	m_start_fontRender->SetPosition({ -100.0f, 0.0f });//上
	m_start_fontRender->SetText(L"スタート");

	m_description_fontRender = NewGO<prefab::CFontRender>(0);	
	m_description_fontRender->SetPosition({ -100.0f, -100.0f });//真ん中
	m_description_fontRender->SetText(L"操作説明");

	m_option_fontRender = NewGO<prefab::CFontRender>(0);	
	m_option_fontRender->SetPosition({ -100.0f, -200.0f });//下
	m_option_fontRender->SetText(L"オプション");
	return true;
}
void TitleScene::Update()
{
	
	if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[1]->IsTrigger(enButtonDown)) {
		m_menuselect++;
		if (m_menuselect >= 3) {
			m_menuselect = 0;//上まで行くと下からになる
		}
	}
	if (g_pad[0]->IsTrigger(enButtonDown) || g_pad[1]->IsTrigger(enButtonUp)) {
		m_menuselect--;
		if (m_menuselect <= -1) {
			m_menuselect = 2;//下まで行くと上からになる
		}
	}
	//選択している部分が赤になる
	switch (m_menuselect)
	{
	case 0://上が赤
		m_start_fontRender->SetColor({ red });
		m_option_fontRender->SetColor({ blue });
		m_description_fontRender->SetColor({ blue });
		break; 
	case 1://中が赤
		m_start_fontRender->SetColor({ blue });
		m_option_fontRender->SetColor({ red });
		m_description_fontRender->SetColor({ blue });
		break;
	case 2://下が赤
		m_start_fontRender->SetColor({ blue });
		m_option_fontRender->SetColor({ blue });
		m_description_fontRender->SetColor({ red });
		break;
	default:
		break;
	}
	//スタートでAボタンを押すとキャラ選択画面に遷移する
	if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {
		if (m_menuselect == 0) {
			//NewGO<CharacterSelect>(0, "characterselect");
			SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
			DeleteGO(this);
		}
	}
}