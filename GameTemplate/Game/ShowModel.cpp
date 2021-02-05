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

	m_SpotLig[0] = NewGO<prefab::CSpotLight>(0);
	m_SpotLig[0]->SetPosition(m_ligPos[0]);
	m_SpotLig[0]->SetColor({ 2.0f,2.0f,2.0f });
	m_SpotLig[0]->SetRange(500.0f);
	Vector3 test = m_position - m_ligPos[0];
	test.Normalize();
	m_SpotLig[0]->SetDirection(test);
	m_SpotLig[0]->SetAngleDeg(30.0f);

	m_SpotLig[1] = NewGO<prefab::CSpotLight>(0);
	m_SpotLig[1]->SetPosition(m_ligPos[1]);
	m_SpotLig[1]->SetColor({ 2.0f,2.0f,2.0f });
	m_SpotLig[1]->SetRange(500.0f);
	test = m_position - m_ligPos[1];
	test.Normalize();
	m_SpotLig[1]->SetDirection(test);
	m_SpotLig[1]->SetAngleDeg(30.0f);

	return true;
}

void ShowModel::Update()
{
	m_position.z += 0.1f;

	if (g_pad[0]->IsPress(enButtonY))
	{
		m_deg += 1.0f;
	}

	if (g_pad[0]->IsPress(enButtonLeft))
	{
		for(int i = 0;i<2;i++)
			m_ligPos[i].x++;
	}

	if (g_pad[0]->IsPress(enButtonRight))
	{
		for (int i = 0;i < 2;i++)
			m_ligPos[i].x--;
	}

	if (g_pad[0]->IsPress(enButtonUp))
	{
		for (int i = 0;i < 2;i++)
			m_ligPos[i].z--;
	}

	if (g_pad[0]->IsPress(enButtonDown))
	{
		for (int i = 0;i < 2;i++)
			m_ligPos[i].z++;
	}

	m_eyePos.x -= g_pad[0]->GetLStickXF();
	m_eyePos.z -= g_pad[0]->GetLStickYF();

	g_camera3D->SetPosition(m_eyePos);
	g_camera3D->SetTarget(m_position);

	for (int i = 0;i < 2;i++)
	{
		m_SpotLig[i]->SetPosition(m_ligPos[i]);
		Vector3 test = m_position - m_ligPos[i];
		test.Normalize();
		m_SpotLig[i]->SetDirection(test);
	}

	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, m_deg);

	m_skinModelRender->SetRotation(qRot);
	m_skinModelRender->SetPosition(m_position);

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		yellowfromside = NewGO<prefab::CDirectionLight>(0);
		yellowfromside->SetDirection({ -1.0f,0.0f,0.0f });
		yellowfromside->SetColor({ 1.0f,1.0f,0.0f });
	}

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		DeleteGO(yellowfromside);
	}
}