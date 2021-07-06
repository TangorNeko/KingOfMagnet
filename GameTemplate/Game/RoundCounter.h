#pragma once

class RoundCounter : public IGameObject
{
public:
	RoundCounter();
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
		m_playerTakeRound[playerNum]++;
	}

	void Disable()
	{
		for (auto font : tmp_playerRoundFont)
		{
			font->SetScale({ 0.0f,0.0f });
		}
	}

	void Enable()
	{
		for (auto font : tmp_playerRoundFont)
		{
			font->SetScale({ 1.0f,1.0f });
		}
	}

private:
	int m_currentRound = 0;					//���݂̃��E���h��
	int m_playerTakeRound[2] = { 0,0 };		//�e�v���C���[�̎擾���E���h

	prefab::CFontRender* tmp_playerRoundFont[2] = { nullptr,nullptr };//�v���C���[���\���t�H���g
};

