#include "stdafx.h"
#include "UsedGravityGrenade.h"
#include "ShowModel.h"
#include "Character_base.h"
#include "BackGround.h"

UsedGravityGrenade::~UsedGravityGrenade()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_pointLight);
}

bool UsedGravityGrenade::Start()
{
	//���͒e���g�̃��f�����쐬

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Gravity.tkm");
	m_skinModelRender->SetScale({ 0.5f, 0.5f, 0.5f });

	//���͒e����o���
	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 1.0f,1.0f,0.0f });
	m_pointLight->SetRange(200.0f);
	//�����������Ɉ��͒e��������
	m_direction_me = m_moveDirection;
	float t = m_direction_me.Dot(Vector3::AxisZ);

	t = acos(t);
	if (m_direction_me.x < 0) {
		t *= -1;
	}

	m_rot.SetRotation(Vector3::AxisY, t);
	m_skinModelRender->SetRotation(m_rot);

	m_stageModel = FindGO<BackGround>("background");

	return true;
}

void UsedGravityGrenade::Update()
{
	//�O�t���[���̒e�̈ʒu���L�^�B
	Vector3 oldPos = m_position;
	
	if (hitFlag == false) {
		//���͒e�̈ړ�����
		m_position += m_moveDirection * m_velocity;
		m_moveDirection.y -= 1.5f * 1.5f * 0.05f;

		//�v���C���[�Ƃ̏Փ˔���p�̃J�v�Z���̈ʒu���X�V�B
		m_collider.SetStartPoint(oldPos);
		m_collider.SetEndPoint(m_position);
		m_collider.SetRadius(15.0f);

		hitFlag = m_stageModel->isLineHitModel(oldPos, m_position, crossPoint);
	}

	else {
		m_position = crossPoint;

	}
	

	////�e�v���C���[������
	//QueryGOs<Character_base>("Player", [this, oldPos](Character_base* player)->bool
	//	{
	//		//���˂���Ă���15�t���[����ɁA���˂����v���C���[�̎��͂�^����(����or����)
	//		if (m_liveCount == 15 && player->m_playerNum == m_parentNo)
	//		{
	//			m_velocity += 5.0f * player->m_magPower;
	//		}

	//		//�e�𔭎˂����v���C���[�ƈႤ�ꍇ(�G�̏ꍇ)
	//		if (player->m_playerNum != m_parentNo)
	//		{
	//			//�G�Ƃ̋���
	//			Vector3 diff = player->m_magPosition - m_position;

	//			//�O�t���[���̈ʒu�ƈړ���̈ʒu�����񂾐����G�v���C���[�̓����蔻��̎O�p�`��ʂ��Ă���ꍇ
	//			if (player->m_collider.isHitCapsule(m_collider))
	//			{
	//				//�G�v���C���[�ɑ��x�ɉ����ă_���[�W��^����
	//				player->Damage(m_velocity / 2);
	//				DeleteGO(this);
	//			}

	//			//�G�Ƃ̋�����500�ȓ��Ȃ�
	//			if (diff.Length() < 500.0f)
	//			{

	//				//�G�����̓��[�h�Ȃ班�������񂹂�B
	//				if (player->m_magPower < 0)
	//				{
	//					Vector3 toPlayer = diff;
	//					toPlayer.Normalize();
	//					//m_position += toPlayer * player->m_magPower * 3 * -1;
	//					Vector3 newDirection = m_moveDirection * m_velocity + toPlayer * player->m_magPower * 2 * -1;
	//					newDirection.Normalize();
	//					m_moveDirection = newDirection;

	//				}

	//				//���˂����v���C���[�̎��͂�^����(����or����)
	//				if (m_isAffectedFromEnemyPower == false)
	//				{
	//					m_velocity -= player->m_magPower;
	//					m_isAffectedFromEnemyPower = true;
	//				}
	//			}
	//		}

	//		return true;
	//	}
	//);

	//���f���ƃ��C�g�̈ʒu���Z�b�g�B
	m_skinModelRender->SetPosition(m_position);
	m_pointLight->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rot);
	//100�t���[���������������
	m_liveCount++;
	if (m_liveCount > 100)
	{
		DeleteGO(this);
	}

}