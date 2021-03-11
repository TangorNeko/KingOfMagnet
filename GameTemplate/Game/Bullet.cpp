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
	m_skinModelRender->Init("Assets/modelData/syuriken1.tkm");
	m_skinModelRender->SetScale({ 7.0f, 7.0f, 7.0f });

	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 1.0f,1.0f,0.0f });
	m_pointLight->SetRange(200.0f);

	return true;
}

void Bullet::Update()
{


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
	Vector3 oldPos = m_position;

	m_position += m_moveDirection * m_velocity;
	//m_position += m_moveSpeed;

	QueryGOs<ShowModel>("Player", [this,oldPos](ShowModel* player)->bool
		{
			if (m_liveCount == 15 && player->m_playerNum == m_parentNo)
			{
				m_velocity += 5.0f * player->m_magPower;
			}

			Vector3 diff = player->m_magPosition - m_position;

			if (player->m_playerNum != m_parentNo)
			{
				if (player->m_collider.isHit(oldPos, m_position))
				{
					player->Damage(m_velocity * 3);
					DeleteGO(this);
				}

				if (player->m_magPower < 0 && diff.Length() < 300.0f)
				{
					diff.Normalize();
					m_position += diff * player->m_magPower * 3 * -1;
				}
			}

			return true;
		}
	);

	m_skinModelRender->SetPosition(m_position);
	m_pointLight->SetPosition(m_position);

	m_liveCount++;
	if (m_liveCount > 100)
	{
		DeleteGO(this);
	}
}