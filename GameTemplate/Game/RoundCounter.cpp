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
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]);
}

bool RoundCounter::Start()
{
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION);

	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->SetPosition(SPRITE_PLAYER1_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->SetPosition(SPRITE_PLAYER2_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	m_roundGetEffect = NewGO<prefab::CEffect2D>(0);
	m_roundGetEffect->Init(u"Assets/effect/Twinkle.efk");
	m_roundGetEffect->SetScale(EFFECT_TWINKLE_SCALE);
	return true;
}

void RoundCounter::Update()
{
	if (m_isResult == true)
	{
		m_roundResultCount++;

		if (m_roundResultCount == ROUNDRESULTCOUNT_SPRITEMOVE)
		{
			m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER1_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER1_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER2_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER2_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
		}

		if (m_roundResultCount == ROUNDRESULTCOUNT_UPDATEROUND)
		{
			if (m_latestRoundWinner == NUMBER_PLAYER1)
			{
				m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->Init(
					"Assets/Image/LightGreen.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT
				);
			}
			else
			{
				m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->Init(
					"Assets/Image/LightYellow.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT
				);
			}

			Vector3 twinklePos = m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->GetPosition();
			
			m_playerTakeRound[m_latestRoundWinner]++;
			m_roundGetEffect->SetPosition(twinklePos);
			m_roundGetEffect->Play();
			m_roundResultCount = ROUNDRESULTCOUNT_START;
			m_isResult = false;
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
	for (auto roundSprite : m_gameRoundSprite)
	{
		roundSprite[ROUND_ONE]->SetScale(Vector3::One);
		roundSprite[ROUND_TWO]->SetScale(Vector3::One);
	}
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->SetPosition(SPRITE_PLAYER1_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->SetPosition(SPRITE_PLAYER2_ROUND2_GAME_POSITION);
}

void RoundCounter::EnableResultRound()
{
	for (auto roundSprite : m_gameRoundSprite)
	{
		roundSprite[0]->SetScale(Vector3::One);
		roundSprite[1]->SetScale(Vector3::One);
	}

	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->SetPosition(SPRITE_PLAYER1_ROUND1_RESULT_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->SetPosition(SPRITE_PLAYER1_ROUND2_RESULT_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->SetPosition(SPRITE_PLAYER2_ROUND1_RESULT_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->SetPosition(SPRITE_PLAYER2_ROUND2_RESULT_POSITION);

	m_isResult = true;
}
