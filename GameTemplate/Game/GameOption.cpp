#include "stdafx.h"
#include "GameOption.h"

GameOption::~GameOption()
{
	//�X�v���C�g�A�t�H���g�̍폜
	DeleteGO(m_optionBackGroundSprite);
	DeleteGO(m_BGMVolumeFont);
	DeleteGO(m_SEVolumeFont);
	DeleteGO(m_1PSensitivityFont);
	DeleteGO(m_2PSensitivityFont);
}

bool GameOption::Start()
{
	//�w�i�X�v���C�g
	m_optionBackGroundSprite = NewGO<prefab::CSpriteRender>(0);
	m_optionBackGroundSprite->Init("Assets/Image/White.dds", 960, 540);
	m_optionBackGroundSprite->SetScale({ 0.0f,0.0f,0.0f });

	//�I�v�V�����t�H���g
	m_optionFont = NewGO<prefab::CFontRender>(0);
	m_optionFont->SetScale({ 0.0f,0.0f });
	m_optionFont->SetPosition({ -125.0f,220.0f });
	m_optionFont->SetText(L"OPTION");

	//BGM�{�����[���t�H���g
	m_BGMVolumeFont = NewGO<prefab::CFontRender>(0);
	m_BGMVolumeFont->SetScale({ 0.0f,0.0f });
	m_BGMVolumeFont->SetPosition({ -450.0f,120.0f });


	//SE�{�����[���t�H���g
	m_SEVolumeFont = NewGO<prefab::CFontRender>(0);
	m_SEVolumeFont->SetScale({ 0.0f,0.0f });
	m_SEVolumeFont->SetPosition({ -450.0f,20.0f });


	//1P���슴�x�t�H���g
	m_1PSensitivityFont = NewGO<prefab::CFontRender>(0);
	m_1PSensitivityFont->SetScale({ 0.0f,0.0f });
	m_1PSensitivityFont->SetPosition({ -450.0f,-80.0f });


	//2P���슴�x�t�H���g
	m_2PSensitivityFont = NewGO<prefab::CFontRender>(0);
	m_2PSensitivityFont->SetScale({ 0.0f,0.0f });
	m_2PSensitivityFont->SetPosition({ -450.0f,-180.0f });
	return true;
}

void GameOption::Update()
{
	//�I�v�V�������J���Ă��āA�g�傪�I����Ă���B
	if (m_optionBackGroundSprite->m_spriteSupporter.GetSpriteScaleListLen() == 0 && isOpen == true)
	{
		//�w�i���D�F��
		m_optionBackGroundSprite->SetMulColor({ 0.0f,0.75f,0.75f,0.8f });

		//�t�H���g�̊g�嗦��1�{�ɂ��邱�Ƃŋ[���I�ɕ\��
		m_optionFont->SetScale({ 1.0f,1.0f });
		m_BGMVolumeFont->SetScale({ 1.0f,1.0f });
		m_SEVolumeFont->SetScale({ 1.0f,1.0f });
		m_1PSensitivityFont->SetScale({ 1.0f,1.0f });
		m_2PSensitivityFont->SetScale({ 1.0f,1.0f });

		//�I�������t�H���g�ȊO�𔒂ɂ��邽�߈�U�S�����ɂ��Ă���(switch���̒��őI�����Ă����ȊO�𔒂ɂ���`�ł���������)
		m_BGMVolumeFont->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_SEVolumeFont->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_1PSensitivityFont->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_2PSensitivityFont->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		//���ڂ̑I�𒆂Ȃ�
		if (m_selectingState == enItem)
		{
			//���������Ɖ��̍��ڂ�
			if (g_pad[0]->IsTrigger(enButtonDown))
			{
				if (++m_selectingItem == 4)
				{
					m_selectingItem = 0;
				}
			}
			else if(g_pad[0]->IsTrigger(enButtonUp))//��������Ə�̍��ڂ�
			{
				if (--m_selectingItem == -1)
				{
					m_selectingItem = 3;
				}
			}

			//�I��ł��鍀�ڂ��|�C���^�ɃZ�b�g
			//NOTE:switch���̑��p����������ă|�C���^�ɃZ�b�g�������A�ݒ荀�ڂɂ���Đ��l�̕����Ⴄ�ȏ��������...
			switch (m_selectingItem)
			{
			case 0://BGM�{�����[��
				m_selectingItemValue = &m_BGMVolume;
				m_selectingItemFont = m_BGMVolumeFont;
				break;
			case 1://SE�{�����[��
				m_selectingItemValue = &m_SEVolume;
				m_selectingItemFont = m_SEVolumeFont;
				break;
			case 2://1P�J�������x
				m_selectingItemValue = &m_1PSensitivity;
				m_selectingItemFont = m_1PSensitivityFont;
				break;
			case 3://2P�J�������x
				m_selectingItemValue = &m_2PSensitivity;
				m_selectingItemFont = m_2PSensitivityFont;
				break;
			}

			//�I������Ă��鍀�ڂ�ԍ��F��
			m_selectingItemFont->SetColor({ 0.5f,0.0f,0.0f,1.0f });

			//A�{�^���������ƑI�����Ă��鍀�ڂ̐��l��ݒ肷�郂�[�h�Ɉڍs
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_selectingState = enNumeric;

				//���l�ݒ�O�̒l���L�^
				m_selectItemTemporaryValue = *m_selectingItemValue;
			}
		}
		else if(m_selectingState == enNumeric)//���ڂ̐��l��ݒ肷�郂�[�h�Ȃ�
		{
			//���������Ɛ��l�����炷
			if (g_pad[0]->IsPress(enButtonDown))
			{
				(*m_selectingItemValue) -= 0.1f;
			}
			else if(g_pad[0]->IsPress(enButtonUp))//��������Ɛ��l�𑝂₷
			{
				(*m_selectingItemValue) += 0.1f;
			}

			//�I������Ă��鍀�ڂ���F��
			m_selectingItemFont->SetColor({ 0.0f,0.0f,0.5f,1.0f });

			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_selectingState = enItem;
			}
			else if (g_pad[0]->IsTrigger(enButtonB))
			{
				m_selectingState = enItem;

				*m_selectingItemValue = m_selectItemTemporaryValue;
			}
		}

		//�ύX���ꂽ���l�����Ƀt�H���g�̕`�敶�����ύX
		swprintf_s(m_buffer, L"BGMVOLUME = %.2f", m_BGMVolume);
		m_BGMVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"SEVOLUME = %.2f", m_SEVolume);
		m_SEVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"1PSENSITIVITY = %.2f", m_1PSensitivity);
		m_1PSensitivityFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"2PSENSITIVITY = %.2f", m_2PSensitivity);
		m_2PSensitivityFont->SetText(m_buffer);

	}
	else//����ȊO
	{
		//�w�i�𔒐F��
		m_optionBackGroundSprite->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });

		//�t�H���g�̊g�嗦��0�{�ɂ��邱�Ƃŋ[���I�ɏ���
		m_optionFont->SetScale({ 0.0f,0.0f });
		m_BGMVolumeFont->SetScale({ 0.0f,0.0f });
		m_SEVolumeFont->SetScale({ 0.0f,0.0f });
		m_1PSensitivityFont->SetScale({ 0.0f,0.0f });
		m_2PSensitivityFont->SetScale({ 0.0f,0.0f });
	}
}

void GameOption::Open()
{
	//�w�i�̊g�嗦��1�{�ɂ���L���[���Z�b�g
	m_optionBackGroundSprite->m_spriteSupporter.SpriteScale(Vector3::One, 12, 1);

	//�J�����t���O���I��
	isOpen = true;
}

void GameOption::Close()
{
	//�w�i�̊g�嗦��0�{�ɂ���L���[���Z�b�g
	m_optionBackGroundSprite->m_spriteSupporter.SpriteScale(Vector3::Zero, 12, 1);

	//�J�����t���O���I�t
	isOpen = false;

	//�I�����ڂ����Z�b�g
	m_selectingItem = 0;
}