#include "stdafx.h"
#include "BackGround.h"
#include "Repulsion.h"

namespace
{
	const Vector3 SPOTLIGHT_NORTH_POSITION = { 0.0f,0.0f,270.0f };
	const Vector3 SPOTLIGHT_SOUTH_POSITION = { 0.0f,0.0f,-270.0f };
	const Vector3 SPOTLIGHT_DIRECTION = { 0.0f,-1.0f,0.0f };
	const Vector3 SPOTLIGHT_COLOR_WHITE = { 1.0f,1.0f,1.0f };
	const float SPOTLIGHT_RANGE = 1000.0f;
	const float SPOTLIGHT_ANGLE = 90.0f;
	const float RESPAWNPOINT_INIT_DISTANCE = 0.0f;
	const int STRCMP_NO_DIFFERENCE = 0;

}

BackGround::~BackGround()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_northSpotLight);
	DeleteGO(m_southSpotLight);
}

bool BackGround::Start()
{
	//�X�e�[�W���쐬�B
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	//�X�e�[�W�͉e�𔭐�������
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->Init("Assets/modelData/stage00.tkm");

	//�X�e�[�W�̃��f���̐ÓI�������f�����쐬
	m_physicsStaticObject.CreateFromModel(m_skinModelRender->GetModel(), m_skinModelRender->GetModel().GetWorldMatrix());

	//�X�e�[�W�̖k�̌����Ƃ炷�X�|�b�g���C�g
	m_northSpotLight = NewGO<prefab::CSpotLight>(0);
	m_northSpotLight->SetPosition(SPOTLIGHT_NORTH_POSITION);
	m_northSpotLight->SetColor(SPOTLIGHT_COLOR_WHITE);
	m_northSpotLight->SetRange(SPOTLIGHT_RANGE);
	m_northSpotLight->SetDirection(SPOTLIGHT_DIRECTION);
	m_northSpotLight->SetAngleDeg(SPOTLIGHT_ANGLE);

	//�X�e�[�W�̓�̌����Ƃ炷�X�|�b�g���C�g
	m_southSpotLight = NewGO<prefab::CSpotLight>(0);
	m_southSpotLight->SetPosition(SPOTLIGHT_SOUTH_POSITION);
	m_southSpotLight->SetColor(SPOTLIGHT_COLOR_WHITE);
	m_southSpotLight->SetRange(SPOTLIGHT_RANGE);
	m_southSpotLight->SetDirection(SPOTLIGHT_DIRECTION);
	m_southSpotLight->SetAngleDeg(SPOTLIGHT_ANGLE);
	
	//���X�|�[���n�_�̓ǂݍ���
	m_level.Init("Assets/modelData/Level_00.tkl", [&](prefab::LevelObjectData& objData) {
		if (strcmp(objData.name, "ResPos") == STRCMP_NO_DIFFERENCE) {//���X�|�[���n�_

			//���X�|�[���n�_�̌��ɒǉ����Ă����B
			m_respawnPoints.push_back({ objData.position.x,0.0f,objData.position.z });
			
			return true;
		}
		return false;
	});
	
	return true;
}

void BackGround::Update()
{
	m_skinModelRender->SetPosition(m_position);
}

bool BackGround::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint)
{
	return m_skinModelRender->isLineHitModel(start, end, crossPoint);
}

Vector3 BackGround::GetRespawnPoint(const Vector3& enemyPos)
{
	//���X�|�[���n�_
	Vector3 respawnpoint;

	//���X�|�[���n�_����󂯎�����G�̍��W�ւ̋���(0�ŏ�����)
	float distance = RESPAWNPOINT_INIT_DISTANCE;

	//���X�|�[���n�_�̌��𑖍�
	for (auto ResPos : m_respawnPoints)
	{
		//���X�|�[���n�_�̌�₩��G�̍��W�ւ̃x�N�g�����擾
		Vector3 diff = ResPos - enemyPos;

		//���X�|�[���n�_�̌�₩��G�̍��W�ւ̃x�N�g���̒������A�����_�ł̒�����蒷���Ȃ�
		if (diff.Length() > distance)
		{
			//���X�|�[���n�_���X�V
			respawnpoint = ResPos;

			//���X�|�[���n�_�ւ̋������X�V
			distance = diff.Length();
		}
	}

	return respawnpoint;
}