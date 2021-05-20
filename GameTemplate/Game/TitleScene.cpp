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
	//�����\��
	/*m_title_fontRender = NewGO<prefab::CFontRender>(0);
	m_title_fontRender->SetPosition({ -200.0f, 200.0f });
	m_title_fontRender->SetText(L"���E�V��");
	m_title_fontRender->SetScale({ 2.0f,2.0f });*/

	m_option_fontRender = NewGO<prefab::CFontRender>(0);	
	m_option_fontRender->SetPosition({ m_optionX, m_optionY });//��
	m_option_fontRender->SetText(L"OPTION");

	m_start_fontRender = NewGO<prefab::CFontRender>(1);
	m_start_fontRender->SetPosition({ m_startX, m_startY });//��
	m_start_fontRender->SetText(L"START");

	m_exit_fontRender = NewGO<prefab::CFontRender>(0);	
	m_exit_fontRender->SetPosition({ m_exitX, m_exitY });//��
	m_exit_fontRender->SetText(L"EXIT");

	m_dot_fontRender = NewGO<prefab::CFontRender>(0);
	m_dot_fontRender->SetPosition({ 500.0f, m_startY });//��
	m_dot_fontRender->SetText(L"�E");

	//�^�C�g�����S
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
			m_menuselect = 2;//���܂ōs���Əォ��ɂȂ�
		}
	}
	if (g_pad[0]->IsTrigger(enButtonDown) || g_pad[1]->IsTrigger(enButtonUp)) {
		m_menuselect++;
		
		if (m_menuselect >= 3) {
			m_menuselect = 0;//��܂ōs���Ɖ�����ɂȂ�
		}
	}
	m_optionX = 400.0f;
	m_startX = 400.0f;
	m_exitX = 400.0f;
	//�I�����Ă��镔�����ԂɂȂ�
	switch (m_menuselect)
	{
	case 0://�オ��
		m_option_fontRender->SetColor({ red });
		m_start_fontRender->SetColor({ blue });
		m_exit_fontRender->SetColor({ blue });	
		m_optionX -= 50.0f;
		m_dot_fontRender->SetPosition({ 500.0f, m_optionY });
		break; 
	case 1://������
		m_option_fontRender->SetColor({ blue });
		m_start_fontRender->SetColor({ red });
		m_exit_fontRender->SetColor({ blue });
		m_startX -= 50.0f;
		m_dot_fontRender->SetPosition({ 500.0f, m_startY });
		break;
	case 2://������
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
	//�X�^�[�g��A�{�^���������ƃL�����I����ʂɑJ�ڂ���
	if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {
		if (m_menuselect == 1) {
			//NewGO<CharacterSelect>(0, "characterselect");
			SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
			DeleteGO(this);
		}
	}
}