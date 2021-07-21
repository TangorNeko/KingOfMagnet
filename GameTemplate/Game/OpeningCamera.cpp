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

	if (g_pad[m_playerNum]->IsTrigger(enButtonA))//�I�[�v�j���O�J�����X�L�b�v
	{
		m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
		m_gameScene->DeleteSkipFont();
	}

	if (m_openingCameraCount < 250)//250�t���[���o�Ă�
	{
		
		Vector3 toPos = m_playerPos;
		
		toPos.y = 400;
		//toPos���񂷁B
		Quaternion qRotY;
		//Y�����ɉ�]����N�H�[�^�j�I��������
		m_openingCameraRotationRad += 0.01;
		qRotY.SetRotation(Vector3::AxisY, m_openingCameraRotationRad);
		//�N�H�[�^�j�I�����g����toPos����
		qRotY.Apply(toPos);
		m_sequenceCameraPos = toPos;
		g_camera3D[m_playerNum]->SetTarget(m_targetPos);
	}
	else//�L�����Ɍ������ăJ�������ړ�������
	{
		Vector3 PlayerPos = m_playerPos;
		PlayerPos.y = m_playerPos.y + 90.0f;//�v���C���[�̓��̈ʒu


		Vector3 targetVec = PlayerPos - m_sequenceCameraPos;
		if (targetVec.Length() < 250)//�J�������߂Â��΃I�[�v�j���O�J�����I��
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