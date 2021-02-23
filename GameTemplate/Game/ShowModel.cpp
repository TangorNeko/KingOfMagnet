#include "stdafx.h"
#include "ShowModel.h"

ShowModel::~ShowModel()
{
	DeleteGO(m_skinModelRender);
}

bool ShowModel::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Player1.tkm");

	m_charaCon.Init(20.0f, 100.0f, m_position);

	return true;
}

void ShowModel::Update()
{

	//ˆÚ“®ŠÖ˜A
	Vector3 front = m_position - g_camera3D->GetPosition();
	front.y = 0.0f;
	front.Normalize();

	Vector3 side = Cross(g_vec3AxisY, front);


	Vector3 m_moveSpeed = front * g_pad[0]->GetLStickYF() * 5.0f + side * g_pad[0]->GetLStickXF() * 5.0f;

	m_position = m_charaCon.Execute(m_moveSpeed,1.0f);
	m_skinModelRender->SetPosition(m_position);


	//ƒJƒƒ‰ŠÖ˜A
	Vector3 cameraPos = m_position;
	cameraPos.y += 50.0f;

	Quaternion qRotY;
	qRotY.SetRotationDeg(Vector3::AxisY, g_pad[0]->GetRStickXF());
	qRotY.Apply(m_toCamera);

	Quaternion qRotX;
	qRotX.SetRotationDeg(side, g_pad[0]->GetRStickYF() * -1);
	qRotX.Apply(m_toCamera);

	g_camera3D->SetPosition(m_position + m_toCamera);
	g_camera3D->SetTarget(cameraPos);
}