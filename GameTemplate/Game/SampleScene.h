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

	
	Player* m_player1 = nullptr;
	Player* m_player2 = nullptr;

	prefab::CDirectionLight* m_stageLight = nullptr;
	BackGround* m_backGround = nullptr;
	SkyBoard* m_sky = nullptr;

	bool m_gameEndFlag = false;
	int m_gameEndCount = 0;

	bool m_gamePauseFlag = false;

	//�Q�[���X�^�[�g�J�E���g�֘A
	void StartCountDown();
	float m_measureSecond = 0.0f;	//�b�𑪂�
	int m_startCount = 7;		//�Q�[���X�^�[�g�܂ł̃J�E���g
	prefab::CFontRender* m_startCountFontRender = nullptr;
	bool m_isGameStart = false;
	bool m_playCountSEFlag = false;
	bool m_GEfirstLoop = true;

	int m_bulletNum = 0;//�Q�[���V�[���ɑ��݂���e�A�O���l�[�h�̑���;
public:

	void SetGameEndFlag(bool flag)
	{
		m_gameEndFlag = flag;
	}

	bool GetGameEndFlag()
	{
		return m_gameEndFlag;
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

	
	//��������
	prefab::CFontRender* m_timeFontRender = nullptr;
	float m_timeLimit = 100;
	float m_deltaTimeCount = 0.0f;

	prefab::CSpriteRender* m_delimitLineSpriteRender = nullptr;	//��؂��
	prefab::CSpriteRender* m_HPCoverSpriteRender = nullptr;	
	prefab::CSpriteRender* m_TimerBaseSpriteRender = nullptr;

	//BGM
	prefab::CSoundSource* ssBGM = nullptr;
};

