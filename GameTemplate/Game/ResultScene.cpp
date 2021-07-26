#include "stdafx.h"
#include "ResultScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "RoundCounter.h"

namespace
{
	const Vector3 RESULT_CAMERA_POSITION = { 0.0f,90.0f,250.0f };			//リザルト画面のカメラ座標
	const Vector3 RESULT_CAMERA_TARGET = { 0.0f,90.0f,0.0f };				//リザルト画面のカメラ注視点
	const Vector3 MODEL_WINNER_POSITION = { 60.0f,25.0f,100.0f };			//勝者の座標
	const Vector3 MODEL_LOSER_POSITION = { -50.0f,40.0f,100.0f };			//敗者の座標
	const Vector3 SPRITE_WINNER_POSITION = { -200.0f,280.0f,0.0f };			//WINNスプライトの座標
	const Vector3 SPRITE_LOSER_POSITION = { 350.0f,220.0f,0.0f };			//LOSEスプライトの座標
	const Vector3 SPRITE_COMMAND_POSITION = { 0.0f,-278.0f,0.0f };			//コマンドスプライトの座標
	const Vector4 SPRITE_COLOR_TRANSPARENT = { 1.0f,1.0f,1.0f,0.0f };		//乗算カラー　透明
	const Vector4 SPRITE_COLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };				//乗算カラー　黒
	const Vector3 SPRITE_COVER_UPPER_STARTPOSITION = { 0.0f,400.0f,0.0f };	//上側のカバースプライトの初期座標
	const Vector3 SPRITE_COVER_LOWER_STARTPOSITION = { 0.0f,-400.0f,0.0f };	//下側のカバースプライトの初期座標
	const int SPRITE_BACKGROUND_WIDTH = 1280;								//背景スプライトの幅
	const int SPRITE_BACKGROUND_HEIGHT = 720;								//背景スプライトの高さ
	const int SPRITE_COVER_UPPER_WIDTH = 1280;								//上側のカバースプライトの幅
	const int SPRITE_COVER_UPPER_HEIGHT = 720;								//上側のカバースプライトの高さ
	const int SPRITE_RESULT_WINDOW_WIDTH = 1280;							//リザルト窓スプライトの幅
	const int SPRITE_RESULT_WINDOW_HEIGHT = 720;							//リザルト窓スプライトの高さ
	const int SPRITE_LOSE_WIDTH = 372;										//LOSEスプライトの幅
	const int SPRITE_LOSE_HEIGHT = 132;										//LOSEスプライトの高さ
	const int SPRITE_WIN_WIDTH = 300;										//WINスプライトの幅
	const int SPRITE_WIN_HEIGHT = 112;										//WINスプライトの高さ
	const int SPRITE_COMMAND_WIDTH = 400;									//コマンドスプライトの幅
	const int SPRITE_COMMAND_HEIGHT = 76;									//コマンドスプライトの高さ
	const int SPRITE_COVER_LOWER_WIDTH = 1280;								//下側のカバースプライトの幅
	const int SPRITE_COVER_LOWER_HEIGHT = 720;								//下側のカバースプライトの高さ
	const float SPRITE_COVER_MOVE_AMOUNT = 400.0f;							//スプライトカバーの移動量
	const int SPRITE_MOVETIMER_START = 0;									//スプライトの移動タイマー　スタート
	const int SPRITE_MOVETIMER_DISPLAY_AND_SHAKE = 60;						//スプライトの移動タイマー　結果表示
	const int SPRITE_MOVETIMER_PLAY_SE1 = 25;								//スプライトの移動タイマー　SE1を鳴らす
	const int SPRITE_MOVETIMER_PLAY_SE2 = 55;								//スプライトの移動タイマー　SE2を鳴らす
	const int SPRITE_MOVETIMER_PLAY_BGM = 80;								//スプライトの移動タイマー　BGMを鳴らす
	const int SPRITE_MOVETIMER_END = 120;									//スプライトの移動タイマー　移動終了
	const Vector3 DIRECTIONLIGHT_RESULT_DIRECTION = { 0.0f,-1.0f,-1.0f };	//リザルトシーンのライトの向き
	const Vector3 DIRECTIONLIGHT_COLOR_WHITE = { 1.0f,1.0f,1.0f };			//リザルトシーンのライトの色
	const float SOUND_SE_RESULT_VOLUME = 1.2f;								//リザルトSEのボリューム
	const float SOUND_BGM_RESULT_VOLUME = 0.2f;								//リザルトBGMのボリューム

	const int SPRITEMOVELIST_ZERO = 0;										//スプライトの移動リストに何も登録されていない

	const Vector2 SPRITE_UP_CLOSE_MOVEMENT
				= { 0.0f,-SPRITE_COVER_MOVE_AMOUNT - 20.0f };				//上側のカバースプライトが閉じる時の移動量
	const Vector2 SPRITE_UP_CLOSE_REBOUND_UP_MOVEMENT = { 0.0f,40.0f };		//上側のカバースプライトがバウンドして少し開く時の移動量
	const Vector2 SPRITE_UP_CLOSE_REBOUND_DOWN_MOVEMENT = { 0.0f,-20.0f };	//上側のカバースプライトがバウンドして完全に閉じる時の移動量

	const Vector2 SPRITE_DOWN_CLOSE_MOVEMENT
				= { 0.0f,SPRITE_COVER_MOVE_AMOUNT + 20.0f };				//下側のカバースプライトが閉じる時の移動量
	const Vector2 SPRITE_DOWN_CLOSE_REBOUND_UP_MOVEMENT = { 0.0f,-40.0f };	//下側のカバースプライトがバウンドして少し開く時の移動量
	const Vector2 SPRITE_DOWN_CLOSE_REBOUND_DOWN_MOVEMENT = { 0.0f,20.0f };	//下側のカバースプライトがバウンドして完全に閉じる時の移動量
	
	const Vector2 SPRITE_COMMAND_CLOSE_MOVEMENT
				= { 0.0f,SPRITE_COVER_MOVE_AMOUNT + 20.0f };				//コマンドスプライトが閉じる時の移動量
	const Vector2 SPRITE_COMMAND_CLOSE_REBOUND_DOWN_MOVEMENT
				= { 0.0f,-40.0f };											//コマンドスプライトがバウンドして少し開く時の移動量
	const Vector2 SPRITE_COMMAND_CLOSE_REBOUND_UP_MOVEMENT
				= { 0.0f,20.0f};											//コマンドスプライトがバウンドして完全に閉じる時の移動量

	const int SPRITE_CLOSE_MOVETIME = 30;									//カバースプライトが閉じる時の移動時間
	const int SPRITE_CLOSE_REBOUND_MOVETIME = 12;							//カバースプライトがバウンドする時の移動時間

	const int SPRITE_CLOSE_MOVEDELAY = 0;									//カバースプライトが閉じる時のディレイ
	const int SPRITE_CLOSE_REBOUND_UP_MOVEDELAY = 30;						//カバースプライトがバウンドして少し開く時のディレイ
	const int SPRITE_CLOSE_REBOUND_DOWN_MOVEDELAY = 42;						//カバースプライトがバウンドして完全に閉じる時のディレイ

	const int SPRITE_MOVEDELAY_ZERO = 0;									//移動ディレイなし
	const int SPRITE_APPEAR_TIME = 12;										//スプライトが透明状態から完全に表示されるまでの時間

	const Vector2 SPRITE_LOSE_SHAKE_MOVEMENT = { 20.0f,0.0f };				//LOSEスプライトのシェイクの移動量
	const int SPRITE_LOSE_SHAKE_MOVETIME = 24;								//LOSEスプライトのシェイクの移動時間

	const Vector3 SPRITE_WIN_HORIZONTAL_SCALE_MIDDLE = { 1.2f,0.8f,1.0f };	//WINスプライトが横に伸ばされた時の拡大率
	const Vector3 SPRITE_WIN_HORIZONTAL_SCALE_HEAVY = { 1.5f,0.5f,1.0f };	//WINスプライトが横に大きく伸ばされた時の拡大率
	const Vector3 SPRITE_WIN_VERTICAL_SCALE_LIGHT = { 0.9f,1.1f,1.0f };		//WINスプライトが縦に小さく伸ばされた時の拡大率
	const Vector3 SPRITE_WIN_VERTICAL_SCALE_MIDDLE = { 0.8f,1.2f,1.0f };	//WINスプライトが縦に伸ばされた時の拡大率
	const Vector3 SPRITE_WIN_VERTICAL_SCALE_HEAVY = { 0.6f,1.4f,1.0f };		//WINスプライトが縦に大きく伸ばされた時の拡大率
	const Vector3 SPRITE_WIN_SCALE_DEFAULT = { 1.0f,1.0f,1.0f };			//WINスプライトのデフォルト拡大率

	const int SPRITE_WIN_MOVETIME_FAST = 6;									//WINスプライトがすばやく動く時の移動時間
	const int SPRITE_WIN_MOVETIME_LATE = 12;								//WINスプライトがゆっくり動く時の移動時間
	const int SPRITE_WIN_MOVETIME_WAIT = 90;								//WINスプライトの待機時間

	const int SPRITE_WIN_MOVEDELAY_START = 0;								//WINスプライトのスタートディレイ
	const int SPRITE_WIN_MOVEDELAY_VERTICAL_MIDDLE = 12;					//WINスプライトが縦に伸ばされるディレイ
	const int SPRITE_WIN_MOVEDELAY_HORIZONTAL_HEAVY = 12;					//WINスプライトが横に大きく伸ばされるディレイ
	const int SPRITE_WIN_MOVEDELAY_VERTICAL_HEAVY = 18;						//WINスプライトが縦に大きく伸ばされるディレイ
	const int SPRITE_WIN_MOVEDELAY_HORIZONTAL_MIDDLE = 30;					//WINスプライトが横に伸ばされるディレイ
	const int SPRITE_WIN_MOVEDELAY_VERTICAL_LIGHT = 36;						//WINスプライトが縦に小さく伸ばされるディレイ
	const int SPRITE_WIN_MOVEDELAY_TO_DEFAULT = 42;							//WINスプライトの大きさが元に戻るディレイ


	const Vector2 SPRITE_WIN_FALL_MOVEMENT_LIGHT = { 0.0f,-10.0f };			//WINスプライトが少し落下する移動量
	const Vector2 SPRITE_WIN_FALL_MOVEMENT_HEAVY = { 0.0f,-60.0f };			//WINスプライトが大きく落下する移動量
	const Vector2 SPRITE_WIN_RISE_MOVEMENT_LIGHT = { 0.0f,20.0f };			//WINスプライトが少し上昇する移動量
	const Vector2 SPRITE_WIN_RISE_MOVEMENT_HEAVY = { 0.0f,60.0f };			//WINスプライトが大きく上昇する移動量
	const Vector2 SPRITE_WIN_WAIT_MOVEMENT = { 0.0f,0.0f };					//WINスプライトが待機する時の移動量

	const Vector2 SPRITE_COMMAND_UP_MOVEMENT = { 0.0f,5.0f };				//コマンドスプライトが上に移動する時の移動量
	const Vector2 SPRITE_COMMAND_DOWN_MOVEMENT = { 0.0f,-5.0f };			//コマンドスプライトが下に移動する時の移動量
	const int SPRITE_COMMAND_UP_MOVETIME = 6;								//コマンドスプライトが上に移動する時の移動時間
	const int SPRITE_COMMAND_DOWN_MOVETIME = 6;								//コマンドスプライトが下に移動する時の移動時間
	const int SPRITE_COMMAND_UP_MOVEDELAY = 0;								//コマンドスプライトが上に移動する時のディレイ
	const int SPRITE_COMMAND_DOWN_MOVEDELAY = 6;							//コマンドスプライトが下に移動する時のディレイ

	const int TRANSITION_TIME_TO_TITLE = 15;								//タイトルへ移行する時のトランジション時間
	const int TRANSITION_TIME_RETRY = 2;									//リトライ時のトランジション時間
}

ResultScene::~ResultScene()
{	
	DeleteGO(m_Down_SpriteRender);
	DeleteGO(m_Up_SpriteRender);
	DeleteGO(m_Under_SpriteRender);
	DeleteGO(m_Lose_SpriteRender);
	DeleteGO(m_Win_SpriteRender);
	DeleteGO(m_Command_SpriteRender);
	DeleteGO(m_BG_SpriteRender);
	DeleteGO(m_P1_skinModelRender);
	DeleteGO(m_P2_skinModelRender);
	DeleteGO(m_directionLight);
}			 
bool ResultScene::Start()
{	
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, true);

	//背景
	m_BG_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_BG_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::Screen1);
	m_BG_SpriteRender->SetPosition(Vector3::Zero);
	m_BG_SpriteRender->Init("Assets/Image/TitleBG.dds", SPRITE_BACKGROUND_WIDTH, SPRITE_BACKGROUND_HEIGHT);
	//モデルの描画前にスプライトを描画できるようにする。
	m_BG_SpriteRender->SetPostRenderMode(false);

	m_Up_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Up_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::Screen1);
	m_Up_SpriteRender->Init("Assets/Image/Result_Up.dds", SPRITE_COVER_UPPER_WIDTH, SPRITE_COVER_UPPER_HEIGHT);
	//モデルの描画前にスプライトを描画できるようにする。
	m_Up_SpriteRender->SetPostRenderMode(false);
	m_Up_SpriteRender->SetPosition(SPRITE_COVER_UPPER_STARTPOSITION);

	m_Under_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Under_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)0);
	m_Under_SpriteRender->Init("Assets/Image/Result_Under.dds", SPRITE_RESULT_WINDOW_WIDTH, SPRITE_RESULT_WINDOW_HEIGHT);
	//モデルの描画前にスプライトを描画できるようにする。
	m_Under_SpriteRender->SetPostRenderMode(false);
	m_Under_SpriteRender->SetPosition(Vector3::Zero);
	m_Under_SpriteRender->SetMulColor(SPRITE_COLOR_TRANSPARENT);

	m_Lose_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Lose_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Lose_SpriteRender->Init("Assets/Image/Result_Lose.dds", SPRITE_LOSE_WIDTH, SPRITE_LOSE_HEIGHT);
	m_Lose_SpriteRender->SetPosition(SPRITE_LOSER_POSITION);
	m_Lose_SpriteRender->SetMulColor(SPRITE_COLOR_TRANSPARENT);

	m_Win_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Win_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Win_SpriteRender->Init("Assets/Image/Result_Win.dds", SPRITE_WIN_WIDTH, SPRITE_WIN_HEIGHT);
	m_Win_SpriteRender->SetPosition(SPRITE_WINNER_POSITION);
	m_Win_SpriteRender->SetMulColor(SPRITE_COLOR_TRANSPARENT);

	m_Command_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Command_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Command_SpriteRender->Init("Assets/Image/Result_Command_Retry.dds", SPRITE_COMMAND_WIDTH, SPRITE_COMMAND_HEIGHT);
	m_Command_SpriteRender->SetPosition({ SPRITE_COMMAND_POSITION.x,SPRITE_COMMAND_POSITION.y - SPRITE_COVER_MOVE_AMOUNT,SPRITE_COMMAND_POSITION.z });
	m_Command_SpriteRender->SetMulColor(SPRITE_COLOR_BLACK);

	//勝者、敗者のモデル
	m_animationClips[enAnimationClip_Win].Load("Assets/animData/Mage_SpecialAttack2.tka");
	m_animationClips[enAnimationClip_Win].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Lose].Load("Assets/animData/Mage_Stunned.tka");
	m_animationClips[enAnimationClip_Lose].SetLoopFlag(true);
	m_P1_skinModelRender = NewGO<prefab::CSkinModelRender>(3);
	m_P1_skinModelRender->Init("Assets/modelData/Player1.tkm", "Assets/modelData/Mage.tks",m_animationClips,enAnimationClip_Num);
	m_P2_skinModelRender = NewGO<prefab::CSkinModelRender>(3);
	m_P2_skinModelRender->Init("Assets/modelData/Player2.tkm", "Assets/modelData/Mage.tks", m_animationClips, enAnimationClip_Num);
	//勝ったプレイヤーで分岐
	if (m_loserNum == NUMBER_PLAYER1)
	{
		m_P1_skinModelRender->SetPosition(MODEL_LOSER_POSITION);
		m_P2_skinModelRender->SetPosition(MODEL_WINNER_POSITION);
	}
	else
	{
		m_P1_skinModelRender->SetPosition(MODEL_WINNER_POSITION);
		m_P2_skinModelRender->SetPosition(MODEL_LOSER_POSITION);
	}

	m_directionLight = NewGO<prefab::CDirectionLight>(0);
	m_directionLight->SetDirection(DIRECTIONLIGHT_RESULT_DIRECTION);
	m_directionLight->SetColor(DIRECTIONLIGHT_COLOR_WHITE);

	m_Down_SpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_Down_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Down_SpriteRender->Init("Assets/Image/Result_Down.dds", SPRITE_COVER_LOWER_WIDTH, SPRITE_COVER_LOWER_HEIGHT);
	m_Down_SpriteRender->SetPosition(SPRITE_COVER_LOWER_STARTPOSITION);

	g_camera3D[0]->SetPosition(RESULT_CAMERA_POSITION);
	g_camera3D[0]->SetTarget(RESULT_CAMERA_TARGET);

	return true;
}
void ResultScene::Update()
{
	if (m_loserNum == NUMBER_PLAYER1)
	{
		m_P1_skinModelRender->PlayAnimation(enAnimationClip_Lose);
		m_P2_skinModelRender->PlayAnimation(enAnimationClip_Win);
	}
	else
	{
		m_P1_skinModelRender->PlayAnimation(enAnimationClip_Win);
		m_P2_skinModelRender->PlayAnimation(enAnimationClip_Lose);
	}

	if (m_moveEndFlag == false) {
		if (m_moveTimer == SPRITE_MOVETIMER_START) {
			//移動予約
			m_Up_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_UP_CLOSE_MOVEMENT, SPRITE_CLOSE_MOVETIME, SPRITE_CLOSE_MOVEDELAY, true);
			m_Up_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_UP_CLOSE_REBOUND_UP_MOVEMENT, SPRITE_CLOSE_REBOUND_MOVETIME, SPRITE_CLOSE_REBOUND_UP_MOVEDELAY, true);
			m_Up_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_UP_CLOSE_REBOUND_DOWN_MOVEMENT, SPRITE_CLOSE_REBOUND_MOVETIME, SPRITE_CLOSE_REBOUND_DOWN_MOVEDELAY, true);

			m_Down_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_DOWN_CLOSE_MOVEMENT, SPRITE_CLOSE_MOVETIME, SPRITE_CLOSE_MOVEDELAY, true);
			m_Down_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_DOWN_CLOSE_REBOUND_UP_MOVEMENT, SPRITE_CLOSE_REBOUND_MOVETIME, SPRITE_CLOSE_REBOUND_UP_MOVEDELAY, true);
			m_Down_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_DOWN_CLOSE_REBOUND_DOWN_MOVEMENT, SPRITE_CLOSE_REBOUND_MOVETIME, SPRITE_CLOSE_REBOUND_DOWN_MOVEDELAY, true);

			m_Command_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_COMMAND_CLOSE_MOVEMENT, SPRITE_CLOSE_MOVETIME, SPRITE_CLOSE_MOVEDELAY, true);
			m_Command_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_COMMAND_CLOSE_REBOUND_DOWN_MOVEMENT, SPRITE_CLOSE_REBOUND_MOVETIME, SPRITE_CLOSE_REBOUND_UP_MOVEDELAY, true);
			m_Command_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_COMMAND_CLOSE_REBOUND_UP_MOVEMENT, SPRITE_CLOSE_REBOUND_MOVETIME, SPRITE_CLOSE_REBOUND_DOWN_MOVEDELAY, true);

		}
		else if (m_moveTimer == SPRITE_MOVETIMER_DISPLAY_AND_SHAKE) {
			//窓と、文字系スプライトの表示
			m_Under_SpriteRender->GetSpriteSupporter().SpriteColor(Vector4::White, SPRITE_APPEAR_TIME, SPRITE_MOVEDELAY_ZERO);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteColor(Vector4::White, SPRITE_APPEAR_TIME, SPRITE_MOVEDELAY_ZERO);
			m_Lose_SpriteRender->GetSpriteSupporter().SpriteColor(Vector4::White, SPRITE_APPEAR_TIME, SPRITE_MOVEDELAY_ZERO);
			m_Command_SpriteRender->GetSpriteSupporter().SpriteColor(Vector4::White, SPRITE_APPEAR_TIME, SPRITE_MOVEDELAY_ZERO);
			
			//Loseスプライトにこれ以降ずっとシェイクを指示
			m_Lose_SpriteRender->GetSpriteSupporter().SpriteShake(SPRITE_LOSE_SHAKE_MOVEMENT, SPRITE_LOSE_SHAKE_MOVETIME, SPRITE_MOVEDELAY_ZERO);

			//これ以降winの文字を動かすようにフラグを設定
			m_win_lose_MoveFlag = true;
		}

		//SE
		if (m_moveTimer == SPRITE_MOVETIMER_PLAY_SE1) {
			SoundOneShotPlay(L"Assets/sound/リザルトSE1.wav", SOUND_SE_RESULT_VOLUME);
		}
		else if (m_moveTimer == SPRITE_MOVETIMER_PLAY_SE2)
		{
			SoundOneShotPlay(L"Assets/sound/リザルトSE1.wav", SOUND_SE_RESULT_VOLUME);
		}
		//BGM
		else if (m_moveTimer == SPRITE_MOVETIMER_PLAY_BGM)
		{	
			m_resultBGM = NewGO<prefab::CSoundSource>(0);;
			m_resultBGM->Init(L"Assets/sound/リザルト曲.wav", SoundType::enBGM);
			m_resultBGM->SetVolume(SOUND_BGM_RESULT_VOLUME);
			m_resultBGM->Play(true);
		}

		m_moveTimer++;
		if (m_moveTimer >= SPRITE_MOVETIMER_END) {
			m_moveEndFlag = true;
		}
	}
	else if(m_moveEndFlag == true){
		//上か下ボタンを押すとリトライと、タイトルが切り替える
		if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonUp) || g_pad[PAD_PLAYER1]->IsTrigger(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp)) {
			if (m_RetryOn == true)
			{
				m_Command_SpriteRender->Init("Assets/Image/Result_Command_Title.dds", SPRITE_COMMAND_WIDTH, SPRITE_COMMAND_HEIGHT);
				m_Command_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_COMMAND_UP_MOVEMENT, SPRITE_COMMAND_UP_MOVETIME, SPRITE_COMMAND_UP_MOVEDELAY, true);
				m_Command_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_COMMAND_DOWN_MOVEMENT, SPRITE_COMMAND_DOWN_MOVETIME, SPRITE_COMMAND_DOWN_MOVEDELAY, true);
				m_RetryOn = false;

				//SE
				SoundOneShotPlay(L"Assets/sound/カーソル移動音.wav");
			}
			else
			{
				m_Command_SpriteRender->Init("Assets/Image/Result_Command_Retry.dds", SPRITE_COMMAND_WIDTH, SPRITE_COMMAND_HEIGHT);
				m_Command_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_COMMAND_UP_MOVEMENT, SPRITE_COMMAND_UP_MOVETIME, SPRITE_COMMAND_UP_MOVEDELAY, true);
				m_Command_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_COMMAND_DOWN_MOVEMENT, SPRITE_COMMAND_DOWN_MOVETIME, SPRITE_COMMAND_DOWN_MOVEDELAY, true);
				m_RetryOn = true;

				//SE
				SoundOneShotPlay(L"Assets/sound/カーソル移動音.wav");
			}
		}

		//スタートでAボタンを押すとキャラ選択画面に遷移する
		if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA)) {

			if (m_RetryOn == true) {
				DeleteGO(m_resultBGM);
				//SE
				SoundOneShotPlay(L"Assets/sound/リザルト画面決定音.wav");

				//次の試合のラウンドの計測を開始
				NewGO<RoundCounter>(0, "roundcounter");

				//トランジション
				TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_RETRY, false);
				GameScene* gameScene = NewGO<GameScene>(0, "gamescene");

				DeleteGO(this);
			}
			if (m_RetryOn == false) {
				DeleteGO(m_resultBGM);
				//SE
				SoundOneShotPlay(L"Assets/sound/リザルト画面決定音.wav");

				//トランジション
				TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::Circle, TRANSITION_TIME_TO_TITLE, false);
				TitleScene* titlescene = NewGO<TitleScene>(0, "titlescene");
				DeleteGO(this);
			}
		}
	}

	if (m_win_lose_MoveFlag == true) {

		if (m_Win_SpriteRender->GetSpriteSupporter().GetSpriteMoveListLen() == SPRITEMOVELIST_ZERO) {
			m_Win_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_WIN_FALL_MOVEMENT_LIGHT, SPRITE_WIN_MOVETIME_LATE, SPRITE_WIN_MOVEDELAY_START, true);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_WIN_HORIZONTAL_SCALE_MIDDLE, SPRITE_WIN_MOVETIME_LATE, SPRITE_WIN_MOVEDELAY_START);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_WIN_RISE_MOVEMENT_HEAVY, SPRITE_WIN_MOVETIME_LATE, SPRITE_WIN_MOVEDELAY_VERTICAL_MIDDLE, true);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_WIN_VERTICAL_SCALE_MIDDLE, SPRITE_WIN_MOVETIME_LATE, SPRITE_WIN_MOVEDELAY_VERTICAL_MIDDLE);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_WIN_FALL_MOVEMENT_HEAVY, SPRITE_WIN_MOVETIME_FAST, SPRITE_WIN_MOVEDELAY_HORIZONTAL_HEAVY, true);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_WIN_HORIZONTAL_SCALE_HEAVY, SPRITE_WIN_MOVETIME_FAST, SPRITE_WIN_MOVEDELAY_HORIZONTAL_HEAVY);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_WIN_RISE_MOVEMENT_LIGHT, SPRITE_WIN_MOVETIME_LATE, SPRITE_WIN_MOVEDELAY_VERTICAL_HEAVY, true);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_WIN_VERTICAL_SCALE_HEAVY, SPRITE_WIN_MOVETIME_LATE, SPRITE_WIN_MOVEDELAY_VERTICAL_HEAVY);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteMove({ SPRITE_WINNER_POSITION.x,SPRITE_WINNER_POSITION.y }, SPRITE_WIN_MOVETIME_FAST, SPRITE_WIN_MOVEDELAY_HORIZONTAL_MIDDLE);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_WIN_HORIZONTAL_SCALE_MIDDLE, SPRITE_WIN_MOVETIME_FAST, SPRITE_WIN_MOVEDELAY_HORIZONTAL_MIDDLE);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_WIN_WAIT_MOVEMENT, SPRITE_WIN_MOVETIME_WAIT, SPRITE_WIN_MOVEDELAY_VERTICAL_LIGHT, true);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_WIN_VERTICAL_SCALE_LIGHT, SPRITE_WIN_MOVETIME_FAST, SPRITE_WIN_MOVEDELAY_VERTICAL_LIGHT);
			m_Win_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_WIN_SCALE_DEFAULT, SPRITE_WIN_MOVETIME_FAST, SPRITE_WIN_MOVEDELAY_TO_DEFAULT);
		}

	}

}
