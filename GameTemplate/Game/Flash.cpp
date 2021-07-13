//フラッシュグレネードの処理。相手の画面を白くする
#include "stdafx.h"
#include "Flash.h"
#include "Player.h"
#include "GameScene.h"

namespace
{
	const Vector3 CENTER_OF_SCREEN = { 0.0f,0.0f,0.0f };
	const float SOUND_SE_FLASH1_VOLUME = 0.3f;
	const float SOUND_SE_FLASH2_VOLUME = 0.4f;
	const Vector3 EFFECT_FLASH_SCALE = { 25.0f,25.0f,25.0f };
	const float FLASH_AFFECT_ANGLE = 0.2f;
	const int SPRITE_FLASH_WIDTH = 640;
	const int SPRITE_FLASH_HEIGHT = 720;
	const Vector3 SPRITE_COLOR_WHITE = { 1.0f,1.0f,1.0f };
	const float FLASH_DECAY_VALUE = 0.002f;
}

Flash::~Flash()
{
	DeleteGO(m_spriteRender);
	m_effect->Stop();
	DeleteGO(m_effect);
}
bool Flash::Start()
{
	//音を再生
	SoundOneShotPlay(L"Assets/sound/閃光弾.wav", SOUND_SE_FLASH1_VOLUME);
	SoundOneShotPlay(L"Assets/sound/閃光弾2.wav", SOUND_SE_FLASH2_VOLUME);

	//エフェクトを再生
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/閃光.efk");
	m_effect->SetPosition(m_position);
	m_effect->SetScale(EFFECT_FLASH_SCALE);
	m_effect->Play();

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			Vector3 angle = m_position - player->GetPosition();
			angle.Normalize();

			float n = player->GetCameraFront().Dot(angle);

			if (m_parentNum != player->GetPlayerNum() &&
				n > FLASH_AFFECT_ANGLE)
			{
				m_flashFlag = true;
				m_spriteRender = NewGO<prefab::CSpriteRender>(0);
				m_spriteRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(player->GetPlayerNum()));
				m_spriteRender->SetPosition(CENTER_OF_SCREEN);
				m_spriteRender->Init("Assets/Image/White.dds", SPRITE_FLASH_WIDTH, SPRITE_FLASH_HEIGHT);
			}
			return true;
		});

	m_gameScene = FindGO<GameScene>("gamescene");
	return true;	
}
void Flash::Update()
{
	if (m_deleteFlag == false)
	{
		if (m_flashFlag == true)
		{
			//Alphaを減らして徐々に画像を薄くする
			m_spriteRender->SetMulColor({ SPRITE_COLOR_WHITE.x,SPRITE_COLOR_WHITE.y,SPRITE_COLOR_WHITE.z,m_Alpha });
			m_Alpha -= FLASH_DECAY_VALUE;
			if (m_Alpha < 0)
			{
				m_deleteFlag = true;
			}
		}
		else
			m_deleteFlag = true;
	}

	else if (m_deleteFlag == true && m_effect->IsPlay() == false)
	{
		DeleteGO(this);
	}

	if (m_gameScene->GetGameState() == GameScene::enResult)
	{
		DeleteGO(this);
	}
}