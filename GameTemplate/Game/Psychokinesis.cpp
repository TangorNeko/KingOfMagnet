#include "stdafx.h"
#include "Psychokinesis.h"
#include "Mage.h"
#include "ShowModel.h"
#include "Character_base.h"
Psychokinesis::~Psychokinesis()
{
	DeleteGO(m_skinModelRender[0]);
	DeleteGO(m_skinModelRender[1]);
	if (m_level2)
	{
		DeleteGO(m_skinModelRender[2]);
		DeleteGO(m_skinModelRender[3]);
	}
	if (m_level3)
	{
		DeleteGO(m_skinModelRender[4]);
		DeleteGO(m_skinModelRender[5]);
	}
	DeleteGO(m_pointLight);
}
bool Psychokinesis::Start()
{	
	for (int i = 0; i < 2; i++)
	{
		m_skinModelRender[i] = NewGO<prefab::CSkinModelRender>(0);
		m_skinModelRender[i]->Init("Assets/modelData/MageBullet.tkm");
	}
	for (int i = 0; i < 6; i++)
	{
		m_rot.Apply(toPos[i]);
	}
	//�e����o���
	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 1.0f,1.0f,0.0f });
	m_pointLight->SetRange(200.0f);
	return true;
}
void Psychokinesis::Update()
{
	
	if (g_pad[m_playerNum]->IsPress(enButtonX) && m_charge > 30)
	{
		m_shot_on = true;
	}
	if (m_shot_on)
	{		
		//�O�t���[���̒e�̈ʒu���L�^�B
		for(int i=0;i<6;i++)
		{
			oldPos[i] = m_position[i];
		}		
		//�e�̈ړ�����
		//�v���C���[�Ƃ̏Փ˔���p�̃J�v�Z���̈ʒu���X�V�B
		for (int i = 0; i < 2; i++)
		{
			m_position[i] += m_moveDirection * m_velocity;
			m_collider[i].SetStartPoint(oldPos[i]);
			m_collider[i].SetEndPoint(m_position[i]);
			m_collider[i].SetRadius(30.0f);
		}
		if (m_level2)
		{
			for (int i = 2; i < 4; i++)
			{
				m_position[i] += m_moveDirection * m_velocity;
				m_collider[i].SetStartPoint(oldPos[i]);
				m_collider[i].SetEndPoint(m_position[i]);
				m_collider[i].SetRadius(30.0f);
			}
		}
		if (m_level3)
		{
			for (int i = 4; i < 6; i++)
			{
				m_position[i] += m_moveDirection * m_velocity;
				m_collider[i].SetStartPoint(oldPos[i]);
				m_collider[i].SetEndPoint(m_position[i]);
				m_collider[i].SetRadius(30.0f);
			}
		}
		
		
		for (int i = 0; i < 6; i++)
		{
			
		}


		//�e�v���C���[������
		QueryGOs<Character_base>("Player", [this](Character_base* player)->bool
			{
				//���˂���Ă���15�t���[����ɁA���˂����v���C���[�̎��͂�^����(����or����)
				if (m_liveCount == 15 && player->m_playerNum == m_parentNo)
				{
					m_velocity += 5.0f * player->m_magPower;
				}

				//�e�𔭎˂����v���C���[�ƈႤ�ꍇ(�G�̏ꍇ)
				if (player->m_playerNum != m_parentNo)
				{
					//�G�Ƃ̋���
					Vector3 diff[6];
					for (int i = 0; i < 6; i++)
					{
						diff[i] = player->m_magPosition - m_position[i];
						//�O�t���[���̈ʒu�ƈړ���̈ʒu�����񂾐����G�v���C���[�̓����蔻��̎O�p�`��ʂ��Ă���ꍇ
						if (player->m_collider.isHitCapsule(m_collider[i]))
						{
							//�G�v���C���[�ɑ��x�ɉ����ă_���[�W��^����
							player->Damage((m_velocity * m_chargelevel)/5);
							DeleteGO(this);
						}
						//�G�Ƃ̋�����500�ȓ��Ȃ�
						if (diff[i].Length() < 500.0f)
						{
							//�G�����̓��[�h�Ȃ班�������񂹂�B
							if (player->m_magPower < 0)
							{
								Vector3 toPlayer = diff[i];
								toPlayer.Normalize();
								//m_position += toPlayer * player->m_magPower * 4 * -1;
								Vector3 newDirection = m_moveDirection * m_velocity + toPlayer * player->m_magPower * 2 * -1;
								newDirection.Normalize();
								m_moveDirection = newDirection;
							}

							//���˂����v���C���[�̎��͂�^����(����or����)
							if (m_isAffectedFromEnemyPower == false)
							{
								m_velocity -= player->m_magPower;
								m_isAffectedFromEnemyPower = true;
							}
						}
						
					}
					

					
				}

				return true;
			}
		);

		//���C�g�̈ʒu���Z�b�g�B
		m_pointLight->SetPosition(m_position[0]);
		for (int i = 0; i < 2; i++)
		{
			m_skinModelRender[i]->SetPosition(m_position[i]);		
		}
		if (m_level2)
		{
			for (int i = 2; i < 4; i++)
			{
				m_skinModelRender[i]->SetPosition(m_position[i]);
			}
		}
		if (m_level3)
		{
			for (int i = 4; i < 6; i++)
			{
				m_skinModelRender[i]->SetPosition(m_position[i]);				
			}
		}
		//100�t���[���������������
		m_liveCount++;
		if (m_liveCount > 100)
		{
			DeleteGO(this);
		}
	}
	else
	{
		if (m_level2)
		{
			if (m_isOnce2)
			{
				for (int i = 2; i < 4; i++)
				{
					m_skinModelRender[i] = NewGO<prefab::CSkinModelRender>(0);
					m_skinModelRender[i]->Init("Assets/modelData/MageBullet.tkm");
				}
				m_isOnce2 = false;
			}
		}
		if (m_level3)
		{
			if (m_isOnce3)
			{
				for (int i = 4; i < 6; i++)
				{
					m_skinModelRender[i] = NewGO<prefab::CSkinModelRender>(0);
					m_skinModelRender[i]->Init("Assets/modelData/MageBullet.tkm");
				}
				m_isOnce3 = false;
			}
		}



		//Y�����ɉ�]����N�H�[�^�j�I��������B
		qRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * 1.5f);

		//�N�H�[�^�j�I�����g����toPos���񂷁B
		for (int i = 0; i < 6; i++)
		{
			qRotY.Apply(toPos[i]);

			m_position[i] = toPos[i] + m_playerpos;			
		}
		if (m_Up_On == true)
		{
			if (m_UpDown > 25)
			{
				m_Up_On = false;
			}
			m_UpDown += 0.5f;
		}
		else
		{
			if (m_UpDown < 0)
			{
				m_Up_On = true;
			}
			m_UpDown -= 0.5;
		}
		for (int i = 0; i < 6; i++)
		{
			m_position[i].y += m_UpDown;			
		}
		for (int i = 0; i < 2; i++)
		{
			m_skinModelRender[i]->SetPosition(m_position[i]);			
			m_skinModelRender[i]->SetRotation(m_rot);			
		}
		if (m_level2)
		{
			for (int i = 2; i < 4; i++)
			{
				m_skinModelRender[i]->SetPosition(m_position[i]);
				m_skinModelRender[i]->SetRotation(m_rot);
			}
		}
		if (m_level3)
		{
			for (int i = 4; i < 6; i++)
			{
				m_skinModelRender[i]->SetPosition(m_position[i]);
				m_skinModelRender[i]->SetRotation(m_rot);
			}
		}
		//���C�g�̈ʒu���Z�b�g�B
		m_pointLight->SetPosition(m_position[0]);
	}
}
