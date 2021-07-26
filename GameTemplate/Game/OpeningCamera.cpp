#include "stdafx.h"
#include "OpeningCamera.h"
#include "GameScene.h"

namespace
{
	const int TIME_CAMERA_ROTATION = 250;		//最初にカメラが回転する時間
	const int PLAYER_OVERHEAD = 400;			//プレイヤーの頭上
	const float CAMERA_ROTATION_SPEED = 0.01f;	//カメラが回転する速さ
	const int PLAYER_HEAD = 90;					//プレイヤーの頭の位置
	const int RANGE_OF_CAMERA_AND_PLAYER = 250;	//最後のカメラとプレイヤーの距離
	const float SPEED_APPROACH_CAMERA = 0.1f;	//カメラがプレイヤーに近づく速さ
}

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
	//注視点：ステージの中心
	//半径：プレイヤーの初期座標
	if (m_openingCameraCount < TIME_CAMERA_ROTATION)//250フレーム経つまでカメラが回転する
	{		
		Vector3 toPos = m_playerPos;//プレイヤーの座標を入れる
		
		toPos.y = PLAYER_OVERHEAD;//上にずらす
		//toPosを回す。
		Quaternion qRotY;
		//Y軸回りに回転するクォータニオンをつくる
		m_openingCameraRotationRad += CAMERA_ROTATION_SPEED;
		qRotY.SetRotation(Vector3::AxisY, m_openingCameraRotationRad);
		//クォータニオンを使ってtoPosを回す
		qRotY.Apply(toPos);
		m_sequenceCameraPos = toPos;
		g_camera3D[m_playerNum]->SetTarget(m_targetPos);
	}
	else//キャラに向かってカメラを移動させる
	{
		Vector3 PlayerPos = m_playerPos;
		PlayerPos.y = m_playerPos.y + PLAYER_HEAD;//プレイヤーの頭の位置

		Vector3 targetVec = PlayerPos - m_sequenceCameraPos;
		if (targetVec.Length() < RANGE_OF_CAMERA_AND_PLAYER)//カメラが近づけばオープニングカメラ終了
		{
			m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
			m_gameScene->DeleteSkipFont();
		}
		targetVec.Normalize();
		m_sequenceCameraPos += targetVec * m_openingCameraZoomSpeed;
		m_openingCameraZoomSpeed += SPEED_APPROACH_CAMERA;
		g_camera3D[m_playerNum]->SetTarget(PlayerPos);
	}
	g_camera3D[m_playerNum]->SetPosition(m_sequenceCameraPos);	
}