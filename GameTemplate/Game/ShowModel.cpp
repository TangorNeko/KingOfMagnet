#include "stdafx.h"
#include "ShowModel.h"

ShowModel::~ShowModel()
{
	DeleteGO(m_skinModelRender);
}

bool ShowModel::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/unityChan.tkm", "Assets/modelData/unityChan.tks");

	return true;
}

void ShowModel::Update()
{
	//m_position.z += 0.1f;

	if (g_pad[0]->IsPress(enButtonB))
	{
		m_deg += 1.0f;
	}

	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, m_deg);

	m_skinModelRender->SetRotation(qRot);
	m_skinModelRender->SetPosition(m_position);

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		prefab::CDirectionLight* yellowfromside = NewGO<prefab::CDirectionLight>(0);
		yellowfromside->SetDirection({ -1.0f,0.0f,0.0f });
		yellowfromside->SetColor({ 1.0f,1.0f,0.0f });
	}
}