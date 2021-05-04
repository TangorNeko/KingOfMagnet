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

	//ŸÒ”»’è
	void WinnerJudge();

	
	Player* m_player1 = nullptr;
	Player* m_player2 = nullptr;

	prefab::CDirectionLight* m_stageLight = nullptr;
	BackGround* m_backGround = nullptr;
	SkyBoard* m_sky = nullptr;

	bool m_gameEndFlag = false;
	int m_gameEndCount = 0;

	bool m_gamePauseFlag = false;

public:

	void SetGameEndFlag(bool flag)
	{
		m_gameEndFlag = flag;
	}

	bool GetGameEndFlag()
	{
		return m_gameEndFlag;
	}

	
	//§ŒÀŠÔ
	prefab::CFontRender* m_timeFontRender = nullptr;
	float m_timeLimit = 100;
	float m_deltaTimeCount = 0.0f;

	prefab::CSpriteRender* m_delimitLineSpriteRender = nullptr;	//‹æØ‚éü
	prefab::CSpriteRender* m_HPCoverSpriteRender = nullptr;	
	prefab::CSpriteRender* m_TimerBaseSpriteRender = nullptr;
};

