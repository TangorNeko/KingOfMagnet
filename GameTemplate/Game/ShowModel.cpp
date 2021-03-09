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

	m_skinModelRender->Init("Assets/modelData/mage00.tkm","Assets/modelData/mageskel.tks");

	m_charaCon.Init(10.0f, 50.0f, m_position);
	//g_camera3D->SetViewAngle(g_camera3D->GetViewAngle() / 2);

	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 0.0f,0.0f,0.0f });
	m_pointLight->SetRange(300.0f);

	return true;
}

void ShowModel::Update()
{
	//プレイヤー番号が与えられていなければ何もしない
	if (m_playerNum == -1)
	{
		return;
	}

	//移動関連

	//カメラの前方向
	Vector3 front = m_position - g_camera3D[m_playerNum]->GetPosition();
	front.y = 0.0f;
	front.Normalize();
	
	//カメラの右方向
	Vector3 right = Cross(g_vec3AxisY, front);
	
	float naiseki = front.Dot(Vector3::AxisZ);//内積
	float angle = acosf(naiseki);//アークコサイン
	if (front.x < 0) {
		angle *= -1;
	}
	rot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(rot);
	
	m_moveSpeed = front * g_pad[m_playerNum]->GetLStickYF() * 5.0f + right * g_pad[m_playerNum]->GetLStickXF() * 5.0f;

	if (m_moveSpeed.Length() != 0)
	{
		m_dir = m_moveSpeed;
		m_dir.Normalize();
	}

	//移動アクション
	if (g_pad[m_playerNum]->IsTrigger(enButtonA) && m_stealthCount == 0)
	{
		m_skinModelRender->SetScale({ 0.0f,0.0f,0.0f });
		m_stealthCount = 50;
		m_moveSpeed += m_dir * 500.0f;
	}
	else
	{
		if (--m_stealthCount <= 0)
		{
			m_skinModelRender->SetScale({ Scale });
			m_stealthCount = 0;
		}
	}

	//攻撃関連
	
	//通常攻撃
	if (g_pad[m_playerNum]->IsTrigger(enButtonRB1))
	{
		if (m_isLock)
		{
			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_position = m_position;
			bullet->m_position.y += 50;
			Vector3 dir = m_enemy->m_magPosition - m_magPosition;
			dir.Normalize();
			bullet->m_moveDirection = dir;
			bullet->m_velocity = 25.0f;
			bullet->m_moveSpeed = dir * 25.0f;
			bullet->m_parentNo = m_playerNum;
		}
		else
		{
			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_position = m_position;
			bullet->m_position.y += 50;
			bullet->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
			//bullet->m_moveDirection = m_dir;
			bullet->m_moveDirection.y = 0.0f;
			bullet->m_moveDirection.Normalize();
			bullet->m_velocity = 25.0f;
			//bullet->m_moveSpeed = m_dir * 25.0f;
			bullet->m_parentNo = m_playerNum;
		}
	}

	//チャージ
	if (g_pad[m_playerNum]->IsPress(enButtonLB2) && g_pad[m_playerNum]->IsPress(enButtonRB2))
	{
		m_charge += 10.0f - m_magPower * 2.5f;

		if (m_charge > 2000.0f)
		{
			m_charge = 2000.0f;
		}

		m_moveSpeed = { 0.0f,0.0f,0.0f };
	}

	//チャージ確認用
	if (m_charge < 2000.0f) {
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
		if (m_isLock)
		{
			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_position = m_position;
			bullet->m_position.y += 50;
			Vector3 dir = m_enemy->m_magPosition - m_magPosition;
			dir.Normalize();
			bullet->m_moveDirection = dir;
			bullet->m_velocity = 50.0f;
			//bullet->m_moveSpeed = dir * 25.0f;
			bullet->m_parentNo = m_playerNum;
		}
		else
		{
			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_position = m_position;
			bullet->m_position.y += 50;
			bullet->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
			//bullet->m_moveDirection = m_dir;
			bullet->m_moveDirection.y = 0.0f;
			bullet->m_moveDirection.Normalize();
			bullet->m_velocity = 50.0f;
			//bullet->m_moveSpeed = m_dir * 25.0f;
			bullet->m_parentNo = m_playerNum;
		}
		m_charge = 0;
	}

	//移動関連2
	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f);
	m_magPosition = m_position;
	m_magPosition.y += 50.0f;
	m_skinModelRender->SetPosition(m_position);


	//カメラ関連
	if (g_pad[m_playerNum]->IsTrigger(enButtonRB3))
	{
		m_isLock = !m_isLock;
	}

	if(m_isLock)
	{ 
		Vector3 targetPos = m_enemy->m_position;
		targetPos.y += 50.0f;

		Vector3 toCamera = m_enemy->m_position - m_position;
		toCamera *= -1.0f;
		toCamera.Normalize();
		Vector3 Axis;
		Axis.Cross(toCamera, Vector3::Up);
		toCamera *= 140.0f;

		Quaternion qRot;
		qRot.SetRotationDeg(Axis, 45.0f);
		qRot.Apply(toCamera);


		g_camera3D[m_playerNum]->SetPosition(m_position + toCamera);
		g_camera3D[m_playerNum]->SetTarget(m_enemy->m_magPosition);
	}
	else
	{
		Vector3 targetPos = m_position;
		targetPos.y += 50.0f;

		Quaternion qRotY;
		qRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF()*1.5);
		qRotY.Apply(m_toCamera);

		Quaternion qRotX;
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -1.5);
		Vector3 checkToCamera = m_toCamera;
		qRotX.Apply(checkToCamera);
		checkToCamera.Normalize();
		float t = checkToCamera.Dot(Vector3::Up);
		if (t > 0.99f || t < 0.1f)
		{

		}
		else
		{
			qRotX.Apply(m_toCamera);
		}

		Vector3 cameraPos = m_position + m_toCamera;
		//g_camera3D->SetPosition(m_position + m_toCamera);
		g_camera3D[m_playerNum]->SetPosition(cameraPos);
		g_camera3D[m_playerNum]->SetTarget(targetPos);
	}
}