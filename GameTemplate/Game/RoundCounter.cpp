#include "stdafx.h"
#include "RoundCounter.h"

namespace
{
	const int ROUND_ONE = 0;
	const int ROUND_TWO = 1;
	const int ROUND_WIN = 2;
	const Vector3 EFFECT_TWINKLE_SCALE = { 30.0f,30.0f,30.0f };
	const int SPRITE_ROUND_WIDTH = 64;
	const int SPRITE_ROUND_HEIGHT = 64;
	const Vector3 SPRITE_PLAYER1_ROUND1_GAME_POSITION = { -140.0f,260.0f,0.0f };
	const Vector3 SPRITE_PLAYER1_ROUND2_GAME_POSITION = { -190.0f,260.0f,0.0f };
	const Vector3 SPRITE_PLAYER2_ROUND1_GAME_POSITION = { 140.0f,260.0f,0.0f };
	const Vector3 SPRITE_PLAYER2_ROUND2_GAME_POSITION = { 190.0f,260.0f,0.0f };
	const Vector3 SPRITE_PLAYER1_ROUND1_RESULT_POSITION = { -690.0f, -300.0f, 0.0f };
	const Vector3 SPRITE_PLAYER1_ROUND2_RESULT_POSITION = { -740.0f,-300.0f,0.0f };
	const Vector3 SPRITE_PLAYER2_ROUND1_RESULT_POSITION = { 690.0f,-300.0f,0.0f };
	const Vector3 SPRITE_PLAYER2_ROUND2_RESULT_POSITION = { 740.0f,-300.0f,0.0f };
	const Vector3 SPRITE_PLAYER1_SIDE_TO_CENTER = { 200.0f,0.0f,0.0f };
	const Vector3 SPRITE_PLAYER2_SIDE_TO_CENTER = { -200.0f,0.0f,0.0f };
	const int SPRITE_RESULT_MOVETIME = 25;
	const int SPRITE_RESULT_MOVEDELAY = 0;
	const int ROUNDRESULTCOUNT_START = 0;
	const int ROUNDRESULTCOUNT_SPRITEMOVE = 20;
	const int ROUNDRESULTCOUNT_UPDATEROUND = 100;
}

RoundCounter::~RoundCounter()
{
	//スプライトの削除
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]);
}

bool RoundCounter::Start()
{
	//プレイヤー1のラウンド1のスプライト
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION);

	//プレイヤー1のラウンド2のスプライト
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->SetPosition(SPRITE_PLAYER1_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	//プレイヤー2のラウンド1のスプライト
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	//プレイヤー2のラウンド2のスプライト
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->SetPosition(SPRITE_PLAYER2_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	//ラウンド取得時の2Dエフェクト
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
			m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER1_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER1_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER2_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER2_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
		}

		//ラウンド取得表示
		if (m_roundResultCount == ROUNDRESULTCOUNT_UPDATEROUND)
		{
			//最後にラウンドを取得したプレイヤー番号で分岐
			if (m_latestRoundWinner == NUMBER_PLAYER1)
			{
				//ライトのスプライトを光っているものに
				m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->Init(
					"Assets/Image/LightGreen.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT
				);
			}
			else
			{
				//ライトのスプライトを光っているものに
				m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->Init(
					"Assets/Image/LightYellow.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT
				);
			}

			//光るエフェクトを再生するのは先程光らせたスプライトの座標
			Vector3 twinklePos = m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->GetPosition();
			
			//プレイヤーの取得ラウンド変数をインクリメント
			m_playerTakeRound[m_latestRoundWinner]++;

			//エフェクトの再生
			m_roundGetEffect->SetPosition(twinklePos);
			m_roundGetEffect->Play();

			SoundOneShotPlay(L"Assets/sound/RoundGet.wav");

			//カウントリセット
			m_roundResultCount = ROUNDRESULTCOUNT_START;

			//結果表示終了
			m_isResult = false;

			m_currentRound++;
			m_roundAlreadyAnnounce = false;
		}
	}
}

int RoundCounter::GetOverAllWinner()
{
	if (m_playerTakeRound[NUMBER_PLAYER1] == ROUND_WIN)
	{
		//プレイヤー1が2本取っていたら勝者はプレイヤー1
		return NUMBER_PLAYER1;
	}
	else if (m_playerTakeRound[NUMBER_PLAYER2] == ROUND_WIN)
	{
		//プレイヤー2が2本取っていたら勝者はプレイヤー2
		return NUMBER_PLAYER2;
	}
	else
	{
		//どちらのプレイヤーも2本取っていないので勝者未確定
		return -1;
	}
}

void RoundCounter::EnableGameRound()
{
	//全てのスプライトのスケールを1にして表示
	for (auto roundSprite : m_gameRoundSprite)
	{
		roundSprite[ROUND_ONE]->SetScale(Vector3::One);
		roundSprite[ROUND_TWO]->SetScale(Vector3::One);
	}

	//各スプライトをゲーム画面位置に設定
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->SetPosition(SPRITE_PLAYER1_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->SetPosition(SPRITE_PLAYER2_ROUND2_GAME_POSITION);
}

void RoundCounter::EnableResultRound()
{
	//全てのスプライトのスケールを1にして表示
	for (auto roundSprite : m_gameRoundSprite)
	{
		roundSprite[0]->SetScale(Vector3::One);
		roundSprite[1]->SetScale(Vector3::One);
	}

	//各スプライトを結果画面の位置(左右画面外)に設定
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->SetPosition(SPRITE_PLAYER1_ROUND1_RESULT_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->SetPosition(SPRITE_PLAYER1_ROUND2_RESULT_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->SetPosition(SPRITE_PLAYER2_ROUND1_RESULT_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->SetPosition(SPRITE_PLAYER2_ROUND2_RESULT_POSITION);

	m_isResult = true;
}

void RoundCounter::RoundAnnounce()
{
	if (m_roundAlreadyAnnounce == true)
	{
		return;
	}

	switch (m_currentRound)
	{
	case 0:
		SoundOneShotPlay(L"Assets/sound/Round1.wav",3.0f);
		break;
	case 1:
		SoundOneShotPlay(L"Assets/sound/Round2.wav",3.0f);
		break;
	case 2:
		SoundOneShotPlay(L"Assets/sound/RoundFinal.wav",3.0f);
		break;
	}

	m_roundAlreadyAnnounce = true;
}