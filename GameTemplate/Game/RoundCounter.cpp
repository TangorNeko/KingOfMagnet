#include "stdafx.h"
#include "RoundCounter.h"

namespace
{
	const Vector3 EFFECT_TWINKLE_SCALE = { 30.0f,30.0f,30.0f };							//���E���h�擾�G�t�F�N�g�̊g�嗦
	const int SPRITE_ROUND_WIDTH = 64;													//���E���h�����v�X�v���C�g�̕�
	const int SPRITE_ROUND_HEIGHT = 64;													//���E���h�����v�X�v���C�g�̍���
	const Vector3 SPRITE_PLAYER1_ROUND1_GAME_POSITION = { -140.0f,260.0f,0.0f };		//�v���C���[1�̃Q�[�����̃��E���h1�X�v���C�g�̍��W
	const Vector3 SPRITE_PLAYER2_ROUND1_GAME_POSITION = { 140.0f,260.0f,0.0f };			//�v���C���[2�̃Q�[�����̃��E���h1�X�v���C�g�̍��W
	const Vector3 SPRITE_PLAYER1_ROUND1_RESULT_POSITION = { -690.0f, -300.0f, 0.0f };	//�v���C���[1�̌��ʉ�ʂ̃��E���h1�X�v���C�g�̍��W
	const Vector3 SPRITE_PLAYER2_ROUND1_RESULT_POSITION = { 690.0f,-300.0f,0.0f };		//�v���C���[2�̌��ʉ�ʂ̃��E���h1�X�v���C�g�̍��W
	const Vector3 SPRITE_PLAYER1_SIDE_TO_CENTER = { 200.0f,0.0f,0.0f };					//�v���C���[1�̌��ʉ�ʂ̃��E���h�X�v���C�g���X���C�h���Ă��鎞�̈ړ���
	const Vector3 SPRITE_PLAYER2_SIDE_TO_CENTER = { -200.0f,0.0f,0.0f };				//�v���C���[2�̌��ʉ�ʂ̃��E���h�X�v���C�g���X���C�h���Ă��鎞�̈ړ���
	const Vector3 SPRITE_PLAYER1_OFFSET = { -50.0f,0.0f,0.0f };							//�v���C���[1�ׂ̗̃��E���h�X�v���C�g�Ƃ̋��
	const Vector3 SPRITE_PLAYER2_OFFSET = { 50.0f,0.0f,0.0f };							//�v���C���[2�ׂ̗̃��E���h�X�v���C�g�Ƃ̋��
	const int SPRITE_RESULT_MOVETIME = 25;												//���ʉ�ʂ̃��E���h�X�v���C�g�̈ړ�����
	const int SPRITE_RESULT_MOVEDELAY = 0;												//���ʉ�ʂ̃��E���h�X�v���C�g�̈ړ��f�B���C
	const int ROUNDRESULTCOUNT_START = 0;												//���ʉ�ʂ̃��E���h�X�v���C�g�̈ړ��^�C�}�[�@�X�^�[�g
	const int ROUNDRESULTCOUNT_SPRITEMOVE = 20;											//���ʉ�ʂ̃��E���h�X�v���C�g�̈ړ��^�C�}�[�@�ړ��J�n
	const int ROUNDRESULTCOUNT_UPDATEROUND = 100;										//���ʉ�ʂ̃��E���h�X�v���C�g�̈ړ��^�C�}�[�@���E���h�擾�\��

	//���ʂɂȂ肻��
	const float SOUND_SE_VOICE_VOLUME = 3.0f;											//�{�C�XSE�̃{�����[��
}

int RoundCounter::m_maxRound = 1;
int RoundCounter::m_roundToWin = 1;

RoundCounter::~RoundCounter()
{
	//�X�v���C�g�̍폜
	for (int i = 0;i < m_roundToWin;i++)
	{
		DeleteGO(m_player1Sprites.at(i));
		DeleteGO(m_player2Sprites.at(i));
	}
}

bool RoundCounter::Start()
{
	//�X�v���C�g�̓��I�z��̃T�C�Y���m��
	m_player1Sprites.reserve(m_roundToWin);
	m_player2Sprites.reserve(m_roundToWin);

	//�\���X�v���C�g�̐����e�v���C���[�̃��E���h�\���X�v���C�g���쐬
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
	
	//���E���h�擾����2D�G�t�F�N�g�̏�����
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
			for (auto player1Sprite : m_player1Sprites)
			{
				player1Sprite->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER1_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			}

			for (auto player2Sprite : m_player2Sprites)
			{
				player2Sprite->GetSpriteSupporter().SpriteMove(SPRITE_PLAYER2_SIDE_TO_CENTER, SPRITE_RESULT_MOVETIME, SPRITE_RESULT_MOVEDELAY, true);
			}
		}

		//���E���h�擾�\��
		if (m_roundResultCount == ROUNDRESULTCOUNT_UPDATEROUND)
		{
			//����G�t�F�N�g���Đ�������W
			Vector3 twinklePos;

			//�Ō�Ƀ��E���h���擾�����v���C���[�ԍ��ŕ���
			if (m_latestRoundWinner == NUMBER_PLAYER1)
			{
				//���C�g�̃X�v���C�g�������Ă�����̂�
				prefab::CSpriteRender* twinkleSprite = m_player1Sprites.at(m_playerTakeRound[NUMBER_PLAYER1]);
				twinkleSprite->Init("Assets/Image/LightGreen.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
				
				//����G�t�F�N�g���Đ�����̂͌��点���X�v���C�g�̍��W
				twinklePos = twinkleSprite->GetPosition();
			}
			else
			{
				//���C�g�̃X�v���C�g�������Ă�����̂�
				prefab::CSpriteRender* twinkleSprite = m_player2Sprites.at(m_playerTakeRound[NUMBER_PLAYER2]);
				twinkleSprite->Init("Assets/Image/LightYellow.DDS", SPRITE_ROUND_WIDTH, SPRITE_ROUND_HEIGHT);
				
				//����G�t�F�N�g���Đ�����̂͌��点���X�v���C�g�̍��W
				twinklePos = twinkleSprite->GetPosition();
			}

			//�v���C���[�̎擾���E���h�ϐ����C���N�������g
			m_playerTakeRound[m_latestRoundWinner]++;

			//�G�t�F�N�g�̍Đ�
			m_roundGetEffect->SetPosition(twinklePos);
			m_roundGetEffect->Play();

			//���E���h�擾�̉����Đ�
			SoundOneShotPlay(L"Assets/sound/RoundGet.wav");

			//�J�E���g���Z�b�g
			m_roundResultCount = ROUNDRESULTCOUNT_START;

			//���ʕ\���I��
			m_isResult = false;

			//���E���h�����C���N�������g
			m_currentRound++;

			//���E���h���̃A�i�E���X�t���O�����Z�b�g
			m_roundAlreadyAnnounce = false;
		}
	}
}

int RoundCounter::GetOverAllWinner()
{
	if (m_playerTakeRound[NUMBER_PLAYER1] == m_roundToWin)
	{
		//�v���C���[1���K�v���E���h������Ă����珟�҂̓v���C���[1
		return NUMBER_PLAYER1;
	}
	else if (m_playerTakeRound[NUMBER_PLAYER2] == m_roundToWin)
	{
		//�v���C���[2���K�v���E���h������Ă����珟�҂̓v���C���[2
		return NUMBER_PLAYER2;
	}
	else
	{
		//�ǂ���̃v���C���[���K�v���E���h������Ă��Ȃ��̂ŏ��Җ��m��
		return -1;
	}
}

void RoundCounter::MoveToGamePosition()
{
	//�e�X�v���C�g���Q�[����ʈʒu�ɐݒ�
	for (int i = 0;i < m_roundToWin;i++)
	{
		//�v���C���[1�̃X�v���C�g
		m_player1Sprites.at(i)->SetPosition(SPRITE_PLAYER1_ROUND1_GAME_POSITION + (SPRITE_PLAYER1_OFFSET * i));

		//�v���C���[2�̃X�v���C�g
		m_player2Sprites.at(i)->SetPosition(SPRITE_PLAYER2_ROUND1_GAME_POSITION + (SPRITE_PLAYER2_OFFSET * i));
	}
}

void RoundCounter::MoveToResultPosition()
{
	//�e�X�v���C�g�����ʕ\���ʒu�ɐݒ�
	for (int i = 0;i < m_roundToWin;i++)
	{
		//�v���C���[1�̃X�v���C�g
		m_player1Sprites.at(i)->SetPosition(SPRITE_PLAYER1_ROUND1_RESULT_POSITION + (SPRITE_PLAYER1_OFFSET * i));

		//�v���C���[2�̃X�v���C�g
		m_player2Sprites.at(i)->SetPosition(SPRITE_PLAYER2_ROUND1_RESULT_POSITION + (SPRITE_PLAYER2_OFFSET * i));
	}
}

void RoundCounter::StartResultMove()
{
	//���U���g�\���̊J�n�t���O���I����
	m_isResult = true;
}

void RoundCounter::RoundAnnounce()
{
	//���łɃA�i�E���X���Ă�����return
	if (m_roundAlreadyAnnounce == true)
	{
		return;
	}

	//���݂̃��E���h���ɉ����ă{�C�X���Đ�
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

	//���łɂ��̃��E���h�̓A�i�E���X�������t���O���I��
	m_roundAlreadyAnnounce = true;
}