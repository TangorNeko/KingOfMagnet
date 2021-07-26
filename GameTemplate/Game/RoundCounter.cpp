#include "stdafx.h"
#include "RoundCounter.h"

namespace
{
	const Vector3 EFFECT_TWINKLE_SCALE = { 30.0f,30.0f,30.0f };							//ラウンド取得エフェクトの拡大率
	const int SPRITE_ROUND_WIDTH = 64;													//ラウンドランプスプライトの幅
	const int SPRITE_ROUND_HEIGHT = 64;													//ラウンドランプスプライトの高さ
	const Vector3 SPRITE_PLAYER1_ROUND1_GAME_POSITION = { -140.0f,260.0f,0.0f };		//プレイヤー1のゲーム中のラウンド1スプライトの座標
	const Vector3 SPRITE_PLAYER2_ROUND1_GAME_POSITION = { 140.0f,260.0f,0.0f };			//プレイヤー2のゲーム中のラウンド1スプライトの座標
	const Vector3 SPRITE_PLAYER1_ROUND1_RESULT_POSITION = { -690.0f, -300.0f, 0.0f };	//プレイヤー1の結果画面のラウンド1スプライトの座標
	const Vector3 SPRITE_PLAYER2_ROUND1_RESULT_POSITION = { 690.0f,-300.0f,0.0f };		//プレイヤー2の結果画面のラウンド1スプライトの座標
	const Vector3 SPRITE_PLAYER1_SIDE_TO_CENTER = { 200.0f,0.0f,0.0f };					//プレイヤー1の結果画面のラウンドスプライトがスライドしてくる時の移動量
	const Vector3 SPRITE_PLAYER2_SIDE_TO_CENTER = { -200.0f,0.0f,0.0f };				//プレイヤー2の結果画面のラウンドスプライトがスライドしてくる時の移動量
	const Vector3 SPRITE_PLAYER1_OFFSET = { -50.0f,0.0f,0.0f };							//プレイヤー1の隣のラウンドスプライトとの空間
	const Vector3 SPRITE_PLAYER2_OFFSET = { 50.0f,0.0f,0.0f };							//プレイヤー2の隣のラウンドスプライトとの空間
	const int SPRITE_RESULT_MOVETIME = 25;												//結果画面のラウンドスプライトの移動時間
	const int SPRITE_RESULT_MOVEDELAY = 0;												//結果画面のラウンドスプライトの移動ディレイ
	const int ROUNDRESULTCOUNT_START = 0;												//結果画面のラウンドスプライトの移動タイマー　スタート
	const int ROUNDRESULTCOUNT_SPRITEMOVE = 20;											//結果画面のラウンドスプライトの移動タイマー　移動開始
	const int ROUNDRESULTCOUNT_UPDATEROUND = 100;										//結果画面のラウンドスプライトの移動タイマー　ラウンド取得表示

	//共通になりそう
	const float SOUND_SE_VOICE_VOLUME = 3.0f;											//ボイスSEのボリューム
}

int RoundCounter::m_maxRound = 1;
int RoundCounter::m_roundToWin = 1;

RoundCounter::~RoundCounter()
{
	//スプライトの削除
	for (int i = 0;i < m_roundToWin;i++)
	{
		DeleteGO(m_player1Sprites.at(i));
		DeleteGO(m_player2Sprites.at(i));
	}
}

bool RoundCounter::Start()
{
	//スプライトの動的配列のサイズを確保
	m_player1Sprites.reserve(m_roundToWin);
	m_player2Sprites.reserve(m_roundToWin);

	//表示スプライトの数分各プレイヤーのラウンド表示スプライトを作成
	for (int i = 0;i < m_roundToWin;i++)
	{
		prefab::CSpriteRender* roundSprite = NewGO<prefab::CSpriteRender>(0);
		roundSprite->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
		roundSprite->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION + (SPRITE_PLAYER1_OFFSET * i));
		m_player1Sprites.push_back(roundSprite);

		roundSprite = NewGO<prefab::CSpriteRender>(0);
		roundSprite->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
		roundSprite->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION + (SPRITE_PLAYER2_OFFSET * i));
		m_player2Sprites.push_back(roundSprite);
	}
	
	//ラウンド取得時の2Dエフェクトの初期化
	m_roundGetEffect = NewGO<prefab::CEffect2D>(0);
	m_roundGetEffect->Init(u"Assets/effect/Twinkle.efk");
	m_roundGetEffect->SetScale(EFFECT_TWINKLE_SCALE);
	return true;
}

void RoundCounter::Update()
{
	//結果表示画面?
	if (m_isResult == true)
	{
		//カウントをインクリメント
		m_roundResultCount++;

		//画面外からスッと飛び出してくる移動をセット
		if (m_roundResultCount == ROUNDRESULTCOUNT_SPRITEMOVE)
		{
			for (auto player1Sprite : m_player1Sprites)
			{
				player1Sprite->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER1_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			}

			for (auto player2Sprite : m_player2Sprites)
			{
				player2Sprite->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER2_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			}
		}

		//ラウンド取得表示
		if (m_roundResultCount == ROUNDRESULTCOUNT_UPDATEROUND)
		{
			//光るエフェクトを再生する座標
			Vector3 twinklePos;

			//最後にラウンドを取得したプレイヤー番号で分岐
			if (m_latestRoundWinner == NUMBER_PLAYER1)
			{
				//ライトのスプライトを光っているものに
				prefab::CSpriteRender* twinkleSprite = m_player1Sprites.at(m_playerTakeRound[NUMBER_PLAYER1]);
				twinkleSprite->Init("Assets/Image/LightGreen.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
				
				//光るエフェクトを再生するのは光らせたスプライトの座標
				twinklePos = twinkleSprite->GetPosition();
			}
			else
			{
				//ライトのスプライトを光っているものに
				prefab::CSpriteRender* twinkleSprite = m_player2Sprites.at(m_playerTakeRound[NUMBER_PLAYER2]);
				twinkleSprite->Init("Assets/Image/LightYellow.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
				
				//光るエフェクトを再生するのは光らせたスプライトの座標
				twinklePos = twinkleSprite->GetPosition();
			}

			//プレイヤーの取得ラウンド変数をインクリメント
			m_playerTakeRound[m_latestRoundWinner]++;

			//エフェクトの再生
			m_roundGetEffect->SetPosition(twinklePos);
			m_roundGetEffect->Play();

			//ラウンド取得の音を再生
			SoundOneShotPlay(L"Assets/sound/RoundGet.wav");

			//カウントリセット
			m_roundResultCount = ROUNDRESULTCOUNT_START;

			//結果表示終了
			m_isResult = false;

			//ラウンド数をインクリメント
			m_currentRound++;

			//ラウンド数のアナウンスフラグをリセット
			m_roundAlreadyAnnounce = false;
		}
	}
}

int RoundCounter::GetOverAllWinner()
{
	if (m_playerTakeRound[NUMBER_PLAYER1] == m_roundToWin)
	{
		//プレイヤー1が必要ラウンド数取っていたら勝者はプレイヤー1
		return NUMBER_PLAYER1;
	}
	else if (m_playerTakeRound[NUMBER_PLAYER2] == m_roundToWin)
	{
		//プレイヤー2が必要ラウンド数取っていたら勝者はプレイヤー2
		return NUMBER_PLAYER2;
	}
	else
	{
		//どちらのプレイヤーも必要ラウンド数取っていないので勝者未確定
		return -1;
	}
}

void RoundCounter::MoveToGamePosition()
{
	//各スプライトをゲーム画面位置に設定
	for (int i = 0;i < m_roundToWin;i++)
	{
		//プレイヤー1のスプライト
		m_player1Sprites.at(i)->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION + (SPRITE_PLAYER1_OFFSET * i));

		//プレイヤー2のスプライト
		m_player2Sprites.at(i)->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION + (SPRITE_PLAYER2_OFFSET * i));
	}
}

void RoundCounter::MoveToResultPosition()
{
	//各スプライトを結果表示位置に設定
	for (int i = 0;i < m_roundToWin;i++)
	{
		//プレイヤー1のスプライト
		m_player1Sprites.at(i)->SetPosition(SPRITE_PLAYER1_ROUND1_RESULT_POSITION + (SPRITE_PLAYER1_OFFSET * i));

		//プレイヤー2のスプライト
		m_player2Sprites.at(i)->SetPosition(SPRITE_PLAYER2_ROUND1_RESULT_POSITION + (SPRITE_PLAYER2_OFFSET * i));
	}
}

void RoundCounter::StartResultMove()
{
	//リザルト表示の開始フラグをオンに
	m_isResult = true;
}

void RoundCounter::RoundAnnounce()
{
	//すでにアナウンスしていたらreturn
	if (m_roundAlreadyAnnounce == true)
	{
		return;
	}

	//現在のラウンド数に応じてボイスを再生
	switch (m_currentRound)
	{
	case 0:
		SoundOneShotPlay(L"Assets/sound/Round1.wav", SOUND_SE_VOICE_VOLUME);
		break;
	case 1:
		SoundOneShotPlay(L"Assets/sound/Round2.wav", SOUND_SE_VOICE_VOLUME);
		break;
	case 2:
		SoundOneShotPlay(L"Assets/sound/RoundFinal.wav", SOUND_SE_VOICE_VOLUME);
		break;
	}

	//すでにこのラウンドはアナウンスをしたフラグをオン
	m_roundAlreadyAnnounce = true;
}