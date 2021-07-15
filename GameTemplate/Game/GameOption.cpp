#include "stdafx.h"
#include "GameOption.h"
#include <algorithm>


namespace
{
	const Vector2 FONT_OPTION_POSITION = { -125.0f,240.0f };
	const Vector2 FONT_BGMVOLUME_POSITION = { -450.0f,170.0f };
	const Vector2 FONT_SEVOLUME_POSITION = { -450.0f,100.0f };
	const Vector2 FONT_P1SENSITIVITY_POSITION = { -450.0f,30.0f };
	const Vector2 FONT_P2SENSITIVITY_POSITION = { -450.0f,-40.0f };
	const Vector2 FONT_GAMETIMELIMIT_POSITION = { -450.0f,-110.0f };
	const Vector2 FONT_ROUNDTOWIN_POSITION = { -450.0f,-180.0f };
	const Vector4 FONT_SHADOWCOLOR_BLACK = { 0,0,0,1 };
	const float FONT_SHADOWOFFSET = 2.0f;
	const Vector2 FONT_SCALE_CLOSE = { 0.0f,0.0f };
	const Vector2 FONT_SCALE_OPEN = { 0.8f,0.8f };
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
	const float OPTION_ROUNDTOWIN_CHANGERATE = 2.0f;
	const int OPTION_ITEM_UNDERRANGE = -1;
	const int OPTION_ITEM_BGM = 0;
	const int OPTION_ITEM_SE = 1;
	const int OPTION_ITEM_P1SENSITIVITY = 2;
	const int OPTION_ITEM_P2SENSITIVITY = 3;
	const int OPTION_ITEM_GAMETIMELIMIT = 4;
	const int OPTION_ITEM_ROUNDTOWIN = 5;
	const int OPTION_ITEM_OVERRANGE = 6;

	const float SOUND_VOLUME_MIN = 0.0f;
	const float SOUND_VOLUME_MAX = 1.0f;

	const float CONTROL_SENSITIVITY_MIN = 0.1f;
	const float CONTROL_SENSITIVITY_MAX = 5.0f;
	
	const float GAME_TIMELIMIT_MIN = 30.0f;
	const float GAME_TIMELIMIT_MAX = 90.0f;

	const float GAME_ROUNDTOWIN_MIN = 1.0f;
	const float GAME_ROUNDTOWIN_MAX = 3.0f;

	const float SOUND_OPTIONDISPLAY_VOLUME = 0.5f;
}

bool GameOption::m_isInited = false;							//���������ꂽ��
OptionValue GameOption::m_BGMVolume;							//BGM�̃{�����[��					
OptionValue GameOption::m_SEVolume;							//���ʉ��̃{�����[��							
OptionValue GameOption::m_P1Sensitivity;						//�v���C���[1�̃J�������x							
OptionValue GameOption::m_P2Sensitivity;						//�v���C���[2�̃J�������x							
OptionValue GameOption::m_gameTimeLimit;						//�Q�[���̐�������								
OptionValue GameOption::m_roundToWin;							//�����ɕK�v�ȃ��E���h��

GameOption::~GameOption()
{
	//�X�v���C�g�A�t�H���g�̍폜
	DeleteGO(m_optionBackGroundSprite);
	DeleteGO(m_BGMVolumeFont);
	DeleteGO(m_SEVolumeFont);
	DeleteGO(m_1PSensitivityFont);
	DeleteGO(m_2PSensitivityFont);
	DeleteGO(m_gameTimeLimitFont);
	DeleteGO(m_roundToWinFont);
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
	
	//�����ɕK�v�ȃ��E���h���t�H���g
	m_roundToWinFont = NewGO<prefab::CFontRender>(0);
	m_roundToWinFont->SetScale(FONT_SCALE_CLOSE);
	m_roundToWinFont->SetPosition(FONT_ROUNDTOWIN_POSITION);
	m_roundToWinFont->SetShadowFlag(true);
	m_roundToWinFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_roundToWinFont->SetShadowOffset(FONT_SHADOWOFFSET);

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
		m_roundToWinFont->SetScale(FONT_SCALE_OPEN);

		//�I�������t�H���g�ȊO�𔒂ɂ��邽�߈�U�S�����ɂ��Ă���
		m_BGMVolumeFont->SetColor(Vector4::White);
		m_SEVolumeFont->SetColor(Vector4::White);
		m_1PSensitivityFont->SetColor(Vector4::White);
		m_2PSensitivityFont->SetColor(Vector4::White);
		m_gameTimeLimitFont->SetColor(Vector4::White);
		m_roundToWinFont->SetColor(Vector4::White);

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
				SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}
			else if(g_pad[PAD_PLAYER1]->IsTrigger(enButtonUp) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp))//��������Ə�̍��ڂ�
			{
				if (--m_selectingItem == OPTION_ITEM_UNDERRANGE)
				{
					m_selectingItem = OPTION_ITEM_ROUNDTOWIN;
				}
				SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}

			//�I��ł��鍀�ڂ��|�C���^�ɃZ�b�g
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
				m_selectingItemValue = &m_P1Sensitivity;
				m_selectingItemFont = m_1PSensitivityFont;
				break;
			case OPTION_ITEM_P2SENSITIVITY://2P�J�������x
				m_selectingItemValue = &m_P2Sensitivity;
				m_selectingItemFont = m_2PSensitivityFont;
				break;
			case OPTION_ITEM_GAMETIMELIMIT://�Q�[����������
				m_selectingItemValue = &m_gameTimeLimit;
				m_selectingItemFont = m_gameTimeLimitFont;
				break;
			case OPTION_ITEM_ROUNDTOWIN://�����ɕK�v�ȃ��E���h
				m_selectingItemValue = &m_roundToWin;
				m_selectingItemFont = m_roundToWinFont;
				break;
			}

			//�I������Ă��鍀�ڂ�F��
			m_selectingItemFont->SetColor(FONT_COLOR_BLUE);

			//A�{�^���������ƑI�����Ă��鍀�ڂ̐��l��ݒ肷�郂�[�h�Ɉڍs
			if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA))
			{
				m_selectingState = enNumeric;

				//���l�ݒ�O�̒l���L�^
				m_selectingItemTemporaryValue = m_selectingItemValue->GetValue();
				SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}
		}
		else if(m_selectingState == enNumeric)//���ڂ̐��l��ݒ肷�郂�[�h�Ȃ�
		{
			//�ݒ荀�ڂ̑������[�h�ŕ���
			switch (m_selectingItemValue->GetValueChangeMode())
			{
			case OptionValue::enTrigger:
				if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonDown))
				{
					m_selectingItemValue->DecreaseValue();//�l�̌���
					SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);
				}
				else if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonUp) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp))//��������Ɛ��l�𑝂₷
				{
					m_selectingItemValue->IncreaseValue();//�l�̑���
					SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

				}
				break;
			case OptionValue::enPress:
				if (g_pad[PAD_PLAYER1]->IsPress(enButtonDown) || g_pad[PAD_PLAYER2]->IsPress(enButtonDown))
				{
					m_selectingItemValue->DecreaseValue();//�l�̌���
					SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);
				}
				else if (g_pad[PAD_PLAYER1]->IsPress(enButtonUp) || g_pad[PAD_PLAYER2]->IsPress(enButtonUp))//��������Ɛ��l�𑝂₷
				{
					m_selectingItemValue->IncreaseValue();//�l�̑���
					SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

				}
			}
			

			//�I��ł���l��BGM�̉��ʂȂ�
			if (m_selectingItemValue == &m_BGMVolume)
			{
				//�ݒ�l��BGM�̉��ʂƂ��ăZ�b�g
				CSoundEngine::GetInstance()->SetBGMVolume(m_selectingItemValue->GetValue());
			}
			else if (m_selectingItemValue == &m_SEVolume)//�I��ł���l��SE�̉��ʂȂ�
			{
				//�ݒ�l��SE�̉��ʂƂ��ăZ�b�g
				CSoundEngine::GetInstance()->SetSEVolume(m_selectingItemValue->GetValue());
			}
			//�I������Ă��鍀�ڂ�ԐF��
			m_selectingItemFont->SetColor(FONT_COLOR_RED);

			//A�{�^���������ƒl���̂܂܍��ڑI����(�l�̌���)
			if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA))
			{
				m_selectingState = enItem;
				SoundOneShotPlay(L"Assets/sound/OptionSelect.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}
			else if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonB) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonB))//B�{�^���������ƒl��ύX�O�ɖ߂��č��ڑI����(�l�̃L�����Z��)
			{
				m_selectingState = enItem;

				m_selectingItemValue->SetValue(m_selectingItemTemporaryValue);

				if (m_selectingItemValue == &m_BGMVolume)
				{
					//�ݒ�l��BGM�̉��ʂƂ��ăZ�b�g
					CSoundEngine::GetInstance()->SetBGMVolume(m_selectingItemValue->GetValue());
				}
				else if (m_selectingItemValue == &m_SEVolume)//�I��ł���l��SE�̉��ʂȂ�
				{
					//�ݒ�l��SE�̉��ʂƂ��ăZ�b�g
					CSoundEngine::GetInstance()->SetSEVolume(m_selectingItemValue->GetValue());
				}

				SoundOneShotPlay(L"Assets/sound/OptionCancel.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}
		}

		//�ύX���ꂽ���l�����Ƀt�H���g�̕`�敶�����ύX
		swprintf_s(m_buffer, L"BGM VOLUME     = %.2f", m_BGMVolume.GetValue());
		m_BGMVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"SE  VOLUME     = %.2f", m_SEVolume.GetValue());
		m_SEVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"1P SENSITIVITY = %.2f", m_P1Sensitivity.GetValue());
		m_1PSensitivityFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"2P SENSITIVITY = %.2f", m_P2Sensitivity.GetValue());
		m_2PSensitivityFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"TIMELIMIT      = %.2f", m_gameTimeLimit.GetValue());
		m_gameTimeLimitFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"ROUNDTOWIN     = %.0f", m_roundToWin.GetValue());
		m_roundToWinFont->SetText(m_buffer);

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
		m_roundToWinFont->SetScale(FONT_SCALE_CLOSE);
	}
}

void GameOption::Open()
{
	if (isQueuing() == false)
	{
		//�w�i�̊g�嗦��1�{�ɂ���L���[���Z�b�g
		m_optionBackGroundSprite->GetSpriteSupporter().SpriteScale(SPRITE_SCALE_OPEN, SPRITE_SCALING_TIME, SPRITE_SCALING_DELAY);

		//�J���T�E���h���Đ�
		SoundOneShotPlay(L"Assets/sound/OptionDisplayOn.wav", SOUND_OPTIONDISPLAY_VOLUME);

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

		//����T�E���h���Đ�
		SoundOneShotPlay(L"Assets/sound/OptionDisplayOff.wav", SOUND_OPTIONDISPLAY_VOLUME);

		//�J�����t���O���I�t
		m_isOpen = false;

		//�I�����ڂ����Z�b�g
		m_selectingItem = OPTION_ITEM_BGM;

		//��������
		WriteOption();
	}
}

bool GameOption::ReadOption()
{
	//�I�[�v���Ɏ��s�������p�ɏ�����
	Init();

	FILE* fp = fopen("gameoption.dat", "rb");

	if (fp == nullptr)
	{
		return false;
	}

	//����OptionValue�Ɋi�[�ł��Ȃ������̂�(�Q�Ƃ�Ԃ��`�ɂ���΂�������������...)���[�J���ϐ��Ɉ�U�i�[
	float BGMVolume,SEVolume, P1Sensitivity, P2Sensitivity, gameTimeLimit, roundToWin;
	fread(&BGMVolume,sizeof(float),1,fp);
	fread(&SEVolume,sizeof(float),1,fp);
	fread(&P1Sensitivity,sizeof(float),1,fp);
	fread(&P2Sensitivity,sizeof(float),1,fp);
	fread(&gameTimeLimit,sizeof(float),1,fp);
	fread(&roundToWin,sizeof(float),1,fp);

	fclose(fp);

	//�ǂݍ��񂾊e�l���Z�b�g
	m_BGMVolume.SetValue(BGMVolume);
	m_SEVolume.SetValue(SEVolume);
	m_P1Sensitivity.SetValue(P1Sensitivity);
	m_P2Sensitivity.SetValue(P2Sensitivity);
	m_gameTimeLimit.SetValue(gameTimeLimit);
	m_roundToWin.SetValue(roundToWin);

	//���ʂ͓ǂݍ��񂾎��ɃZ�b�g
	CSoundEngine::GetInstance()->SetBGMVolume(m_BGMVolume.GetValue());
	CSoundEngine::GetInstance()->SetSEVolume(m_SEVolume.GetValue());

	return true;
}

bool GameOption::WriteOption()
{
	FILE* fp = fopen("gameoption.dat", "wb");

	if (fp == nullptr)
	{
		return false;
	}

	//OptionValue���璼�ڏ������߂Ȃ������̂ň�U���[�J���ϐ��Ɋi�[
	float BGMVolume, SEVolume, P1Sensitivity, P2Sensitivity, gameTimeLimit, roundToWin;

	BGMVolume = m_BGMVolume.GetValue();
	SEVolume = m_SEVolume.GetValue();
	P1Sensitivity = m_P1Sensitivity.GetValue();
	P2Sensitivity = m_P2Sensitivity.GetValue();
	gameTimeLimit = m_gameTimeLimit.GetValue();
	roundToWin = m_roundToWin.GetValue();

	//��������
	fwrite(&BGMVolume, sizeof(float), 1, fp);
	fwrite(&SEVolume, sizeof(float), 1, fp);
	fwrite(&P1Sensitivity, sizeof(float), 1, fp);
	fwrite(&P2Sensitivity, sizeof(float), 1, fp);
	fwrite(&gameTimeLimit, sizeof(float), 1, fp);
	fwrite(&roundToWin, sizeof(float), 1, fp);

	fclose(fp);

	return true;
}

void GameOption::Init()
{
	if (m_isInited == true)
	{
		return;
	}

	//BGM
	m_BGMVolume.Init(OPTION_VALUE_CHANGERATE, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX, OptionValue::enPress);
	m_BGMVolume.SetValue(1.0f);
	//SE
	m_SEVolume.Init(OPTION_VALUE_CHANGERATE, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX, OptionValue::enPress);
	m_SEVolume.SetValue(1.0f);
	//�v���C���[1�̃J�������x
	m_P1Sensitivity.Init(OPTION_VALUE_CHANGERATE, CONTROL_SENSITIVITY_MIN, CONTROL_SENSITIVITY_MAX, OptionValue::enPress);
	m_P1Sensitivity.SetValue(2.0f);
	//�v���C���[2�̃J�������x
	m_P2Sensitivity.Init(OPTION_VALUE_CHANGERATE, CONTROL_SENSITIVITY_MIN, CONTROL_SENSITIVITY_MAX, OptionValue::enPress);
	m_P2Sensitivity.SetValue(2.0f);
	//�Q�[���̐�������
	m_gameTimeLimit.Init(OPTION_GAMETIME_CHANGERATE, GAME_TIMELIMIT_MIN, GAME_TIMELIMIT_MAX, OptionValue::enTrigger);
	m_gameTimeLimit.SetValue(40.0f);
	//�����ɕK�v�ȃ��E���h��
	m_roundToWin.Init(OPTION_ROUNDTOWIN_CHANGERATE, GAME_ROUNDTOWIN_MIN, GAME_ROUNDTOWIN_MAX, OptionValue::enTrigger);
	m_roundToWin.SetValue(1.0f);

	m_isInited = true;
}