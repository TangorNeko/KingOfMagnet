//�t���b�V���O���l�[�h�̏����B����̉�ʂ𔒂�����
#include "stdafx.h"
#include "Flash.h"
#include "Player.h"

Flash::~Flash()
{
	DeleteGO(m_spriteRender);
}
bool Flash::Start()
{
	//�����Đ�
	prefab::CSoundSource* ss1 = NewGO<prefab::CSoundSource>(0);;
	ss1->Init(L"Assets/sound/�M���e.wav");
	ss1->SetVolume(0.5f);
	ss1->Play(false);
	prefab::CSoundSource* ss2 = NewGO<prefab::CSoundSource>(0);;
	ss2->Init(L"Assets/sound/�M���e2.wav");
	ss2->Play(false);

	//�G�t�F�N�g���Đ�
	prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
	effect->Init(u"Assets/effect/�M��.efk");
	effect->SetPosition(m_position);
	effect->SetScale({ 25.0f, 25.0f, 25.0f });
	effect->Play();


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
				m_position_sprite = { 0,0,0 };//��ʂ̒��S
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
		//Alpha�����炵�ď��X�ɉ摜�𔖂�����
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