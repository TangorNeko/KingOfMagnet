#pragma once
#include "OptionValue.h"

class GameOption : public IGameObject
{
	~GameOption() override;
	bool Start() override;
	void Update() override;
public:
	//���݂̑I��[�x
	enum SelectingState
	{
		enItem,		//�I�v�V��������
		enNumeric	//���ڂ̐��l
	};

public:
	/**
	 * @brief �I�v�V������ʂ��J��
	*/
	void Open();

	/**
	 * @brief �I�v�V������ʂ����
	*/
	void Close();

	/**
	 * @brief BGM�S�̂̃{�����[�����擾
	 * @return BGM�S�̂̃{�����[��
	*/
	float GetBGMVolume() const { return m_BGMVolume.GetValue(); }

	/**
	 * @brief SE�S�̂̃{�����[�����擾
	 * @return SE�S�̂̃{�����[��
	*/
	float GetSEVolume() const { return m_SEVolume.GetValue(); }

	/**
	 * @brief �v���C���[1�̃J�������x�̐��l���擾
	 * @return �v���C���[1�̃J�������x
	*/
	float GetP1Sensitivity() const { return m_P1Sensitivity.GetValue(); }

	/**
	 * @brief �v���C���[2�̃J�������x�̐��l���擾
	 * @return �v���C���[2�̃J�������x
	*/
	float GetP2Sensitivity() const { return m_P2Sensitivity.GetValue(); }

	/**
	 * @brief �Q�[���̐������Ԃ̐��l���擾
	 * @return �Q�[���̐�������
	*/
	float GetTimeLimit() const { return m_gameTimeLimit.GetValue(); }

	/**
	 * @brief �����ɕK�v�ȃ��E���h�����擾
	 * @return �����ɕK�v�ȃ��E���h��
	*/
	int GetRoundToWin() const { return static_cast<int>(m_maxRound.GetValue()); }

	/**
	 * @brief ���݂̃I�v�V������ʂ̑I��[�x���擾
	 * @return �I��[�x
	*/
	SelectingState GetSelectingState() const { return m_selectingState; }

	/**
	 * @brief �w�i�X�v���C�g���܂��g��������͏k������?
	 * @return �g��������͏k�����Ȃ�true
	*/
	bool isQueuing() const
	{
		return m_optionBackGroundSprite->GetSpriteSupporter().GetSpriteScaleListLen() != 0;
	}

	/**
	 * @brief �t�@�C������I�v�V�����ǂݍ���
	 * @return ����?
	*/
	static bool ReadOption();

	/**
	 * @brief �t�@�C���ɃI�v�V������������
	 * @return ����?
	*/
	bool WriteOption();

	/**
	 * @brief �ݒ�̏�����
	*/
	static void Init();

private:
	prefab::CSpriteRender* m_optionBackGroundSprite = nullptr;	//�w�i�X�v���C�g
	bool m_isOpen = false;										//�I�v�V�������J���Ă��邩								
	static OptionValue m_BGMVolume;								//BGM�̃{�����[��					
	static OptionValue m_SEVolume;								//���ʉ��̃{�����[��							
	static OptionValue m_P1Sensitivity;							//�v���C���[1�̃J�������x							
	static OptionValue m_P2Sensitivity;							//�v���C���[2�̃J�������x							
	static OptionValue m_gameTimeLimit;							//�Q�[���̐�������								
	static OptionValue m_maxRound;								//�����ɕK�v�ȃ��E���h��
	prefab::CFontRender* m_optionFont = nullptr;				//�I�v�V�����̃t�H���g
	prefab::CFontRender* m_BGMVolumeFont = nullptr;				//BGM�̃{�����[���̃t�H���g
	prefab::CFontRender* m_SEVolumeFont = nullptr;				//���ʉ��̃{�����[���̃t�H���g
	prefab::CFontRender* m_1PSensitivityFont = nullptr;			//�v���C���[1�̃J�������x�̃t�H���g
	prefab::CFontRender* m_2PSensitivityFont = nullptr;			//�v���C���[2�̃J�������x�̃t�H���g
	prefab::CFontRender* m_gameTimeLimitFont = nullptr;			//�Q�[���̐������Ԃ̃t�H���g
	prefab::CFontRender* m_maxRoundFont = nullptr;				//�����ɕK�v�ȃ��E���h���̃t�H���g
	SelectingState m_selectingState = enItem;					//�I��[�x
	int m_selectingItem = 0;									//�I�����Ă��鍀�ڔԍ�(0:BGM,1:SE,2:1P�J�������x,3:2P�J�������x,4:�Q�[����������,5:�����ɕK�v�ȃ��E���h��)
	OptionValue* m_selectingItemValue = nullptr;				//�I�����Ă��鍀�ڂ��i�[����|�C���^
	float m_selectingItemTemporaryValue = 1.0f;					//�I���������ڂ̒l��ۑ����Ă���(�L�����Z���������ɖ߂���悤)
	prefab::CFontRender* m_selectingItemFont = nullptr;			//�I�����Ă��鍀�ڂ̃t�H���g���i�[����|�C���^
	wchar_t m_buffer[256] = L"";								//�t�H���g��SetText�ɓn�����̕ϊ��p�o�b�t�@
	static bool m_isInited;										//���������ꂽ��
};

