#include "stdafx.h"
#include "RoundCounter.h"

RoundCounter::RoundCounter()
{

}

RoundCounter::~RoundCounter()
{
	DeleteGO(tmp_playerRoundFont[0]);
	DeleteGO(tmp_playerRoundFont[1]);
}

bool RoundCounter::Start()
{
	tmp_playerRoundFont[0] = NewGO<prefab::CFontRender>(7);
	tmp_playerRoundFont[0]->SetPosition({ -200.0f,200.0f });
	tmp_playerRoundFont[0]->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);
	tmp_playerRoundFont[1] = NewGO<prefab::CFontRender>(7);
	tmp_playerRoundFont[1]->SetPosition({ 200.0f,200.0f });
	tmp_playerRoundFont[1]->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);

	return true;
}

void RoundCounter::Update()
{
	tmp_playerRoundFont[0]->SetText(std::to_wstring(m_playerTakeRound[0]));
	tmp_playerRoundFont[1]->SetText(std::to_wstring(m_playerTakeRound[1]));
}

int RoundCounter::GetOverAllWinner()
{
	if (m_playerTakeRound[NUMBER_PLAYER1] == 2)
	{
		//�v���C���[1��2�{����Ă����珟�҂̓v���C���[1
		return NUMBER_PLAYER1;
	}
	else if (m_playerTakeRound[NUMBER_PLAYER2] == 2)
	{
		//�v���C���[2��2�{����Ă����珟�҂̓v���C���[2
		return NUMBER_PLAYER2;
	}
	else
	{
		//�ǂ���̃v���C���[��2�{����Ă��Ȃ��̂ŏ��Җ��m��
		return -1;
	}
}
