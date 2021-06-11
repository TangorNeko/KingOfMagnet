#include "stdafx.h"
#include "BackGround.h"
#include "Repulsion.h"

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
	m_northSpotLight->SetPosition({ 0.0f,0.0f,270.0f });
	m_northSpotLight->SetColor({ 1.0f,1.0f,1.0f });
	m_northSpotLight->SetRange(1000);
	m_northSpotLight->SetDirection({ 0.0f,-1.0f,0.0f });
	m_northSpotLight->SetAngleDeg(90.0f);

	//�X�e�[�W�̓�̌����Ƃ炷�X�|�b�g���C�g
	m_southSpotLight = NewGO<prefab::CSpotLight>(0);
	m_southSpotLight->SetPosition({ 0.0f,0.0f,-270.0f });
	m_southSpotLight->SetColor({ 1.0f,1.0f,1.0f });
	m_southSpotLight->SetRange(1000);
	m_southSpotLight->SetDirection({ 0.0f,-1.0f,0.0f });
	m_southSpotLight->SetAngleDeg(90.0f);
	
	//���X�|�[���n�_�̓ǂݍ���
	m_level.Init("Assets/modelData/Level_00.tkl", [&](prefab::LevelObjectData& objData) {
		if (strcmp(objData.name, "ResPos") == 0) {//���X�|�[���n�_

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
	float distance = 0;

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