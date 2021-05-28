#pragma once

class GameOption : public IGameObject
{
	~GameOption();
	bool Start() override;
	void Update() override;

	prefab::CSpriteRender* m_optionBackGroundSprite = nullptr;//�w�i�X�v���C�g

	bool isOpen = false;//�I�v�V�������J���Ă��邩

	static float m_BGMVolume;//BGM�̃{�����[��
	static float m_SEVolume;//���ʉ��̃{�����[��
	static float m_1PSensitivity;//�v���C���[1�̃J�������x
	static float m_2PSensitivity;//�v���C���[2�̃J�������x
	float m_selectItemTemporaryValue;//�I���������ڂ̒l��ۑ����Ă���(�L�����Z���������ɖ߂���悤)

	prefab::CFontRender* m_optionFont = nullptr;//�I�v�V�����̃t�H���g
	prefab::CFontRender* m_BGMVolumeFont = nullptr;//BGM�̃{�����[���̃t�H���g
	prefab::CFontRender* m_SEVolumeFont = nullptr;//���ʉ��̃{�����[���̃t�H���g
	prefab::CFontRender* m_1PSensitivityFont = nullptr;//�v���C���[1�̃J�������x�̃t�H���g
	prefab::CFontRender* m_2PSensitivityFont = nullptr;//�v���C���[2�̃J�������x�̃t�H���g

public:
	//���݂̑I��[�x
	enum SelectingState
	{
		enItem,//�I�v�V��������
		enNumeric//���ڂ̐��l
	};

private:
	//�I��[�x
	SelectingState m_selectingState = enItem;

	//�I�����Ă��鍀�ڔԍ�(0:BGM,1:SE,2:1P�J�������x,3:2P�J�������x)
	int m_selectingItem = 0;

	//�I�����Ă��鍀�ڂ̐��l���i�[����|�C���^
	float* m_selectingItemValue = nullptr;

	//�I�����Ă��鍀�ڂ̃t�H���g���i�[����|�C���^
	prefab::CFontRender* m_selectingItemFont = nullptr;

	//�t�H���g��SetText�ɓn�����̕ϊ��p�o�b�t�@
	wchar_t m_buffer[256];
public:
	//�I�v�V�������J��
	void Open();

	//�I�v�V���������
	void Close();

	//BGM�{�����[���̐��l���擾
	float GetBGMVolume() { return m_BGMVolume; }

	//SE�{�����[���̐��l���擾
	float GetSEVolume() { return m_SEVolume; }

	//1P�̃J�������x�̐��l���擾
	float Get1PSensitivity() { return m_1PSensitivity; }

	//2P�̃J�������x�̐��l���擾
	float Get2PSensitivity() { return m_2PSensitivity; }

	SelectingState GetSelectingState() { return m_selectingState; }
};

