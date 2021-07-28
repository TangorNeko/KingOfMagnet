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
	const int LOSECAMERALOOP_WINSE = 250;		  //�����T�E���h���Đ������J�E���g��
	const int LOSECAMERALOOP_PLAYERSPRITE_DISP = 300;//��P���\�������J�E���g��
	const int LOSECAMERALOOP_WINSPRITE_DISP = 350;	 //WIN���\�������J�E���g��

	const int PLAYER_TO_HEAD = 50;			  //�v���C���[�̓��̈ʒu
	const int TARGET_ADD_Y = 20;			  //�^�[�Q�b�g��������ɂ��炷
	const int TARGET_APPLY_BACK = -30;		  //���K�����ꂽ�x�N�g�������Ɍ����Ċ|����l
	const int CAMERAPOS_ADD_Y = 100;		  //�J������������ɂ��炷
	const int TO_RIGHT_DISTANCE = 200;		  //�E�J�����ƃv���C���[�̋���
	const int TO_LEFT_DISTANCE = -200;		  //���J�����ƃv���C���[�̋���
	const int TO_FRONT_DISTANCE = 200;		  //�O�J�����ƃv���C���[�̋���
	const int TO_FRONT_DISTANCE_SHORT = 150;  //�O�J�����ƃv���C���[�̏����Z������
	const int TO_BACK_DISTANCE = -200;		  //���J�����ƃv���C���[�̋���
	const int TO_WAIST_DISTANCE = 20;		  //�v���C���[�̍��̈ʒu�p
	const float HALF_VECTOR = 0.5f;			  //�x�N�g���̔����̒������o���p
	const float APPROACH_CAMERA_SPEED = 0.01f;//�J�������v���C���[�ɋ߂Â��X�s�[�h
	const float APPROACH_CAMERA_SPEED_INDEX = 1.5f;//�J�������v���C���[�ɋ߂Â��Ƃ��̎w���֐�
	const int ALL_VECTOR = 1;				  //�x�N�g���̑S�����o���p
	const float SE_VOLUME = 1.5f;			  //SE�̑傫��
	const float VOICE_VOLUME = 3.0f;		  //VOICE�̑傫��

	const int RESULTWINNERSPRITE1P_WIDTH = 148; //1P�摜�̉���
	const int RESULTWINNERSPRITE1P_HEIGHT = 120;//1P�摜�̏c��
	const int RESULTWINNERSPRITE2P_WIDTH = 180; //2P�摜�̉���
	const int RESULTWINNERSPRITE2P_HEIGHT = 128;//2P�摜�̏c��
	const Vector3 RESULTWINNERSPRITE_POSITION = { -120.0f, -120.0f, 0.0f };//1P2P�摜�̈ʒu
	const int RESULTWINSPRITE_WIDTH = 300;		//WIN�摜�̉���
	const int RESULTWINSPRITE_HEIGHT = 128;		//WIN�摜�̏c��
	const Vector3 RESULTWINSPRITE_POSITION = { 150.0f, -120.0f, 0.0f };//WIN�摜�̈ʒu

	const int SPRITE_PRIORITY = 5;				//�摜�̗D��x
	const int SPRITE_DRAWSCREEN = 2;			//�摜��S��ʕ\���ɂ���
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
	targetPos += m_loser->GetLastFrontDir() * TARGET_APPLY_BACK;//�L�����̏������ɐL�΂�	
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
		m_winnerWaistPos.y += TO_WAIST_DISTANCE;
		g_camera3D[0]->SetTarget(m_winnerWaistPos);
		break;
	case enToWinner://�G�̑O�܂ňړ�����
		//�J������G�̑O�܂ňړ�������				
		winnerFrontPos = (m_winner->GetPosition() + m_winner->GetLastFrontDir() * TO_FRONT_DISTANCE_SHORT) - (m_loser->GetPosition() + m_winnerVec * TO_BACK_DISTANCE);//	
		if (m_coef < ALL_VECTOR)//�x�N�g���Ɋ|����l
			m_coef += APPROACH_CAMERA_SPEED;
		if (m_coef > HALF_VECTOR)//�J�����������̋����܂ōs���ƃA�j���[�V�������Đ�����
		{
			m_isWinAnimOn=true;
		}
		m_sequenceCameraPos += (winnerFrontPos * (pow(m_coef, APPROACH_CAMERA_SPEED_INDEX)));//�w���֐��I�ɃJ�������߂Â�

		m_sequenceCameraPos += m_winnerVec * TO_BACK_DISTANCE;//case3�̂Ƃ��̃J�����̈ʒu�ƍ��킹�邽��

		g_camera3D[0]->SetTarget(m_winnerWaistPos);
		break;
	default:
		break;
	}

	//SE
	if (m_LoseCameraLoop == LOSECAMERALOOP_RIGHT || m_LoseCameraLoop == LOSECAMERALOOP_LEFT || m_LoseCameraLoop == LOSECAMERALOOP_FRONT)
	{
		//�����Đ�
		SoundOneShotPlay(L"Assets/sound/K.O..wav", SE_VOLUME);
	}
	else if (m_LoseCameraLoop == LOSECAMERALOOP_WINSE)
	{
		//�W���O�����Đ�
		SoundOneShotPlay(L"Assets/sound/WinSE.wav", SE_VOLUME);
	}
	else if (m_LoseCameraLoop == LOSECAMERALOOP_PLAYERSPRITE_DISP)
	{
		if (m_loserNum == NUMBER_PLAYER2)	//1P�����������ꍇ
		{
			m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIORITY);
			m_resultWinnerSprite->Init("Assets/Image/1P.DDS", RESULTWINNERSPRITE1P_WIDTH, RESULTWINNERSPRITE1P_HEIGHT);
			m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)SPRITE_DRAWSCREEN);
			m_resultWinnerSprite->SetPosition(RESULTWINNERSPRITE_POSITION);

			//�{�C�X�Đ�
			SoundOneShotPlay(L"Assets/sound/Player1Win.wav", VOICE_VOLUME);
		}
		else if (m_loserNum == NUMBER_PLAYER1)	//2P�����������ꍇ
		{
			m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIORITY);
			m_resultWinnerSprite->Init("Assets/Image/2P.DDS", RESULTWINNERSPRITE2P_WIDTH, RESULTWINNERSPRITE2P_HEIGHT);
			m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)SPRITE_DRAWSCREEN);
			m_resultWinnerSprite->SetPosition(RESULTWINNERSPRITE_POSITION);

			//�{�C�X�Đ�
			SoundOneShotPlay(L"Assets/sound/Player2Win.wav", VOICE_VOLUME);
		}
	}
	else if (m_LoseCameraLoop == LOSECAMERALOOP_WINSPRITE_DISP)
	{
		m_resultWinSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIORITY);
		m_resultWinSprite->Init("Assets/Image/Win.DDS", RESULTWINSPRITE_WIDTH , RESULTWINSPRITE_HEIGHT);
		m_resultWinSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)SPRITE_DRAWSCREEN);
		m_resultWinSprite->SetPosition({ RESULTWINSPRITE_POSITION});
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