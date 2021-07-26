#pragma once

class RoundCounter : public IGameObject
{
	~RoundCounter() override;
	bool Start() override;
	void Update() override;
public:

	/**
	 * @brief �ő僉�E���h���Z�b�g
	 * @param round �ő僉�E���h
	*/
	void SetMaxRound(int round)
	{
		m_maxRound = round;
		//�ő僉�E���h������\������X�v���C�g�̐����v�Z
		m_roundToWin = (m_maxRound / 2) + 1;
	}

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
	 * @brief �X�v���C�g���Q�[����ʗp�̈ʒu�ŕ\��
	*/
	void MoveToGamePosition();

	/**
	* @brief �X�v���C�g�����ʉ�ʂ̈ʒu�Ɉړ�
	*/
	void MoveToResultPosition();

	/**
	 * @brief �X�v���C�g�����ʉ�ʗp�̈ʒu�ŕ\��
	*/
	void StartResultMove();

	/**
	 * @brief ���݂̃��E���h�̓ǂݏグ
	*/
	void RoundAnnounce();

private:
	static int m_maxRound;									//�ő僉�E���h��(����)
	static int m_roundToWin;								//�����ɕK�v�ȃ��E���h
	int m_currentRound = 0;									//���݂̃��E���h��
	int m_latestRoundWinner = -1;							//��ԍŋ߂̃��E���h�̏���
	int m_playerTakeRound[2] = { 0,0 };						//�e�v���C���[�̎擾���E���h
	bool m_isResult = false;								//���ҕ\�����?
	int m_roundResultCount = 0;								//���E���h���Ƃ̌��ʂ��Đ������܂ł̃J�E���g
	prefab::CEffect2D* m_roundGetEffect = nullptr;			//���E���h�l���\���̃G�t�F�N�g
	bool m_roundAlreadyAnnounce = false;					//���̃��E���h�̃A�i�E���X������������?
	std::vector<prefab::CSpriteRender*> m_player1Sprites;	//�v���C���[1�̃X�v���C�g�̃��X�g
	std::vector<prefab::CSpriteRender*> m_player2Sprites;	//�v���C���[2�̃X�v���C�g�̃��X�g

};

