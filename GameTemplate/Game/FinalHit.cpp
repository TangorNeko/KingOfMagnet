#include "stdafx.h"
#include "FinalHit.h"
#include "Player.h"
#include "GameScene.h"

namespace
{
	const float ANIMATION_SPEED_FINISH = 0.1f;//�s�҂��ڂ��Ƃ��̃A�j���[�V�����X�s�[�h
	const float ANIMATION_SPEED_NORMAL = 1.0f;//�ʏ�̃A�j���[�V�����X�s�[�h
	const int LOSECAMERALOOP_RIGHT = 0;		  //�E���猩���J�����̃J�E���g��
	const int LOSECAMERALOOP_LEFT = 50;		  //�����猩���J�����̃J�E���g��
	const int LOSECAMERALOOP_FRONT = 100;	  //���ʂ��猩���J�����̃J�E���g��
	const int LOSECAMERALOOP_LOSERBACK = 150; //��납�猩���J�����̃J�E���g��
	const int LOSECAMERALOOP_TOWINNER = 200;  //���҂Ɋ��J�����̃J�E���g��

	const int PLAYER_TO_HEAD = 50;
	const int TARGET_ADD_Y = 20;
	const int TARGET_APPLY_X = -30;
	const int CAMERAPOS_ADD_Y = 100;
	const int TO_RIGHT_DISTANCE = 200;
	const int TO_LEFT_DISTANCE = -200;
	const int TO_FRONT_DISTANCE = 200;
	const int TO_BACK_DISTANCE = -200;

}

FinalHit::~FinalHit()
{
	DeleteGO(m_resultWinnerSprite);
	DeleteGO(m_resultWinSprite);
}

bool FinalHit::Start()
{
	m_gameScene = FindGO<GameScene>("gamescene");
	if (m_loserNum == NUMBER_PLAYER1)
	{
		m_loser = m_gameScene->GetPlayer1();
		m_winner = m_gameScene->GetPlayer2();
	}
	else
	{
		m_loser = m_gameScene->GetPlayer2();
		m_winner = m_gameScene->GetPlayer1();
	}
	
	//��ʕ������I��
	GameObjectManager::GetInstance()->Set2ScreenMode(false);
	m_gameScene->StopGameBGM();

	return true;
}

void FinalHit::Update()
{
	m_loser->GetSkinModelRender()->PlayAnimation(m_loser->enAnimationClip_Death);//�A�j���[�V�����Đ�
	Vector3 winnerFrontPos;//���҂̈ʒu���班���O�ɐL�т�x�N�g��		
	Vector3 winnerHeadPos = m_winner->GetPosition();
	winnerHeadPos.y += PLAYER_TO_HEAD;//���҂̓��̈ʒu
	Vector3 LastRight = Cross(g_vec3AxisY, m_loser->GetLastFrontDir());//�Ō�Ɍ����Ă��������̉E�x�N�g��
	Vector3 targetPos = m_loser->GetPosition();//�^�[�Q�b�g�|�W�V����
	m_sequenceCameraPos = targetPos;//�J�����̃|�W�V����
	targetPos.y += TARGET_ADD_Y;//������ɐݒ肷��
	targetPos += m_loser->GetLastFrontDir() * TARGET_APPLY_X;//�L�����̏������ɐL�΂�	
	m_sequenceCameraPos.y += CAMERAPOS_ADD_Y;//�L������菭���キ�炢
	//���[�v�̒l�ɍ��킹�ăX�e�[�^�X��ς���
	if (m_LoseCameraLoop == LOSECAMERALOOP_RIGHT)
	{
		m_finalCameraState = enRight;
	}
	if (m_LoseCameraLoop == LOSECAMERALOOP_LEFT)
	{
		m_finalCameraState = enLeft;
	}
	if (m_LoseCameraLoop == LOSECAMERALOOP_FRONT)
	{
		m_finalCameraState = enFront;
	}
	if (m_LoseCameraLoop == LOSECAMERALOOP_LOSERBACK)
	{
		m_finalCameraState = enLoserBack;
	}
	if (m_LoseCameraLoop == LOSECAMERALOOP_TOWINNER)
	{
		m_finalCameraState = enToWinner;
	}

	switch (m_finalCameraState)
	{
	case enRight://�E����̃J����
		m_loser->GetSkinModelRender()->SetAnimationSpeed(ANIMATION_SPEED_FINISH);//�A�j���[�V������x������
		m_winner->GetSkinModelRender()->SetAnimationSpeed(ANIMATION_SPEED_FINISH);
		m_sequenceCameraPos += LastRight * TO_RIGHT_DISTANCE;//�E
		g_camera3D[0]->SetTarget(targetPos);
		break;
	case enLeft://������̃J����
		m_sequenceCameraPos += LastRight * TO_LEFT_DISTANCE;//��
		g_camera3D[0]->SetTarget(targetPos);
		break;
	case enFront://�O����̃J����
		m_sequenceCameraPos += m_loser->GetLastFrontDir() * TO_FRONT_DISTANCE;//����
		g_camera3D[0]->SetTarget(targetPos);
		break;
	case enLoserBack://�������ʂ��Ȃ���G���������J����
		m_loser->GetSkinModelRender()->SetAnimationSpeed(ANIMATION_SPEED_NORMAL);//�A�j���[�V�����X�s�[�h�����Ƃɖ߂�
		m_winner->GetSkinModelRender()->SetAnimationSpeed(ANIMATION_SPEED_NORMAL);
		//�G�̂�����ƑO�Ǝ��������񂾐��𐳋K�����Č��ɏ����L�΂�
		m_winnerVec = (winnerHeadPos + m_winner->GetLastFrontDir() * TO_FRONT_DISTANCE) - m_loser->GetPosition();
		m_winnerVec.Normalize();
		m_sequenceCameraPos += m_winnerVec * TO_BACK_DISTANCE;//���
		m_winnerWaistPos = m_winner->GetPosition();//�G�̍��̈ʒu
		///////////////////////////////////////////////////////////////////////////
		m_winnerWaistPos.y += 20;
		g_camera3D[0]->SetTarget(m_winnerWaistPos);
		break;
	case enToWinner://�G�̑O�܂ňړ�����
		//�J������G�̑O�܂ňړ�������				
		winnerFrontPos = (m_winner->GetPosition() + m_winner->GetLastFrontDir() * 150) - (m_loser->GetPosition() + m_winnerVec * -200);//	
		if (m_coef < 1.0f)//�x�N�g���Ɋ|����l
			m_coef += 0.01f;
		if (m_coef > 0.5f)//�J�����������̋����܂ōs���ƃA�j���[�V�������Đ�����
		{
			m_isWinAnimOn=true;
		}
		m_sequenceCameraPos += (winnerFrontPos * (pow(m_coef, 1.5)));//�w���֐��I�ɃJ�������߂Â�

		m_sequenceCameraPos += m_winnerVec * -200;//case3�̂Ƃ��̃J�����̈ʒu�ƍ��킹�邽��

		g_camera3D[0]->SetTarget(m_winnerWaistPos);
		break;
	default:
		break;
	}

	//SE
	if (m_LoseCameraLoop == 0 || m_LoseCameraLoop == 50 || m_LoseCameraLoop == 100)
	{
		//�����Đ�
		SoundOneShotPlay(L"Assets/sound/K.O..wav", 1.5f);
	}
	else if (m_LoseCameraLoop == 250)
	{
		//�W���O�����Đ�
		SoundOneShotPlay(L"Assets/sound/yattaze!1.wav", 1.5f);
	}
	else if (m_LoseCameraLoop == 300)
	{
		if (m_loserNum == 1)	//1P�����������ꍇ
		{
			m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(5);
			m_resultWinnerSprite->Init("Assets/Image/1P.DDS", 148, 120);
			m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
			m_resultWinnerSprite->SetPosition({ -120.0f, -120.0f, 0.0f });

			//�{�C�X�Đ�
			SoundOneShotPlay(L"Assets/sound/Player1Win.wav", 3.0f);
		}
		else if (m_loserNum == 0)	//2P�����������ꍇ
		{
			m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(5);
			m_resultWinnerSprite->Init("Assets/Image/2P.DDS", 180, 128);
			m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
			m_resultWinnerSprite->SetPosition({ -120.0f, -120.0f, 0.0f });

			//�{�C�X�Đ�
			SoundOneShotPlay(L"Assets/sound/Player2Win.wav", 3.0f);
		}
	}
	else if (m_LoseCameraLoop == 350)
	{
		m_resultWinSprite = NewGO<prefab::CSpriteRender>(5);
		m_resultWinSprite->Init("Assets/Image/Win.DDS", 300, 128);
		m_resultWinSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
		m_resultWinSprite->SetPosition({ 150.0f, -120.0f, 0.0f });
	}

	g_camera3D[0]->SetPosition(m_sequenceCameraPos);
	m_LoseCameraLoop++;



	if (m_isWinAnimOn == true)//�J�����������̋����܂ōs���ƃA�j���[�V�������Đ�����
	{
		m_winner->GetSkinModelRender()->PlayAnimation(m_winner->enAnimationClip_Winner);
	}
	else
	{
		m_winner->GetSkinModelRender()->PlayAnimation(m_winner->enAnimationClip_Idle);
	}
}