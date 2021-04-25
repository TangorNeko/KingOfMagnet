//���͑J�ڂ�����������A�C�e����\��������
#include "stdafx.h"
#include "MagAcceleration_item.h"
MagAcceleration_item::~MagAcceleration_item()
{
	DeleteGO(m_skinModelRender);
}
bool MagAcceleration_item::Start()
{
	//���f�����쐬
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/SandClockBlue.tkm");
	//m_skinModelRender->SetScale({ 5.0f, 5.0f, 5.0f });
	m_collider.SetStartPoint(m_position);
	topPos = m_position;
	topPos.y += 50;
	m_collider.SetEndPoint(topPos);
	m_collider.SetRadius(15.0f);
	//m_skinModelRender->SetPosition(m_position);

	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}
void MagAcceleration_item::Update()
{
	/*
	if (m_gameScene->GetGameEndFlag() == true)
	{
		DeleteGO(this);
	}
	*/

	//�e�v���C���[������
	QueryGOs<Character_base>("Player", [this](Character_base* player)->bool
		{
			//�v���C���[���߂����
			if (player->m_collider.isHitCapsule(m_collider))
			{
				player->m_timerAccele = 2;
				player->m_AcceleLoop = 0;
				DeleteGO(this);
			}
			return true;
		});
	ItemMotion();
}