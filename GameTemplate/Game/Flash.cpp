//フラッシュグレネードをとったとき相手の画面を白くする
#include "stdafx.h"
#include "Flash.h"
Flash::~Flash()
{
	DeleteGO(m_spriteRender);
}
bool Flash::Start()
{
	m_spriteRender = NewGO<prefab::CSpriteRender>(3);
	m_spriteRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_affectPlayerNum));
	m_position_sprite = { 0,0,0 };//画面の中心
	m_spriteRender->SetPosition(m_position_sprite);
	m_spriteRender->Init("Assets/Image/White.dds", 640, 720);
	return true;
}
void Flash::Update()
{
	//Alphaを減らして徐々に画像を薄くする
	m_spriteRender->SetMulColor({ 1.0f,1.0f,1.0f,m_Alpha });
	m_Alpha -= 0.002;
	if (m_Alpha < 0)
	{
		DeleteGO(this);
	}
}