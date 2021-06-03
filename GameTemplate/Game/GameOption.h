#pragma once

class GameOption : public IGameObject
{
	~GameOption();
	bool Start() override;
	void Update() override;
public:
	//���݂̑I��[�x
	enum SelectingState
	{
		enItem,//�I�v�V��������
		enNumeric//���ڂ̐��l
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
	float GetBGMVolume() { return m_BGMVolume; }

	/**
	 * @brief SE�S�̂̃{�����[�����擾
	 * @return SE�S�̂̃{�����[��
	*/
	float GetSEVolume() { return m_SEVolume; }

	/**
	 * @brief �v���C���[1�̃J�������x�̐��l���擾
	 * @return �v���C���[1�̃J�������x
	*/
	float GetP1Sensitivity() { return m_1PSensitivity; }

	/**
	 * @brief �v���C���[2�̃J�������x�̐��l���擾
	 * @return �v���C���[2�̃J�������x
	*/
	float GetP2Sensitivity() { return m_2PSensitivity; }

	/**
	 * @brief ���݂̃I�v�V������ʂ̑I��[�x���擾
	 * @return �I��[�x
	*/
	SelectingState GetSelectingState() { return m_selectingState; }

	/**
	 * @brief �w�i�X�v���C�g���܂��g��������͏k������?
	 * @return �g��������͏k�����Ȃ�true
	*/
	bool isQueuing()
	{
		return m_optionBackGroundSprite->GetSpriteSupporter().GetSpriteScaleListLen() != 0;
	}

private:
	prefab::CSpriteRender* m_optionBackGroundSprite = nullptr;//�w�i�X�v���C�g
	bool m_isOpen = false;//�I�v�V�������J���Ă��邩
	static float m_BGMVolume;//BGM�̃{�����[��
	static float m_SEVolume;//���ʉ��̃{�����[��
	static float m_1PSensitivity;//�v���C���[1�̃J�������x
	static float m_2PSensitivity;//�v���C���[2�̃J�������x
	prefab::CFontRender* m_optionFont = nullptr;//�I�v�V�����̃t�H���g
	prefab::CFontRender* m_BGMVolumeFont = nullptr;//BGM�̃{�����[���̃t�H���g
	prefab::CFontRender* m_SEVolumeFont = nullptr;//���ʉ��̃{�����[���̃t�H���g
	prefab::CFontRender* m_1PSensitivityFont = nullptr;//�v���C���[1�̃J�������x�̃t�H���g
	prefab::CFontRender* m_2PSensitivityFont = nullptr;//�v���C���[2�̃J�������x�̃t�H���g
	SelectingState m_selectingState = enItem;//�I��[�x
	int m_selectingItem = 0;//�I�����Ă��鍀�ڔԍ�(0:BGM,1:SE,2:1P�J�������x,3:2P�J�������x)
	float* m_selectingItemValue = nullptr;//�I�����Ă��鍀�ڂ̐��l���i�[����|�C���^
	float m_selectingItemTemporaryValue = 1.0f;//�I���������ڂ̒l��ۑ����Ă���(�L�����Z���������ɖ߂���悤)
	prefab::CFontRender* m_selectingItemFont = nullptr;//�I�����Ă��鍀�ڂ̃t�H���g���i�[����|�C���^
	wchar_t m_buffer[256] = L"";//�t�H���g��SetText�ɓn�����̕ϊ��p�o�b�t�@
};

