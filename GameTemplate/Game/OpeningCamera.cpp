#include "stdafx.h"
#include "OpeningCamera.h"
#include "GameScene.h"

bool OpeningCamera::Start()
{
	m_gameScene = FindGO<GameScene>("gamescene");	
	return true;
}
void OpeningCamera::Update()
{
	CameraMove();
}
void OpeningCamera::CameraMove()
{
	
	m_openingCameraCount++;

	if (g_pad[m_playerNum]->IsTrigger(enButtonA))//オープニングカメラスキップ
	{
		m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
		m_gameScene->DeleteSkipFont();
	}

	if (m_openingCameraCount < 250)//250フレーム経てば
	{
		
		Vector3 toPos = m_playerPos;
		
		toPos.y = 400;
		//toPosを回す。
		Quaternion qRotY;
		//Y軸回りに回転するクォータニオンをつくる
		m_openingCameraRotationRad += 0.01;
		qRotY.SetRotation(Vector3::AxisY, m_openingCameraRotationRad);
		//クォータニオンを使ってtoPosを回す
		qRotY.Apply(toPos);
		m_sequenceCameraPos = toPos;
		g_camera3D[m_playerNum]->SetTarget(m_targetPos);
	}
	else//キャラに向かってカメラを移動させる
	{
		Vector3 PlayerPos = m_playerPos;
		PlayerPos.y = m_playerPos.y + 90.0f;//プレイヤーの頭の位置


		Vector3 targetVec = PlayerPos - m_sequenceCameraPos;
		if (targetVec.Length() < 250)//カメラが近づけばオープニングカメラ終了
		{
			m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
			m_gameScene->DeleteSkipFont();
		}
		targetVec.Normalize();
		m_sequenceCameraPos += targetVec * m_openingCamerZoomSpeed;
		m_openingCamerZoomSpeed += 0.1;
		g_camera3D[m_playerNum]->SetTarget(PlayerPos);
	}
	g_camera3D[m_playerNum]->SetPosition(m_sequenceCameraPos);	
}