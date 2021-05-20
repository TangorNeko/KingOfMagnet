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

	//勝者判定
	void WinnerJudge();

	
	Player* m_player1 = nullptr;
	Player* m_player2 = nullptr;

	prefab::CDirectionLight* m_stageLight = nullptr;
	BackGround* m_backGround = nullptr;
	SkyBoard* m_sky = nullptr;

	int m_gameEndCount = 0;

	//ゲームスタートカウント関連
	void StartCountDown();
	float m_measureSecond = 0.0f;	//秒を測る
	int m_startCount = 3;		//ゲームスタートまでのカウント
	prefab::CFontRender* m_startCountFontRender = nullptr;
	bool m_playCountSEFlag = false;
	bool m_GEfirstLoop = true;

	int m_bulletNum = 0;//ゲームシーンに存在する弾、グレネードの総数;

	//制限時間
	prefab::CFontRender* m_timeFontRender = nullptr;
	float m_timeLimit = 100;
	float m_deltaTimeCount = 0.0f;

	prefab::CSpriteRender* m_delimitLineSpriteRender = nullptr;	//区切る線
	prefab::CSpriteRender* m_HPCoverSpriteRender = nullptr;
	prefab::CSpriteRender* m_TimerBaseSpriteRender = nullptr;

	//BGM
	prefab::CSoundSource* ssBGM = nullptr;
public:
	enum GameState
	{
		enBirdseye,//俯瞰カメラ状態
		enStartCountDown,//スタートカウントダウン
		enPlaying,//プレイ中
		enPause,//ポーズ中
		enResult,//リザルト中
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

	//現在のゲームシーンに存在する弾数を増やす(弾、グレネードを作った時に呼ぶ)
	void BulletIncrease()
	{
		m_bulletNum++;
	}

	//現在のゲームシーンに存在する弾数を減らす(弾、グレネードが消えた時に呼ぶ)
	void BulletDecrease()
	{
		m_bulletNum--;
	}

	//現在のゲームシーンに存在する弾数を取得
	int GetBulletNum()
	{
		return m_bulletNum;
	}

	//ゲームシーンに存在できる弾数の最大数
	const int MAXBULLETNUM = 30;
};

