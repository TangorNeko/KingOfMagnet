#include "stdafx.h"
#include "ShowSprite.h"


ShowSprite::~ShowSprite()
{
	//DeleteGO(m_spriteRender);
	DeleteGO(m_fontRender);
}

bool ShowSprite::Start()
{

	//m_spriteRender = NewGO<prefab::CSpriteRender>(0);

	//m_spriteRender->Init("Assets/modelData/utc_all2.dds", 128, 128);
	//m_spriteRender->SetDrawScreen(prefab::CSpriteRender::Screen1);

	m_fontRender = NewGO<prefab::CFontRender>(1);
	m_fontRender->SetText(L"test");
	return true;
}

void ShowSprite::Update()
{
	//m_spriteRender->SetPosition({ 0.0f,0.0f,0.0f });
}
