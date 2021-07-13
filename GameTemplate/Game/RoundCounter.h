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
	}

	/**
	 * @brief �X�v���C�g��\��
	*/
	void Disable()
	{
		//�S�ẴX�v���C�g�̃X�P�[����0�ɂ��Ĕ�\��
		for (auto roundSprite : m_gameRoundSprite)
		{
			roundSprite[0]->SetScale({ 0.0f,0.0f,0.0f });
			roundSprite[1]->SetScale({ 0.0f,0.0f,0.0f });
		}
	}

	/**
	 * @brief �X�v���C�g���Q�[����ʗp�̈ʒu�ŕ\��
	*/
	void EnableGameRound();

	/**
	 * @brief �X�v���C�g�����ʉ�ʗp�̈ʒu�ŕ\��
	*/
	void EnableResultRound();

	/**
	 * @brief ���݂̃��E���h�̓ǂݏグ
	*/
	void RoundAnnounce();

private:
	int m_currentRound = 0;								//���݂̃��E���h��
	int m_latestRoundWinner = -1;						//��ԍŋ߂̃��E���h�̏���
	int m_playerTakeRound[2] = { 0,0 };					//�e�v���C���[�̎擾���E���h
	bool m_isResult = false;							//���ҕ\�����?
	int m_roundResultCount = 0;							//���E���h���Ƃ̌��ʂ��Đ������܂ł̃J�E���g
	prefab::CSpriteRender* m_gameRoundSprite[2][2] =	//�擾���E���h�\���X�v���C�g
	{ {nullptr,nullptr},{nullptr,nullptr} };
	prefab::CEffect2D* m_roundGetEffect = nullptr;		//���E���h�l���\���̃G�t�F�N�g
	bool m_roundAlreadyAnnounce = false;
};

