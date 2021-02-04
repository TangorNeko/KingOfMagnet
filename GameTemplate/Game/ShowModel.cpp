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

	redpointlig = NewGO<prefab::CPointLight>(0);
	redpointlig->SetPosition(m_ligPos);
	redpointlig->SetColor({ 1.0f,0.0f,0.0f });
	redpointlig->SetRange(100.0f);

	return true;
}

void ShowModel::Update()
{
	//m_position.z += 0.1f;

	if (g_pad[0]->IsPress(enButtonB))
	{
		m_deg += 1.0f;
	}

	if (g_pad[0]->IsPress(enButtonLeft))
	{
		m_ligPos.x++;
	}

	if (g_pad[0]->IsPress(enButtonRight))
	{
		m_ligPos.x--;
	}

	if (g_pad[0]->IsPress(enButtonUp))
	{
		m_ligPos.z--;
	}

	if (g_pad[0]->IsPress(enButtonDown))
	{
		m_ligPos.z++;
	}

	m_eyePos.x -= g_pad[0]->GetLStickXF();
	m_eyePos.z -= g_pad[0]->GetLStickYF();

	g_camera3D->SetPosition(m_eyePos);

	redpointlig->SetPosition(m_ligPos);
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