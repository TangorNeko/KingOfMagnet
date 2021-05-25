#pragma once

class Player;
class CDirectionLight;
class BackGround;
class SkyBoard;

class SampleScene : public IGameObject
{
	~SampleScene();
	bool Start() override;
	void Update() override;

	//���Ҕ���
	void WinnerJudge();

	//�^�C�����~�b�g�֘A
	float m_timeLimit = 100.0f;
	float m_deltaTimeCount = 0.0f;
	void TimeLimitCount();
	void TimeLimitChangesSprits(int num, int numPlace);	//num�̓X�v���C�g�ɂ����������AnumPlace�͈ꌅ�Ȃ��0�𑗂�A�񌅂Ȃ�Α��鐔������̈ʂȂ�1���A�\�̂��炢�Ȃ�10�𑗂�B
	int m_oldTimeLimit = 0;
	prefab::CSpriteRender* m_onesPlaceSpriteRender = nullptr;	//�^�C�����~�b�g�̈ꌅ�ڂ�\������X�v���C�g
	prefab::CSpriteRender* m_tensPlaceSpriteRender = nullptr;	//�^�C�����~�b�g�̓񌅖ڂ�\������X�v���C�g

	Player* m_player1 = nullptr;
	Player* m_player2 = nullptr;

	prefab::CDirectionLight* m_stageLight = nullptr;
	BackGround* m_backGround = nullptr;
	SkyBoard* m_sky = nullptr;

	int m_gameEndCount = 0;
	int m_gameEndLoop = 0;
	//�Q�[���X�^�[�g�J�E���g�֘A
	void StartCountDown();
	float m_measureSecond = 0.0f;	//�b�𑪂�
	int m_startCount = 4;		//�Q�[���X�^�[�g�܂ł̃J�E���g
	//prefab::CFontRender* m_startCountFontRender = nullptr;
	bool m_playCountSEFlag = false;
	bool m_GEfirstLoop = true;

	int m_bulletNum = 0;//�Q�[���V�[���ɑ��݂���e�A�O���l�[�h�̑���;

	prefab::CSpriteRender* m_delimitLineSpriteRender = nullptr;	//��؂��
	prefab::CSpriteRender* m_HPCoverSpriteRender = nullptr;
	prefab::CSpriteRender* m_TimerBaseSpriteRender = nullptr;

	prefab::CFontRender* m_drawFontRender = nullptr;
	int m_drawFontCount = 300;

	//�J�E���g�_�E��
	bool m_count3_Flag = false;
	bool m_count2_Flag = false;
	bool m_count1_Flag = false;
	prefab::CSpriteRender* m_CountDown_3_1 = nullptr;
	prefab::CSpriteRender* m_CountDown_3_2 = nullptr;
	prefab::CSpriteRender* m_CountDown_3_3 = nullptr;
	prefab::CSpriteRender* m_CountDown_3_4 = nullptr;
	//
	prefab::CSpriteRender* m_CountDown_2_1 = nullptr;
	prefab::CSpriteRender* m_CountDown_2_2 = nullptr;
	prefab::CSpriteRender* m_CountDown_2_3 = nullptr;
	prefab::CSpriteRender* m_CountDown_2_4 = nullptr;
	prefab::CSpriteRender* m_CountDown_2_5 = nullptr;
	//
	prefab::CSpriteRender* m_CountDown_1_1 = nullptr;


	//BGM
	prefab::CSoundSource* ssBGM = nullptr;
public:
	enum GameState
	{
		enBirdseye,//���ՃJ�������
		enStartCountDown,//�X�^�[�g�J�E���g�_�E��
		enPlaying,//�v���C��
		enPause,//�|�[�Y��
		enResult,//���U���g��
		enDraw
	};

private:

	GameState m_gameState = enBirdseye;

public:

	void SetGameState(GameState gamestate)
	{
		m_gameState = gamestate;
	}

	GameState GetGameState()
	{
		return m_gameState;
	}

	//���݂̃Q�[���V�[���ɑ��݂���e���𑝂₷(�e�A�O���l�[�h����������ɌĂ�)
	void BulletIncrease()
	{
		m_bulletNum++;
	}

	//���݂̃Q�[���V�[���ɑ��݂���e�������炷(�e�A�O���l�[�h�����������ɌĂ�)
	void BulletDecrease()
	{
		m_bulletNum--;
	}

	//���݂̃Q�[���V�[���ɑ��݂���e�����擾
	int GetBulletNum()
	{
		return m_bulletNum;
	}

	//�Q�[���V�[���ɑ��݂ł���e���̍ő吔
	const int MAXBULLETNUM = 30;
};

