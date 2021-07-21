#include "stdafx.h"
#include "FinalHit.h"
#include "Player.h"
#include "GameScene.h"

namespace
{
	const float ANIMATION_SPEED_FINISH = 0.1f;//敗者を移すときのアニメーションスピード
	const float ANIMATION_SPEED_NORMAL = 1.0f;//通常のアニメーションスピード
	const int LOSECAMERALOOP_RIGHT = 0;		  //右から見たカメラのカウント数
	const int LOSECAMERALOOP_LEFT = 50;		  //左から見たカメラのカウント数
	const int LOSECAMERALOOP_FRONT = 100;	  //正面から見たカメラのカウント数
	const int LOSECAMERALOOP_LOSERBACK = 150; //後ろから見たカメラのカウント数
	const int LOSECAMERALOOP_TOWINNER = 200;  //勝者に寄るカメラのカウント数

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
	
	//画面分割を終了
	GameObjectManager::GetInstance()->Set2ScreenMode(false);
	m_gameScene->StopGameBGM();

	return true;
}

void FinalHit::Update()
{
	m_loser->GetSkinModelRender()->PlayAnimation(m_loser->enAnimationClip_Death);//アニメーション再生
	Vector3 winnerFrontPos;//勝者の位置から少し前に伸びるベクトル		
	Vector3 winnerHeadPos = m_winner->GetPosition();
	winnerHeadPos.y += PLAYER_TO_HEAD;//勝者の頭の位置
	Vector3 LastRight = Cross(g_vec3AxisY, m_loser->GetLastFrontDir());//最後に向いていた向きの右ベクトル
	Vector3 targetPos = m_loser->GetPosition();//ターゲットポジション
	m_sequenceCameraPos = targetPos;//カメラのポジション
	targetPos.y += TARGET_ADD_Y;//少し上に設定する
	targetPos += m_loser->GetLastFrontDir() * TARGET_APPLY_X;//キャラの少し後ろに伸ばす	
	m_sequenceCameraPos.y += CAMERAPOS_ADD_Y;//キャラより少し上くらい
	//ループの値に合わせてステータスを変える
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
	case enRight://右からのカメラ
		m_loser->GetSkinModelRender()->SetAnimationSpeed(ANIMATION_SPEED_FINISH);//アニメーションを遅くする
		m_winner->GetSkinModelRender()->SetAnimationSpeed(ANIMATION_SPEED_FINISH);
		m_sequenceCameraPos += LastRight * TO_RIGHT_DISTANCE;//右
		g_camera3D[0]->SetTarget(targetPos);
		break;
	case enLeft://左からのカメラ
		m_sequenceCameraPos += LastRight * TO_LEFT_DISTANCE;//左
		g_camera3D[0]->SetTarget(targetPos);
		break;
	case enFront://前からのカメラ
		m_sequenceCameraPos += m_loser->GetLastFrontDir() * TO_FRONT_DISTANCE;//正面
		g_camera3D[0]->SetTarget(targetPos);
		break;
	case enLoserBack://自分を写しながら敵を向いたカメラ
		m_loser->GetSkinModelRender()->SetAnimationSpeed(ANIMATION_SPEED_NORMAL);//アニメーションスピードをもとに戻す
		m_winner->GetSkinModelRender()->SetAnimationSpeed(ANIMATION_SPEED_NORMAL);
		//敵のちょっと前と自分を結んだ線を正規化して後ろに少し伸ばす
		m_winnerVec = (winnerHeadPos + m_winner->GetLastFrontDir() * TO_FRONT_DISTANCE) - m_loser->GetPosition();
		m_winnerVec.Normalize();
		m_sequenceCameraPos += m_winnerVec * TO_BACK_DISTANCE;//後ろ
		m_winnerWaistPos = m_winner->GetPosition();//敵の腰の位置
		///////////////////////////////////////////////////////////////////////////
		m_winnerWaistPos.y += 20;
		g_camera3D[0]->SetTarget(m_winnerWaistPos);
		break;
	case enToWinner://敵の前まで移動する
		//カメラを敵の前まで移動させる				
		winnerFrontPos = (m_winner->GetPosition() + m_winner->GetLastFrontDir() * 150) - (m_loser->GetPosition() + m_winnerVec * -200);//	
		if (m_coef < 1.0f)//ベクトルに掛ける値
			m_coef += 0.01f;
		if (m_coef > 0.5f)//カメラが半分の距離まで行くとアニメーションを再生する
		{
			m_isWinAnimOn=true;
		}
		m_sequenceCameraPos += (winnerFrontPos * (pow(m_coef, 1.5)));//指数関数的にカメラが近づく

		m_sequenceCameraPos += m_winnerVec * -200;//case3のときのカメラの位置と合わせるため

		g_camera3D[0]->SetTarget(m_winnerWaistPos);
		break;
	default:
		break;
	}

	//SE
	if (m_LoseCameraLoop == 0 || m_LoseCameraLoop == 50 || m_LoseCameraLoop == 100)
	{
		//音を再生
		SoundOneShotPlay(L"Assets/sound/K.O..wav", 1.5f);
	}
	else if (m_LoseCameraLoop == 250)
	{
		//ジングルを再生
		SoundOneShotPlay(L"Assets/sound/yattaze!1.wav", 1.5f);
	}
	else if (m_LoseCameraLoop == 300)
	{
		if (m_loserNum == 1)	//1Pが勝利した場合
		{
			m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(5);
			m_resultWinnerSprite->Init("Assets/Image/1P.DDS", 148, 120);
			m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
			m_resultWinnerSprite->SetPosition({ -120.0f, -120.0f, 0.0f });

			//ボイス再生
			SoundOneShotPlay(L"Assets/sound/Player1Win.wav", 3.0f);
		}
		else if (m_loserNum == 0)	//2Pが勝利した場合
		{
			m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(5);
			m_resultWinnerSprite->Init("Assets/Image/2P.DDS", 180, 128);
			m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
			m_resultWinnerSprite->SetPosition({ -120.0f, -120.0f, 0.0f });

			//ボイス再生
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



	if (m_isWinAnimOn == true)//カメラが半分の距離まで行くとアニメーションを再生する
	{
		m_winner->GetSkinModelRender()->PlayAnimation(m_winner->enAnimationClip_Winner);
	}
	else
	{
		m_winner->GetSkinModelRender()->PlayAnimation(m_winner->enAnimationClip_Idle);
	}
}