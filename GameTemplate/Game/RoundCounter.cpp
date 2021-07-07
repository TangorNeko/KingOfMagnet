#include "stdafx.h"
#include "RoundCounter.h"

RoundCounter::~RoundCounter()
{
	DeleteGO(tmp_gameRoundFont[0]);
	DeleteGO(tmp_gameRoundFont[1]);
	DeleteGO(tmp_resultRoundFont[0]);
	DeleteGO(tmp_resultRoundFont[1]);
}

bool RoundCounter::Start()
{
	tmp_gameRoundFont[0] = NewGO<prefab::CFontRender>(7);
	tmp_gameRoundFont[0]->SetPosition({ -200.0f,260.0f });
	tmp_gameRoundFont[0]->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);
	tmp_gameRoundFont[1] = NewGO<prefab::CFontRender>(7);
	tmp_gameRoundFont[1]->SetPosition({ 200.0f,260.0f });
	tmp_gameRoundFont[1]->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);

	tmp_resultRoundFont[0] = NewGO<prefab::CFontRender>(7);
	tmp_resultRoundFont[0]->SetPosition({ -500.0f,-250.0f });
	tmp_resultRoundFont[0]->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);
	tmp_resultRoundFont[1] = NewGO<prefab::CFontRender>(7);
	tmp_resultRoundFont[1]->SetPosition({ 500.0f,-250.0f });
	tmp_resultRoundFont[1]->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);

	tmp_roundEffect = NewGO<prefab::CEffect2D>(0);
	tmp_roundEffect->Init(u"Assets/effect/Twinkle.efk");
	tmp_roundEffect->SetScale({ 20.0f,20.0f, 20.0f});
	return true;
}

void RoundCounter::Update()
{
	tmp_gameRoundFont[0]->SetText(std::to_wstring(m_playerTakeRound[0]));
	tmp_gameRoundFont[1]->SetText(std::to_wstring(m_playerTakeRound[1]));

	tmp_resultRoundFont[0]->SetText(std::to_wstring(m_playerTakeRound[0]));
	tmp_resultRoundFont[1]->SetText(std::to_wstring(m_playerTakeRound[1]));

	if (m_isResult == true)
	{
		m_twinkleCount++;

		if (m_twinkleCount == 100)
		{
			tmp_roundEffect->Play();
			m_twinkleCount = 0;
			m_isResult = false;
		}
	}
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
