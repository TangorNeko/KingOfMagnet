//�t���b�V���O���l�[�h�̏����B����̉�ʂ𔒂�����
#include "stdafx.h"
#include "Flash.h"
#include "Player.h"
#include "SampleScene.h"

Flash::~Flash()
{
	DeleteGO(m_spriteRender);
	m_effect->Stop();
	DeleteGO(m_effect);
}
bool Flash::Start()
{
	//�����Đ�
	prefab::CSoundSource* ss1 = NewGO<prefab::CSoundSource>(0);;
	ss1->Init(L"Assets/sound/�M���e.wav", SoundType::enSE);
	ss1->SetVolume(0.3f);
	ss1->Play(false);
	prefab::CSoundSource* ss2 = NewGO<prefab::CSoundSource>(0);;
	ss2->Init(L"Assets/sound/�M���e2.wav", SoundType::enSE);
	ss2->SetVolume(0.4f);
	ss2->Play(false);

	//�G�t�F�N�g���Đ�
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/�M��.efk");
	m_effect->SetPosition(m_position);
	m_effect->SetScale({ 25.0f, 25.0f, 25.0f });
	m_effect->Play();

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			Vector3 angle = m_position - player->m_position;
			angle.Normalize();

			float n = player->m_front.Dot(angle);

			if (m_parentNum != player->m_playerNum &&
				n > 0.2f)
			{
				m_flashFlag = true;
				m_spriteRender = NewGO<prefab::CSpriteRender>(0);
				m_spriteRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(player->m_playerNum));
				m_position_sprite = { 0,0,0 };//��ʂ̒��S
				m_spriteRender->SetPosition(m_position_sprite);
				m_spriteRender->Init("Assets/Image/White.dds", 640, 720);
			}
			return true;
		});

	m_gameScene = FindGO<SampleScene>("gamescene");
	return true;	
}
void Flash::Update()
{
	if (m_deleteFlag == false)
	{
		if (m_flashFlag == true)
		{
			//Alpha�����炵�ď��X�ɉ摜�𔖂�����
			m_spriteRender->SetMulColor({ 1.0f,1.0f,1.0f,m_Alpha });
			m_Alpha -= 0.002;
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

	if (m_gameScene->GetGameState() == SampleScene::enResult)
	{
		DeleteGO(this);
	}
}