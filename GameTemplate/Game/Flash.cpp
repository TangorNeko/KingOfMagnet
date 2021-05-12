//フラッシュグレネードの処理。相手の画面を白くする
#include "stdafx.h"
#include "Flash.h"
#include "Player.h"

Flash::~Flash()
{
	DeleteGO(m_spriteRender);
}
bool Flash::Start()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			Vector3 angle = m_position - player->m_position;
			angle.Normalize();

			float n = player->front.Dot(angle);

			if (m_parentNum != player->m_playerNum &&
				n > 0.2)
			{
				m_flashFlag = true;
				m_spriteRender = NewGO<prefab::CSpriteRender>(0);
				m_spriteRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(player->m_playerNum));
				m_position_sprite = { 0,0,0 };//画面の中心
				m_spriteRender->SetPosition(m_position_sprite);
				m_spriteRender->Init("Assets/Image/White.dds", 640, 720);
			}
			return true;
		});
	return true;	
}
void Flash::Update()
{
	if (m_flashFlag == true) 
	{
		//Alphaを減らして徐々に画像を薄くする
		m_spriteRender->SetMulColor({ 1.0f,1.0f,1.0f,m_Alpha });
		m_Alpha -= 0.002;
		if (m_Alpha < 0)
		{
			DeleteGO(this);
		}
	}
	else 
		DeleteGO(this);
}