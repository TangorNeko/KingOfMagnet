#pragma once

class Player;
class CDirectionLight;
class BackGround;
class SkyBoard;

class RoundCounter;

class GameScene : public IGameObject
{
	~GameScene();
	bool Start() override;
	void Update() override;

public:
	enum GameState
	{
		enBirdseye,			//���ՃJ�������
		enStartCountDown,	//�X�^�[�g�J�E���g�_�E��
		enPlaying,			//�v���C��
		enPause,			//�|�[�Y��
		enResult,			//���U���g��
		enDraw
	};

	//�^�C�����~�b�g�ŕ\������X�v���C�g�̌���
	enum TimeLimitSpriteDigit
	{
		enOnesPlaceOfSingleDigit,	//�ꌅ�\�����̈ꌅ(��:�c��6�b�̎���6)
		enOnesPlaceOfDoubleDigit,	//�񌅕\�����̈ꌅ(��:�c��15�b�̎���5)
		enTenthPlaceOfDoubleDigit,	//�񌅕\�����̓�(��:�c��21�b�̎���2)
	};

private:
	/**
	 * @brief �Q�[���X�^�[�g�O�̃J�E���g�_�E��
	*/
	void StartCountDown();

	/**
	 * @brief ���҂𔻒肷��
	*/
	void WinnerJudge();

	/**
	 * @brief ���Ԑ؂�̃J�E���g�_�E��
	*/
	void FinalCount();

	/**
	 * @brief �c�莞�Ԃ̕\��
	*/
	void TimeLimitCount();

	/**
	 * @brief �c�莞�Ԃ̕\���ɔ����X�v���C�g�̕ω�
	 * @param num �X�v���C�g�ɂ���������
	 * @param numPlace �X�v���C�g�ɂ����������̌���(�ꌅ�Ȃ��0�A�񌅂Ȃ�΁A���鐔������̈ʂȂ�1���A�\�̈ʂȂ�2������)
	*/
	void TimeLimitChangesSprits(int num, TimeLimitSpriteDigit numPlace);

public:

	/**
	 * @brief �Q�[���̏�Ԃ�ݒ�
	 * @param gamestate �Q�[���̏��
	*/
	void SetGameState(GameState gamestate)
	{
		m_gameState = gamestate;
	}

	/**
	 * @brief �Q�[���̏�Ԃ��擾
	 * @return �Q�[���̏��
	*/
	GameState GetGameState() const
	{
		return m_gameState;
	}

	/**
	 * @brief ���݂̃Q�[���V�[���ɑ��݂���e���̃J�E���g�𑝂₷(�e�A�O���l�[�h����������ɌĂ�)
	*/
	void BulletIncrease()
	{
		m_bulletNum++;
	}

	/**
	 * @brief ���݂̃Q�[���V�[���ɑ��݂���e���̃J�E���g�����炷(�e�A�O���l�[�h�����������ɌĂ�)
	*/
	void BulletDecrease()
	{
		m_bulletNum--;
	}

	/**
	 * @brief ���݂̃Q�[���V�[���ɑ��݂���e�����擾
	 * @return �Q�[���V�[���ɑ��݂���e��
	*/
	int GetBulletNum() const
	{
		return m_bulletNum;
	}
	
	/**
	 * @brief P1�̃J�������x�ݒ�
	 * @param sensitivity �v���C���[1�̃J�������x
	*/
	void SetP1Sensitivity(float sensitivity)
	{
		m_P1Sensitivity = sensitivity;
	}

	/**
	 * @brief P2�̃J�������x�ݒ�
	 * @param sensitivity �v���C���[2�̃J�������x
	*/
	void SetP2Sensitivity(float sensitivity)
	{
		m_P2Sensitivity = sensitivity;
	}

	/**
	 * @brief �Q�[���̐������Ԃ̐ݒ�
	 * @param timeLimit ��������
	*/
	void SetTimeLimit(float timeLimit)
	{
		m_maxTimeLimit = timeLimit;
	}

	/**
	 * @brief �Q�[���V�[����BGM���X�g�b�v
	*/
	void StopGameBGM()
	{
		m_gameBGM->Stop();
	}

	/**
	 * @brief �X�L�b�v�p�t�H���g���폜����
	*/
	void DeleteSkipFont()
	{
		if (m_skipFontRender != nullptr)
		{
			DeleteGO(m_skipFontRender);
			m_skipFontRender = nullptr;
		}
	}

	const int MAXBULLETNUM = 60;								//�Q�[���V�[���ɑ��݂ł���e���̍ő吔
private:
	GameState m_gameState = enBirdseye;							//�Q�[���̏�ԁ@���񂵂���X�^�[�g

	prefab::CFontRender* m_skipFontRender = nullptr;			//���񂵃J�����̃X�L�b�v���ł��鎖��������t�H���g

	Player* m_player1 = nullptr;								//�v���C���[1
	static float m_P1Sensitivity;								//�v���C���[1�̃J�������x
	Player* m_player2 = nullptr;								//�v���C���[2
	static float m_P2Sensitivity;								//�v���C���[2�̃J�������x

	BackGround* m_backGround = nullptr;							//�X�e�[�W�̃��f��
	SkyBoard* m_sky = nullptr;									//��̔|��

	prefab::CDirectionLight* m_stageLight = nullptr;			//�X�e�[�W���Ƃ炷���C�g

	prefab::CSoundSource* m_gameBGM = nullptr;					//�Q�[������BGM

	int m_bulletNum = 0;										//���݃Q�[���V�[���ɑ��݂���e�A�O���l�[�h�̑���;

	prefab::CSpriteRender* m_delimitLineSpriteRender = nullptr;	//���E�̉�ʂ���؂钌�̃X�v���C�g
	prefab::CSpriteRender* m_HPCoverSpriteRender = nullptr;		//HP�̃J�o�[�̃X�v���C�g
	prefab::CSpriteRender* m_TimerBaseSpriteRender = nullptr;	//�^�C�}�[�̕\���p�l���̃X�v���C�g

	static float m_maxTimeLimit;								//�Q�[���̍ő吧������
	float m_timeLimit = 40.0f;									//�Q�[���̌��݂̐�������
	int m_oldTimeLimit = 0;										//�O�t���[���̃Q�[���̐�������
	prefab::CSpriteRender* m_onesPlaceSpriteRender = nullptr;	//�������Ԃ̈ꌅ�ڂ�\������X�v���C�g
	prefab::CSpriteRender* m_tensPlaceSpriteRender = nullptr;	//�������Ԃ̓񌅖ڂ�\������X�v���C�g

	int m_gameEndCount = 0;										//�Q�[���̏I�����m�肵�Ă���̌o�ߎ���

	float m_measureSecond = 0.0f;								//�Q�[���X�^�[�g�̃J�E���g����1�b�𑪂�p
	int m_startCount = 5;										//�Q�[���X�^�[�g�܂ł̃J�E���g
	prefab::CSpriteRender* m_startSprite = nullptr;				//�Q�[���X�^�[�g�̃X�v���C�g
	bool m_playCountSEFlag = false;								//�J�E���g�_�E���̉���炷?
	bool m_isGameEndFirstFrame = true;							//�Q�[�����I����������̃t���[����?

	//�J�n�J�E���g�_�E��
	bool m_startCount3_Flag = false;							//�J�n3�b�O�̃J�E���g�_�E����\������?
	bool m_startCount2_Flag = false;							//�J�n2�b�O�̃J�E���g�_�E����\������?
	bool m_startCount1_Flag = false;							//�J�n1�b�O�̃J�E���g�_�E����\������?
	prefab::CSpriteRender* m_startCountDown_3_Top = nullptr;	//3�b�O�̃J�E���g�_�E���̏㑤�̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_3_Middle = nullptr;	//3�b�O�̃J�E���g�_�E���̒����̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_3_Bottom = nullptr;	//3�b�O�̃J�E���g�_�E���̉����̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_3_Right = nullptr;	//3�b�O�̃J�E���g�_�E���̉E���̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_2_Top = nullptr;	//2�b�O�̃J�E���g�_�E���̏㑤�̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_2_Middle = nullptr;	//2�b�O�̃J�E���g�_�E���̒����̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_2_Bottom = nullptr;	//2�b�O�̃J�E���g�_�E���̉����̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_2_Right = nullptr;	//2�b�O�̃J�E���g�_�E���̉E���̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_2_Left = nullptr;	//2�b�O�̃J�E���g�_�E���̍����̃X�v���C�g
	prefab::CSpriteRender* m_startCountDown_1 = nullptr;		//1�b�O�̃J�E���g�_�E���̃X�v���C�g

	//�I���J�E���g�_�E��
	bool m_finalCount3_Flag = false;							//�I��3�b�O�̃J�E���g�_�E����\������?
	bool m_finalCount2_Flag = false;							//�I��2�b�O�̃J�E���g�_�E����\������?
	bool m_finalCount1_Flag = false;							//�I��1�b�O�̃J�E���g�_�E����\������?
	prefab::CSpriteRender* m_finalCountDown = nullptr;			//�I���̃J�E���g�_�E����\������X�v���C�g

	prefab::CFontRender* m_drawFontRender = nullptr;			//���������̃t�H���g
	int m_drawFontCount = 300;									//���������̃t�H���g��\�����鎞��

	RoundCounter* m_roundCounter = nullptr;						//�v���C���[�̎擾���E���h���̊Ǘ��N���X
};

