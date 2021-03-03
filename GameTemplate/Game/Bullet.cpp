#include "stdafx.h"
#include "Bullet.h"
#include "ShowModel.h"


Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_pointLight);
}

bool Bullet::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/Bullet.tkm");
	m_skinModelRender->SetScale({ 2.0f, 2.0f, 2.0f });

	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 1.0f,1.0f,0.0f });
	m_pointLight->SetRange(200.0f);

	return true;
}

void Bullet::Update()
{
	if (m_liveCount == 15)
	{
		QueryGOs<ShowModel>("Player", [this](ShowModel* player)->bool
			{
				if (player->m_playerNum == m_parentNo)
				{
					m_velocity += 5.0f * player->m_magPower;
				}
				return true;
			}
		);
	}
	/*
	QueryGOs<ShowModel>("Player", [this](ShowModel* player)->bool
		{
			Vector3 diff = m_position - player->m_magPosition;
			if (diff.Length() < 1000.0f)
			{
				diff.Normalize();
				m_moveSpeed.x += diff.x * player->m_magPower * 0.1f;// *0.05;
				m_moveSpeed.z += diff.z * player->m_magPower * 0.1f;// *0.05;
			}
			return true;
		}
	);
	*/

	m_position += m_moveDirection * m_velocity;
	//m_position += m_moveSpeed;

	m_skinModelRender->SetPosition(m_position);
	m_pointLight->SetPosition(m_position);

	m_liveCount++;
	if (m_liveCount > 100)
	{
		DeleteGO(this);
	}
}