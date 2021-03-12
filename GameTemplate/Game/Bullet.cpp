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
	Vector3 oldPos = m_position;

	m_position += m_moveDirection * m_velocity;

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

				if (diff.Length() < 500.0f)
				{
					if (player->m_magPower < 0)
					{
						Vector3 toPlayer = diff;
						toPlayer.Normalize();
						m_position += toPlayer * player->m_magPower * 3 * -1;
					}


					if (m_isAffectedFromEnemyPower == false)
					{
						//m_velocity -= player->m_magPower;
						m_isAffectedFromEnemyPower = true;
					}
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