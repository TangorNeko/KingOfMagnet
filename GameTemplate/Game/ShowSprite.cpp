#include "stdafx.h"
#include "ShowSprite.h"


ShowSprite::~ShowSprite()
{
	DeleteGO(m_spriteRender);
}

bool ShowSprite::Start()
{

	m_spriteRender = NewGO<prefab::CSpriteRender>(0);

	m_spriteRender->Init("Assets/modelData/utc_all2.dds", 128, 128);
	return true;
}

void ShowSprite::Update()
{
	m_spriteRender->SetPosition({ 0.0f,0.0f,0.0f });
}

void ShowSprite::Render(RenderContext& rc, Camera* camera)
{
	/*
	Font font;
	font.Begin(rc);
	font.Draw(L"test", { 0.0f,300.0f }, { 1.0f,1.0f,1.0f,1.0f }, 0.0f, 1.0f, { 0.5f,0.5f });
	font.End(rc);
	*/
}
