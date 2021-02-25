#include "stdafx.h"
#include "ShowModel.h"
#include "Bullet.h"

ShowModel::~ShowModel()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_pointLight);
}

bool ShowModel::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Player1.tkm");

	m_charaCon.Init(10.0f, 50.0f, m_position);

	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 0.0f,0.0f,0.0f });
	m_pointLight->SetRange(300.0f);

	return true;
}

void ShowModel::Update()
{

	//移動関連
	Vector3 front = m_position - g_camera3D->GetPosition();
	front.y = 0.0f;
	front.Normalize();

	Vector3 side = Cross(g_vec3AxisY, front);


	Vector3 m_moveSpeed = front * g_pad[0]->GetLStickYF() * 5.0f + side * g_pad[0]->GetLStickXF() * 5.0f;

	m_position = m_charaCon.Execute(m_moveSpeed,1.0f);
	m_skinModelRender->SetPosition(m_position);

	//攻撃関連

	//通常攻撃
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Bullet* bullet = NewGO<Bullet>(0, "bullet");
		bullet->m_position = m_position;
		bullet->m_position.y += 50;
		bullet->m_moveSpeed = g_camera3D->GetForward() * 10;
	}

	//チャージ
	if (g_pad[0]->IsPress(enButtonLB2) || g_pad[0]->IsPress(enButtonRB2))
	{
		m_charge += 10.0f;

		if (m_charge > 2000.0f)
		{
			m_charge = 2000.0f;
		}
	}

	//チャージ確認用
	m_pointLight->SetColor({ 0.0f,m_charge / 100,0.0f });
	m_pointLight->SetPosition(m_position);

	//固有攻撃
	if (g_pad[0]->IsPress(enButtonB) && m_charge > 1000)
	{
		Bullet* bullet = NewGO<Bullet>(0, "bullet");
		bullet->m_position = m_position;
		bullet->m_position.y += 50;
		bullet->m_moveSpeed = g_camera3D->GetForward() * 50;
		m_charge = 0;
	}

	//カメラ関連
	Vector3 targetPos = m_position;
	targetPos.y += 50.0f;

	Quaternion qRotY;
	qRotY.SetRotationDeg(Vector3::AxisY, g_pad[0]->GetRStickXF());
	qRotY.Apply(m_toCamera);

	Quaternion qRotX;
	qRotX.SetRotationDeg(side, g_pad[0]->GetRStickYF() * -1);
	qRotX.Apply(m_toCamera);

	Vector3 cameraPos = m_position + m_toCamera;
	//g_camera3D->SetPosition(m_position + m_toCamera);
	g_camera3D->SetPosition(cameraPos);
	g_camera3D->SetTarget(targetPos);
}