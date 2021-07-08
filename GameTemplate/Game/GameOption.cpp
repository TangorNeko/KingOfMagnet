#include "stdafx.h"
#include "GameOption.h"


namespace
{
	const Vector2 FONT_OPTION_POSITION = { -125.0f,240.0f };
	const Vector2 FONT_BGMVOLUME_POSITION = { -450.0f,160.0f };
	const Vector2 FONT_SEVOLUME_POSITION = { -450.0f,70.0f };
	const Vector2 FONT_P1SENSITIVITY_POSITION = { -450.0f,-20.0f };
	const Vector2 FONT_P2SENSITIVITY_POSITION = { -450.0f,-110.0f };
	const Vector2 FONT_GAMETIMELIMIT_POSITION = { -450.0f,-200.0f };
	const Vector4 FONT_SHADOWCOLOR_BLACK = { 0,0,0,1 };
	const float FONT_SHADOWOFFSET = 2.0f;
	const Vector2 FONT_SCALE_CLOSE = { 0.0f,0.0f };
	const Vector2 FONT_SCALE_OPEN = { 1.0f,1.0f };
	const Vector4 FONT_COLOR_WHITE = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 FONT_COLOR_BLUE = { 0.1f,0.1f,1.0f,1.0f };
	const Vector4 FONT_COLOR_RED = { 1.0f,0.25f,0.25f,1.0f };

	const Vector4 SPRITE_COLOR_LIGHTBLUE = { 0.0f,0.75f,0.75f,0.8f };
	const Vector3 SPRITE_SCALE_CLOSE = { 0.0f,0.0f,0.0f };
	const Vector3 SPRITE_SCALE_OPEN = { 1.0f,1.0f,1.0f };
	const int SPRITE_SCALING_TIME = 12;
	const int SPRITE_SCALING_DELAY = 1;

	const float OPTION_VALUE_CHANGERATE = 0.05f;
	const float OPTION_GAMETIME_CHANGERATE = 10.0f;
	const int OPTION_ITEM_UNDERRANGE = -1;
	const int OPTION_ITEM_BGM = 0;
	const int OPTION_ITEM_SE = 1;
	const int OPTION_ITEM_P1SENSITIVITY = 2;
	const int OPTION_ITEM_P2SENSITIVITY = 3;
	const int OPTION_ITEM_GAMETIMELIMIT = 4;
	const int OPTION_ITEM_OVERRANGE = 5;

	const float SOUND_VOLUME_MIN = 0.0f;
	const float SOUND_VOLUME_MAX = 1.0f;

	const float CONTROL_SENSITIVITY_MIN = 0.1f;
	const float CONTROL_SENSITIVITY_MAX = 5.0f;
	
	const float GAME_TIMELIMIT_MIN = 30.0f;
	const float GAME_TIMELIMIT_MAX = 90.0f;
}

float GameOption::m_BGMVolume = 1.0f;		//BGM�̃{�����[��
float GameOption::m_SEVolume = 1.0f;		//���ʉ��̃{�����[��
float GameOption::m_1PSensitivity = 2.0f;	//�v���C���[1�̃J�������x
float GameOption::m_2PSensitivity = 2.0f;	//�v���C���[2�̃J�������x
float GameOption::m_gameTimeLimit = 40.0f;	//�Q�[���̐�������

GameOption::~GameOption()
{
	//�X�v���C�g�A�t�H���g�̍폜
	DeleteGO(m_optionBackGroundSprite);
	DeleteGO(m_BGMVolumeFont);
	DeleteGO(m_SEVolumeFont);
	DeleteGO(m_1PSensitivityFont);
	DeleteGO(m_2PSensitivityFont);
	DeleteGO(m_gameTimeLimitFont);
}

bool GameOption::Start()
{
	//�w�i�X�v���C�g
	m_optionBackGroundSprite = NewGO<prefab::CSpriteRender>(0);
	m_optionBackGroundSprite->Init("Assets/Image/White.dds", 960, 540);
	m_optionBackGroundSprite->SetScale(SPRITE_SCALE_CLOSE);
	m_optionBackGroundSprite->SetMulColor(SPRITE_COLOR_LIGHTBLUE);

	//�I�v�V�����t�H���g
	m_optionFont = NewGO<prefab::CFontRender>(0);
	m_optionFont->SetScale(FONT_SCALE_CLOSE);
	m_optionFont->SetPosition(FONT_OPTION_POSITION);
	m_optionFont->SetText(L"OPTION");
	m_optionFont->SetShadowFlag(true);
	m_optionFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_optionFont->SetShadowOffset(FONT_SHADOWOFFSET);

	//BGM�{�����[���t�H���g
	m_BGMVolumeFont = NewGO<prefab::CFontRender>(0);
	m_BGMVolumeFont->SetScale(FONT_SCALE_CLOSE);
	m_BGMVolumeFont->SetPosition(FONT_BGMVOLUME_POSITION);
	m_BGMVolumeFont->SetShadowFlag(true);
	m_BGMVolumeFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_BGMVolumeFont->SetShadowOffset(FONT_SHADOWOFFSET);


	//SE�{�����[���t�H���g
	m_SEVolumeFont = NewGO<prefab::CFontRender>(0);
	m_SEVolumeFont->SetScale(FONT_SCALE_CLOSE);
	m_SEVolumeFont->SetPosition(FONT_SEVOLUME_POSITION);
	m_SEVolumeFont->SetShadowFlag(true);
	m_SEVolumeFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_SEVolumeFont->SetShadowOffset(FONT_SHADOWOFFSET);


	//1P���슴�x�t�H���g
	m_1PSensitivityFont = NewGO<prefab::CFontRender>(0);
	m_1PSensitivityFont->SetScale(FONT_SCALE_CLOSE);
	m_1PSensitivityFont->SetPosition(FONT_P1SENSITIVITY_POSITION);
	m_1PSensitivityFont->SetShadowFlag(true);
	m_1PSensitivityFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_1PSensitivityFont->SetShadowOffset(FONT_SHADOWOFFSET);


	//2P���슴�x�t�H���g
	m_2PSensitivityFont = NewGO<prefab::CFontRender>(0);
	m_2PSensitivityFont->SetScale(FONT_SCALE_CLOSE);
	m_2PSensitivityFont->SetPosition(FONT_P2SENSITIVITY_POSITION);
	m_2PSensitivityFont->SetShadowFlag(true);
	m_2PSensitivityFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_2PSensitivityFont->SetShadowOffset(FONT_SHADOWOFFSET);
	

	//�Q�[���������ԃt�H���g
	m_gameTimeLimitFont = NewGO<prefab::CFontRender>(0);
	m_gameTimeLimitFont->SetScale(FONT_SCALE_CLOSE);
	m_gameTimeLimitFont->SetPosition(FONT_GAMETIMELIMIT_POSITION);
	m_gameTimeLimitFont->SetShadowFlag(true);
	m_gameTimeLimitFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_gameTimeLimitFont->SetShadowOffset(FONT_SHADOWOFFSET);
	return true;
}

void GameOption::Update()
{
	//�I�v�V�������J���Ă��āA�g�傪�I����Ă���B
	if (isQueuing() == false && m_isOpen == true)
	{
		//�t�H���g�̊g�嗦��1�{�ɂ��邱�Ƃŋ[���I�ɕ\��
		m_optionFont->SetScale(FONT_SCALE_OPEN);
		m_BGMVolumeFont->SetScale(FONT_SCALE_OPEN);
		m_SEVolumeFont->SetScale(FONT_SCALE_OPEN);
		m_1PSensitivityFont->SetScale(FONT_SCALE_OPEN);
		m_2PSensitivityFont->SetScale(FONT_SCALE_OPEN);
		m_gameTimeLimitFont->SetScale(FONT_SCALE_OPEN);

		//�I�������t�H���g�ȊO�𔒂ɂ��邽�߈�U�S�����ɂ��Ă���
		m_BGMVolumeFont->SetColor(Vector4::White);
		m_SEVolumeFont->SetColor(Vector4::White);
		m_1PSensitivityFont->SetColor(Vector4::White);
		m_2PSensitivityFont->SetColor(Vector4::White);
		m_gameTimeLimitFont->SetColor(Vector4::White);

		//���ڂ̑I�𒆂Ȃ�
		if (m_selectingState == enItem)
		{
			//���������Ɖ��̍��ڂ�
			if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonDown))
			{
				if (++m_selectingItem == OPTION_ITEM_OVERRANGE)
				{
					m_selectingItem = OPTION_ITEM_BGM;
				}
			}
			else if(g_pad[PAD_PLAYER1]->IsTrigger(enButtonUp) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp))//��������Ə�̍��ڂ�
			{
				if (--m_selectingItem == OPTION_ITEM_UNDERRANGE)
				{
					m_selectingItem = OPTION_ITEM_GAMETIMELIMIT;
				}
			}

			//�I��ł��鍀�ڂ��|�C���^�ɃZ�b�g
			//NOTE:switch���̑��p����������ă|�C���^�ɃZ�b�g�������A�ݒ荀�ڂɂ���Đ��l�̕����Ⴄ�ȏ��������...
			switch (m_selectingItem)
			{
			case OPTION_ITEM_BGM://BGM�{�����[��
				m_selectingItemValue = &m_BGMVolume;
				m_selectingItemFont = m_BGMVolumeFont;
				break;
			case OPTION_ITEM_SE://SE�{�����[��
				m_selectingItemValue = &m_SEVolume;
				m_selectingItemFont = m_SEVolumeFont;
				break;
			case OPTION_ITEM_P1SENSITIVITY://1P�J�������x
				m_selectingItemValue = &m_1PSensitivity;
				m_selectingItemFont = m_1PSensitivityFont;
				break;
			case OPTION_ITEM_P2SENSITIVITY://2P�J�������x
				m_selectingItemValue = &m_2PSensitivity;
				m_selectingItemFont = m_2PSensitivityFont;
				break;
			case OPTION_ITEM_GAMETIMELIMIT://�Q�[����������
				m_selectingItemValue = &m_gameTimeLimit;
				m_selectingItemFont = m_gameTimeLimitFont;
				break;
			}

			//�I������Ă��鍀�ڂ�F��
			m_selectingItemFont->SetColor(FONT_COLOR_BLUE);

			//A�{�^���������ƑI�����Ă��鍀�ڂ̐��l��ݒ肷�郂�[�h�Ɉڍs
			if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA))
			{
				m_selectingState = enNumeric;

				//���l�ݒ�O�̒l���L�^
				m_selectingItemTemporaryValue = *m_selectingItemValue;
			}
		}
		else if(m_selectingState == enNumeric)//���ڂ̐��l��ݒ肷�郂�[�h�Ȃ�
		{
			if (m_selectingItemValue == &m_gameTimeLimit)
			{
				//���������Ɛ��l�����炷
				if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonDown))
				{
					(*m_selectingItemValue) -= OPTION_GAMETIME_CHANGERATE;
				}
				else if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonUp) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp))//��������Ɛ��l�𑝂₷
				{
					(*m_selectingItemValue) += OPTION_GAMETIME_CHANGERATE;
				}
			}
			else
			{
				//���������Ɛ��l�����炷
				if (g_pad[PAD_PLAYER1]->IsPress(enButtonDown) || g_pad[PAD_PLAYER2]->IsPress(enButtonDown))
				{
					(*m_selectingItemValue) -= OPTION_VALUE_CHANGERATE;
				}
				else if (g_pad[PAD_PLAYER1]->IsPress(enButtonUp) || g_pad[PAD_PLAYER2]->IsPress(enButtonUp))//��������Ɛ��l�𑝂₷
				{
					(*m_selectingItemValue) += OPTION_VALUE_CHANGERATE;
				}
			}

			//�I��ł���l��BGM�̉��ʂȂ�
			if (m_selectingItemValue == &m_BGMVolume)
			{
				//BGM�̉��ʂ͈̔͐���(0.0~1.0)
				if (*m_selectingItemValue <= SOUND_VOLUME_MIN)
				{
					*m_selectingItemValue = SOUND_VOLUME_MIN;
				}
				else if (*m_selectingItemValue >= SOUND_VOLUME_MAX)
				{
					*m_selectingItemValue = SOUND_VOLUME_MAX;
				}

				//�ݒ�l��BGM�̉��ʂƂ��ăZ�b�g
				CSoundEngine::GetInstance()->SetBGMVolume(*m_selectingItemValue);
			}
			else if (m_selectingItemValue == &m_SEVolume)//�I��ł���l��SE�̉��ʂȂ�
			{
				//SE�̉��ʂ͈̔͐���(0.0~1.0)
				if (*m_selectingItemValue <= SOUND_VOLUME_MIN)
				{
					*m_selectingItemValue = SOUND_VOLUME_MIN;
				}
				else if (*m_selectingItemValue >= SOUND_VOLUME_MAX)
				{
					*m_selectingItemValue = SOUND_VOLUME_MAX;
				}

				//�ݒ�l��SE�̉��ʂƂ��ăZ�b�g
				CSoundEngine::GetInstance()->SetSEVolume(*m_selectingItemValue);
			}
			else if (m_selectingItemValue == &m_1PSensitivity || m_selectingItemValue == &m_2PSensitivity)//�I��ł���l���J�����̊��x�Ȃ�
			{
				//�v���C���[�̃J�������x�͈̔͐���(0.1~5.0)
				if (*m_selectingItemValue <= CONTROL_SENSITIVITY_MIN)
				{
					*m_selectingItemValue = CONTROL_SENSITIVITY_MIN;
				}
				else if (*m_selectingItemValue >= CONTROL_SENSITIVITY_MAX)
				{
					*m_selectingItemValue = CONTROL_SENSITIVITY_MAX;
				}
			}
			else if (m_selectingItemValue == &m_gameTimeLimit)
			{
				//�Q�[���̐������Ԃ͈̔͐���(30.0f~90.0f)
				if (*m_selectingItemValue <= GAME_TIMELIMIT_MIN)
				{
					*m_selectingItemValue = GAME_TIMELIMIT_MIN;
				}
				else if (*m_selectingItemValue >= GAME_TIMELIMIT_MAX)
				{
					*m_selectingItemValue = GAME_TIMELIMIT_MAX;
				}
			}
			//�I������Ă��鍀�ڂ�ԐF��
			m_selectingItemFont->SetColor(FONT_COLOR_RED);

			//A�{�^���������ƒl���̂܂܍��ڑI����(�l�̌���)
			if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA))
			{
				m_selectingState = enItem;
			}
			else if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonB) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonB))//B�{�^���������ƒl��ύX�O�ɖ߂��č��ڑI����(�l�̃L�����Z��)
			{
				m_selectingState = enItem;

				*m_selectingItemValue = m_selectingItemTemporaryValue;

				if (m_selectingItemValue == &m_BGMVolume)
				{
					//�ݒ�l��BGM�̉��ʂƂ��ăZ�b�g
					CSoundEngine::GetInstance()->SetBGMVolume(*m_selectingItemValue);
				}
				else if (m_selectingItemValue == &m_SEVolume)//�I��ł���l��SE�̉��ʂȂ�
				{
					//�ݒ�l��SE�̉��ʂƂ��ăZ�b�g
					CSoundEngine::GetInstance()->SetSEVolume(*m_selectingItemValue);
				}
			}
		}

		//�ύX���ꂽ���l�����Ƀt�H���g�̕`�敶�����ύX
		swprintf_s(m_buffer, L"BGM VOLUME     = %.2f", m_BGMVolume);
		m_BGMVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"SE  VOLUME     = %.2f", m_SEVolume);
		m_SEVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"1P SENSITIVITY = %.2f", m_1PSensitivity);
		m_1PSensitivityFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"2P SENSITIVITY = %.2f", m_2PSensitivity);
		m_2PSensitivityFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"TIMELIMIT      = %.2f", m_gameTimeLimit);
		m_gameTimeLimitFont->SetText(m_buffer);

	}
	else//����ȊO
	{
		//�t�H���g�̊g�嗦��0�{�ɂ��邱�Ƃŋ[���I�ɏ���
		m_optionFont->SetScale(FONT_SCALE_CLOSE);
		m_BGMVolumeFont->SetScale(FONT_SCALE_CLOSE);
		m_SEVolumeFont->SetScale(FONT_SCALE_CLOSE);
		m_1PSensitivityFont->SetScale(FONT_SCALE_CLOSE);
		m_2PSensitivityFont->SetScale(FONT_SCALE_CLOSE);
		m_gameTimeLimitFont->SetScale(FONT_SCALE_CLOSE);
	}
}

void GameOption::Open()
{
	if (isQueuing() == false)
	{
		//�w�i�̊g�嗦��1�{�ɂ���L���[���Z�b�g
		m_optionBackGroundSprite->GetSpriteSupporter().SpriteScale(SPRITE_SCALE_OPEN, SPRITE_SCALING_TIME, SPRITE_SCALING_DELAY);

		//�J�����t���O���I��
		m_isOpen = true;
	}
}

void GameOption::Close()
{
	if (isQueuing() == false)
	{
		//�w�i�̊g�嗦��0�{�ɂ���L���[���Z�b�g
		m_optionBackGroundSprite->GetSpriteSupporter().SpriteScale(SPRITE_SCALE_CLOSE, SPRITE_SCALING_TIME, SPRITE_SCALING_DELAY);

		//�J�����t���O���I�t
		m_isOpen = false;

		//�I�����ڂ����Z�b�g
		m_selectingItem = OPTION_ITEM_BGM;
	}
}