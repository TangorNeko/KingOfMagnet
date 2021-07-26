#include "stdafx.h"
#include "OpeningCamera.h"
#include "GameScene.h"

namespace
{
	const int TIME_CAMERA_ROTATION = 250;		//�ŏ��ɃJ��������]���鎞��
	const int PLAYER_OVERHEAD = 400;			//�v���C���[�̓���
	const float CAMERA_ROTATION_SPEED = 0.01f;	//�J��������]���鑬��
	const int PLAYER_HEAD = 90;					//�v���C���[�̓��̈ʒu
	const int RANGE_OF_CAMERA_AND_PLAYER = 250;	//�Ō�̃J�����ƃv���C���[�̋���
	const float SPEED_APPROACH_CAMERA = 0.1f;	//�J�������v���C���[�ɋ߂Â�����
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

	if (g_pad[m_playerNum]->IsTrigger(enButtonA))//�I�[�v�j���O�J�����X�L�b�v
	{
		m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
		m_gameScene->DeleteSkipFont();
	}
	//�����_�F�X�e�[�W�̒��S
	//���a�F�v���C���[�̏������W
	if (m_openingCameraCount < TIME_CAMERA_ROTATION)//250�t���[���o�܂ŃJ��������]����
	{		
		Vector3 toPos = m_playerPos;//�v���C���[�̍��W������
		
		toPos.y = PLAYER_OVERHEAD;//��ɂ��炷
		//toPos���񂷁B
		Quaternion qRotY;
		//Y�����ɉ�]����N�H�[�^�j�I��������
		m_openingCameraRotationRad += CAMERA_ROTATION_SPEED;
		qRotY.SetRotation(Vector3::AxisY, m_openingCameraRotationRad);
		//�N�H�[�^�j�I�����g����toPos����
		qRotY.Apply(toPos);
		m_sequenceCameraPos = toPos;
		g_camera3D[m_playerNum]->SetTarget(m_targetPos);
	}
	else//�L�����Ɍ������ăJ�������ړ�������
	{
		Vector3 PlayerPos = m_playerPos;
		PlayerPos.y = m_playerPos.y + PLAYER_HEAD;//�v���C���[�̓��̈ʒu

		Vector3 targetVec = PlayerPos - m_sequenceCameraPos;
		if (targetVec.Length() < RANGE_OF_CAMERA_AND_PLAYER)//�J�������߂Â��΃I�[�v�j���O�J�����I��
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