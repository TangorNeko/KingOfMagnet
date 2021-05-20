#include "stdafx.h"
#include "ResultScene.h"
#include "SampleScene.h"
#include "TitleScene.h"
ResultScene::~ResultScene()
{
	DeleteGO(m_title_fontRender);
	DeleteGO(m_replay_fontRender);
	DeleteGO(m_detail_fontRender);
}
bool ResultScene::Start()
{
	m_title_fontRender = NewGO<prefab::CFontRender>(1);
	m_title_fontRender->SetPosition({ m_titleX, m_titleY });//中
	m_title_fontRender->SetText(L"タイトルへ");

	m_replay_fontRender = NewGO<prefab::CFontRender>(1);
	m_replay_fontRender->SetPosition({ m_replayX, m_replayY });//中
	m_replay_fontRender->SetText(L"もう一度");

	m_detail_fontRender = NewGO<prefab::CFontRender>(1);
	m_detail_fontRender->SetPosition({ m_detailX, m_detailY });//中
	m_detail_fontRender->SetText(L"詳細表示");

	return true;
}
void ResultScene::Update()
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
	m_titleX = 0.0f;
	m_replayX = 0.0f;
	m_detailX = 0.0f;
	//選択している部分が赤になる
	switch (m_menuselect)
	{
	case 0://上が赤
		m_title_fontRender->SetColor({ red });
		m_replay_fontRender->SetColor({ blue });
		m_detail_fontRender->SetColor({ blue });
		m_titleX -= 50.0f;
		break;
	case 1://中が赤
		m_title_fontRender->SetColor({ blue });
		m_replay_fontRender->SetColor({ red });
		m_detail_fontRender->SetColor({ blue });
		m_replayX -= 50.0f;
		break;
	case 2://下が赤
		m_title_fontRender->SetColor({ blue });
		m_replay_fontRender->SetColor({ blue });
		m_detail_fontRender->SetColor({ red });
		m_detailX -= 50.0f;
		break;
	default:
		break;
	}
	m_title_fontRender->SetPosition({ m_titleX,m_titleY });
	m_replay_fontRender->SetPosition({ m_replayX,m_replayY });
	m_detail_fontRender->SetPosition({ m_detailX,m_detailY });
	//スタートでAボタンを押すとキャラ選択画面に遷移する
	if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {
		if (m_menuselect == 1) {
			SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
			DeleteGO(this);
		}
		if (m_menuselect == 0) {
			TitleScene* titlescene = NewGO<TitleScene>(0, "titlescene");
			DeleteGO(this);
		}
	}
}