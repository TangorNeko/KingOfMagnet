#pragma once

class RoundCounter : public IGameObject
{
public:
	~RoundCounter();
private:
	bool Start() override;
	void Update() override;
public:

	/**
	 * @brief 2���E���h��悵�ď��������v���C���[���擾
	 * @return ���҃v���C���[�ԍ��@�܂����܂��Ă��Ȃ��ꍇ��-1
	*/
	int GetOverAllWinner();

	/**
	 * @brief ���E���h�̏��҂�o�^
	 * @param playerNum ���E���h�̏��҂̃v���C���[�ԍ�
	*/
	void SubmitRoundWinner(int playerNum)
	{
		m_latestRoundWinner = playerNum;
		m_playerTakeRound[playerNum]++;
	}

	void Disable()
	{
		for (auto font : tmp_gameRoundFont)
		{
			font->SetScale({ 0.0f,0.0f });
		}
		
		for (auto font : tmp_resultRoundFont)
		{
			font->SetScale({ 0.0f,0.0f });
		}
	}

	void EnableGameRound()
	{
		for (auto font : tmp_gameRoundFont)
		{
			font->SetScale({ 1.0f,1.0f });
		}
	}

	void EnableResultRound()
	{
		for (auto font : tmp_resultRoundFont)
		{
			font->SetScale({ 1.0f,1.0f });
		}

		switch (m_latestRoundWinner)
		{
		case 0:
			tmp_roundEffect->SetPosition({ -500.0f,-250.0f,0.0f });
			break;
		case 1:
			tmp_roundEffect->SetPosition({ 500.0f,-250.0f,0.0f });
			break;
		}

		m_isResult = true;
	}

private:
	int m_currentRound = 0;					//���݂̃��E���h��
	int m_latestRoundWinner = -1;			//��ԍŋ߂̃��E���h�̏���
	int m_playerTakeRound[2] = { 0,0 };		//�e�v���C���[�̎擾���E���h
	bool m_isResult = false;
	int m_twinkleCount = 0;

	prefab::CFontRender* tmp_gameRoundFont[2] = { nullptr,nullptr };//�Q�[������ʂ̉��\���t�H���g
	prefab::CFontRender* tmp_resultRoundFont[2] = { nullptr,nullptr };//���U���g���̉��\���t�H���g
	prefab::CEffect2D* tmp_roundEffect = nullptr; //���E���h�l���\���̃G�t�F�N�g
};

