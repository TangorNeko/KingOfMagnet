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
	const int LOSECAMERALOOP_WINSE = 250;		  //勝利サウンドが再生されるカウント数
	const int LOSECAMERALOOP_PLAYERSPRITE_DISP = 300;//○Pが表示されるカウント数
	const int LOSECAMERALOOP_WINSPRITE_DISP = 350;	 //WINが表示されるカウント数

	const int PLAYER_TO_HEAD = 50;			  //プレイヤーの頭の位置
	const int TARGET_ADD_Y = 20;			  //ターゲットを少し上にずらす
	const int TARGET_APPLY_BACK = -30;		  //正規化されたベクトルを後ろに向けて掛ける値
	const int CAMERAPOS_ADD_Y = 100;		  //カメラを少し上にずらす
	const int TO_RIGHT_DISTANCE = 200;		  //右カメラとプレイヤーの距離
	const int TO_LEFT_DISTANCE = -200;		  //左カメラとプレイヤーの距離
	const int TO_FRONT_DISTANCE = 200;		  //前カメラとプレイヤーの距離
	const int TO_FRONT_DISTANCE_SHORT = 150;  //前カメラとプレイヤーの少し短い距離
	const int TO_BACK_DISTANCE = -200;		  //後ろカメラとプレイヤーの距離
	const int TO_WAIST_DISTANCE = 20;		  //プレイヤーの腰の位置用
	const float HALF_VECTOR = 0.5f;			  //ベクトルの半分の長さを出す用
	const float APPROACH_CAMERA_SPEED = 0.01f;//カメラがプレイヤーに近づくスピード
	const float APPROACH_CAMERA_SPEED_INDEX = 1.5f;//カメラがプレイヤーに近づくときの指数関数
	const int ALL_VECTOR = 1;				  //ベクトルの全長を出す用
	const float SE_VOLUME = 1.5f;			  //SEの大きさ
	const float VOICE_VOLUME = 3.0f;		  //VOICEの大きさ

	const int RESULTWINNERSPRITE1P_WIDTH = 148; //1P画像の横幅
	const int RESULTWINNERSPRITE1P_HEIGHT = 120;//1P画像の縦幅
	const int RESULTWINNERSPRITE2P_WIDTH = 180; //2P画像の横幅
	const int RESULTWINNERSPRITE2P_HEIGHT = 128;//2P画像の縦幅
	const Vector3 RESULTWINNERSPRITE_POSITION = { -120.0f, -120.0f, 0.0f };//1P2P画像の位置
	const int RESULTWINSPRITE_WIDTH = 300;		//WIN画像の横幅
	const int RESULTWINSPRITE_HEIGHT = 128;		//WIN画像の縦幅
	const Vector3 RESULTWINSPRITE_POSITION = { 150.0f, -120.0f, 0.0f };//WIN画像の位置

	const int SPRITE_PRIORITY = 5;				//画像の優先度
	const int SPRITE_DRAWSCREEN = 2;			//画像を全画面表示にする
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
	targetPos += m_loser->GetLastFrontDir() * TARGET_APPLY_BACK;//キャラの少し後ろに伸ばす	
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
		m_winnerWaistPos.y += TO_WAIST_DISTANCE;
		g_camera3D[0]->SetTarget(m_winnerWaistPos);
		break;
	case enToWinner://敵の前まで移動する
		//カメラを敵の前まで移動させる				
		winnerFrontPos = (m_winner->GetPosition() + m_winner->GetLastFrontDir() * TO_FRONT_DISTANCE_SHORT) - (m_loser->GetPosition() + m_winnerVec * TO_BACK_DISTANCE);//	
		if (m_coef < ALL_VECTOR)//ベクトルに掛ける値
			m_coef += APPROACH_CAMERA_SPEED;
		if (m_coef > HALF_VECTOR)//カメラが半分の距離まで行くとアニメーションを再生する
		{
			m_isWinAnimOn=true;
		}
		m_sequenceCameraPos += (winnerFrontPos * (pow(m_coef, APPROACH_CAMERA_SPEED_INDEX)));//指数関数的にカメラが近づく

		m_sequenceCameraPos += m_winnerVec * TO_BACK_DISTANCE;//case3のときのカメラの位置と合わせるため

		g_camera3D[0]->SetTarget(m_winnerWaistPos);
		break;
	default:
		break;
	}

	//SE
	if (m_LoseCameraLoop == LOSECAMERALOOP_RIGHT || m_LoseCameraLoop == LOSECAMERALOOP_LEFT || m_LoseCameraLoop == LOSECAMERALOOP_FRONT)
	{
		//音を再生
		SoundOneShotPlay(L"Assets/sound/K.O..wav", SE_VOLUME);
	}
	else if (m_LoseCameraLoop == LOSECAMERALOOP_WINSE)
	{
		//ジングルを再生
		SoundOneShotPlay(L"Assets/sound/WinSE.wav", SE_VOLUME);
	}
	else if (m_LoseCameraLoop == LOSECAMERALOOP_PLAYERSPRITE_DISP)
	{
		if (m_loserNum == NUMBER_PLAYER2)	//1Pが勝利した場合
		{
			m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIORITY);
			m_resultWinnerSprite->Init("Assets/Image/1P.DDS", RESULTWINNERSPRITE1P_WIDTH, RESULTWINNERSPRITE1P_HEIGHT);
			m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)SPRITE_DRAWSCREEN);
			m_resultWinnerSprite->SetPosition(RESULTWINNERSPRITE_POSITION);

			//ボイス再生
			SoundOneShotPlay(L"Assets/sound/Player1Win.wav", VOICE_VOLUME);
		}
		else if (m_loserNum == NUMBER_PLAYER1)	//2Pが勝利した場合
		{
			m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIORITY);
			m_resultWinnerSprite->Init("Assets/Image/2P.DDS", RESULTWINNERSPRITE2P_WIDTH, RESULTWINNERSPRITE2P_HEIGHT);
			m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)SPRITE_DRAWSCREEN);
			m_resultWinnerSprite->SetPosition(RESULTWINNERSPRITE_POSITION);

			//ボイス再生
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



	if (m_isWinAnimOn == true)//カメラが半分の距離まで行くとアニメーションを再生する
	{
		m_winner->GetSkinModelRender()->PlayAnimation(m_winner->enAnimationClip_Winner);
	}
	else
	{
		m_winner->GetSkinModelRender()->PlayAnimation(m_winner->enAnimationClip_Idle);
	}
}