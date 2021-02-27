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
	//g_camera3D->SetViewAngle(g_camera3D->GetViewAngle() / 2);

	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 0.0f,0.0f,0.0f });
	m_pointLight->SetRange(300.0f);

	return true;
}

void ShowModel::Update()
{

	if (m_playerNum == -1)
	{
		return;
	}

	//移動関連
	Vector3 front = m_position - g_camera3D[m_playerNum]->GetPosition();
	front.y = 0.0f;
	front.Normalize();

	Vector3 side = Cross(g_vec3AxisY, front);


	m_moveSpeed = front * g_pad[m_playerNum]->GetLStickYF() * 5.0f + side * g_pad[m_playerNum]->GetLStickXF() * 5.0f;

	//攻撃関連
	
	//通常攻撃
	if (g_pad[m_playerNum]->IsTrigger(enButtonRB1))
	{
		Bullet* bullet = NewGO<Bullet>(0, "bullet");
		bullet->m_position = m_position;
		bullet->m_position.y += 50;
		bullet->m_moveDirection = g_camera3D[m_playerNum]->GetForward();
		bullet->m_moveDirection.y = 0.0f;
		bullet->m_moveDirection.Normalize();
		bullet->m_velocity = 10.0f;
	}

	//チャージ
	if (g_pad[m_playerNum]->IsPress(enButtonLB2) && g_pad[m_playerNum]->IsPress(enButtonRB2))
	{
		m_charge += 10.0f;

		if (m_charge > 2000.0f)
		{
			m_charge = 2000.0f;
		}

		m_moveSpeed = { 0.0f,0.0f,0.0f };
	}

	//チャージ確認用
	if (m_charge != 2000.0f) {
		m_pointLight->SetColor({ 0.0f,m_charge / 100,0.0f });
	}
	else
	{
		m_pointLight->SetColor({ 1.0f,0.0f,0.0f });
	}
	m_pointLight->SetPosition(m_position);

	//固有攻撃
	if (g_pad[m_playerNum]->IsPress(enButtonX) && m_charge > 1000)
	{
		Bullet* bullet = NewGO<Bullet>(0, "bullet");
		bullet->m_position = m_position;
		bullet->m_position.y += 50;
		bullet->m_moveDirection = g_camera3D[m_playerNum]->GetForward();
		bullet->m_moveDirection.y = 0.0f;
		bullet->m_moveDirection.Normalize();
		bullet->m_velocity = 50.0f;
		m_charge = 0;
	}

	//移動関連2
	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f);
	m_skinModelRender->SetPosition(m_position);


	//カメラ関連
		Vector3 targetPos = m_position;
		targetPos.y += 50.0f;

		Quaternion qRotY;
		qRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF());
		qRotY.Apply(m_toCamera);

		Quaternion qRotX;
		qRotX.SetRotationDeg(side, g_pad[m_playerNum]->GetRStickYF() * -1);
		qRotX.Apply(m_toCamera);

		Vector3 cameraPos = m_position + m_toCamera;
		//g_camera3D->SetPosition(m_position + m_toCamera);
		g_camera3D[m_playerNum]->SetPosition(cameraPos);
		g_camera3D[m_playerNum]->SetTarget(targetPos);
}