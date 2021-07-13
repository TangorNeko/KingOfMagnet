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
	//�X�v���C�g�̍폜
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]);
	DeleteGO(m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]);
}

bool RoundCounter::Start()
{
	//�v���C���[1�̃��E���h1�̃X�v���C�g
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION);

	//�v���C���[1�̃��E���h2�̃X�v���C�g
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->SetPosition(SPRITE_PLAYER1_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	//�v���C���[2�̃��E���h1�̃X�v���C�g
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	//�v���C���[2�̃��E���h2�̃X�v���C�g
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO] = NewGO<prefab::CSpriteRender>(0);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->SetPosition(SPRITE_PLAYER2_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->Init("Assets/Image/LightOff.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);

	//���E���h�擾����2D�G�t�F�N�g
	m_roundGetEffect = NewGO<prefab::CEffect2D>(0);
	m_roundGetEffect->Init(u"Assets/effect/Twinkle.efk");
	m_roundGetEffect->SetScale(EFFECT_TWINKLE_SCALE);
	return true;
}

void RoundCounter::Update()
{
	//���ʕ\�����?
	if (m_isResult == true)
	{
		//�J�E���g���C���N�������g
		m_roundResultCount++;

		//��ʊO����X�b�Ɣ�яo���Ă���ړ����Z�b�g
		if (m_roundResultCount == ROUNDRESULTCOUNT_SPRITEMOVE)
		{
			m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER1_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER1_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER2_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER2_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
		}

		//���E���h�擾�\��
		if (m_roundResultCount == ROUNDRESULTCOUNT_UPDATEROUND)
		{
			//�Ō�Ƀ��E���h���擾�����v���C���[�ԍ��ŕ���
			if (m_latestRoundWinner == NUMBER_PLAYER1)
			{
				//���C�g�̃X�v���C�g�������Ă�����̂�
				m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->Init(
					"Assets/Image/LightGreen.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT
				);
			}
			else
			{
				//���C�g�̃X�v���C�g�������Ă�����̂�
				m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->Init(
					"Assets/Image/LightYellow.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT
				);
			}

			//����G�t�F�N�g���Đ�����̂͐�����点���X�v���C�g�̍��W
			Vector3 twinklePos = m_gameRoundSprite[m_latestRoundWinner][m_playerTakeRound[m_latestRoundWinner]]->GetPosition();
			
			//�v���C���[�̎擾���E���h�ϐ����C���N�������g
			m_playerTakeRound[m_latestRoundWinner]++;

			//�G�t�F�N�g�̍Đ�
			m_roundGetEffect->SetPosition(twinklePos);
			m_roundGetEffect->Play();

			SoundOneShotPlay(L"Assets/sound/RoundGet.wav");

			//�J�E���g���Z�b�g
			m_roundResultCount = ROUNDRESULTCOUNT_START;

			//���ʕ\���I��
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
		//�v���C���[1��2�{����Ă����珟�҂̓v���C���[1
		return NUMBER_PLAYER1;
	}
	else if (m_playerTakeRound[NUMBER_PLAYER2] == ROUND_WIN)
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

void RoundCounter::EnableGameRound()
{
	//�S�ẴX�v���C�g�̃X�P�[����1�ɂ��ĕ\��
	for (auto roundSprite : m_gameRoundSprite)
	{
		roundSprite[ROUND_ONE]->SetScale(Vector3::One);
		roundSprite[ROUND_TWO]->SetScale(Vector3::One);
	}

	//�e�X�v���C�g���Q�[����ʈʒu�ɐݒ�
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_ONE]->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER1][ROUND_TWO]->SetPosition(SPRITE_PLAYER1_ROUND2_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_ONE]->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION);
	m_gameRoundSprite[NUMBER_PLAYER2][ROUND_TWO]->SetPosition(SPRITE_PLAYER2_ROUND2_GAME_POSITION);
}

void RoundCounter::EnableResultRound()
{
	//�S�ẴX�v���C�g�̃X�P�[����1�ɂ��ĕ\��
	for (auto roundSprite : m_gameRoundSprite)
	{
		roundSprite[0]->SetScale(Vector3::One);
		roundSprite[1]->SetScale(Vector3::One);
	}

	//�e�X�v���C�g�����ʉ�ʂ̈ʒu(���E��ʊO)�ɐݒ�
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