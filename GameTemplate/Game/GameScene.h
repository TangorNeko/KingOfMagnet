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
		enBirdseye,			//俯瞰カメラ状態
		enStartCountDown,	//スタートカウントダウン
		enPlaying,			//プレイ中
		enPause,			//ポーズ中
		enResult,			//リザルト中
		enDraw
	};

	//タイムリミットで表示するスプライトの桁数
	enum TimeLimitSpriteDigit
	{
		enOnesPlaceOfSingleDigit,	//一桁表示中の一桁(例:残り6秒の時の6)
		enOnesPlaceOfDoubleDigit,	//二桁表示中の一桁(例:残り15秒の時の5)
		enTenthPlaceOfDoubleDigit,	//二桁表示中の二桁(例:残り21秒の時の2)
	};

private:
	/**
	 * @brief ゲームスタート前のカウントダウン
	*/
	void StartCountDown();

	/**
	 * @brief 勝者を判定する
	*/
	void WinnerJudge();

	/**
	 * @brief 時間切れのカウントダウン
	*/
	void FinalCount();

	/**
	 * @brief 残り時間の表示
	*/
	void TimeLimitCount();

	/**
	 * @brief 残り時間の表示に伴うスプライトの変化
	 * @param num スプライトにしたい数字
	 * @param numPlace スプライトにしたい数字の桁数(一桁ならば0、二桁ならば、送る数字が一の位なら1を、十の位なら2を入れる)
	*/
	void TimeLimitChangesSprits(int num, TimeLimitSpriteDigit numPlace);

public:

	/**
	 * @brief ゲームの状態を設定
	 * @param gamestate ゲームの状態
	*/
	void SetGameState(GameState gamestate)
	{
		m_gameState = gamestate;
	}

	/**
	 * @brief ゲームの状態を取得
	 * @return ゲームの状態
	*/
	GameState GetGameState() const
	{
		return m_gameState;
	}

	/**
	 * @brief 現在のゲームシーンに存在する弾数のカウントを増やす(弾、グレネードを作った時に呼ぶ)
	*/
	void BulletIncrease()
	{
		m_bulletNum++;
	}

	/**
	 * @brief 現在のゲームシーンに存在する弾数のカウントを減らす(弾、グレネードが消えた時に呼ぶ)
	*/
	void BulletDecrease()
	{
		m_bulletNum--;
	}

	/**
	 * @brief 現在のゲームシーンに存在する弾数を取得
	 * @return ゲームシーンに存在する弾数
	*/
	int GetBulletNum() const
	{
		return m_bulletNum;
	}
	
	/**
	 * @brief P1のカメラ感度設定
	 * @param sensitivity プレイヤー1のカメラ感度
	*/
	void SetP1Sensitivity(float sensitivity)
	{
		m_P1Sensitivity = sensitivity;
	}

	/**
	 * @brief P2のカメラ感度設定
	 * @param sensitivity プレイヤー2のカメラ感度
	*/
	void SetP2Sensitivity(float sensitivity)
	{
		m_P2Sensitivity = sensitivity;
	}

	/**
	 * @brief ゲームの制限時間の設定
	 * @param timeLimit 制限時間
	*/
	void SetTimeLimit(float timeLimit)
	{
		m_maxTimeLimit = timeLimit;
	}

	/**
	 * @brief ゲームシーンのBGMをストップ
	*/
	void StopGameBGM()
	{
		m_gameBGM->Stop();
	}

	/**
	 * @brief スキップ用フォントを削除する
	*/
	void DeleteSkipFont()
	{
		if (m_skipFontRender != nullptr)
		{
			DeleteGO(m_skipFontRender);
			m_skipFontRender = nullptr;
		}
	}

	const int MAXBULLETNUM = 60;								//ゲームシーンに存在できる弾数の最大数
private:
	GameState m_gameState = enBirdseye;							//ゲームの状態　見回しからスタート

	prefab::CFontRender* m_skipFontRender = nullptr;			//見回しカメラのスキップができる事を教えるフォント

	Player* m_player1 = nullptr;								//プレイヤー1
	static float m_P1Sensitivity;								//プレイヤー1のカメラ感度
	Player* m_player2 = nullptr;								//プレイヤー2
	static float m_P2Sensitivity;								//プレイヤー2のカメラ感度

	BackGround* m_backGround = nullptr;							//ステージのモデル
	SkyBoard* m_sky = nullptr;									//空の板ポリ

	prefab::CDirectionLight* m_stageLight = nullptr;			//ステージを照らすライト

	prefab::CSoundSource* m_gameBGM = nullptr;					//ゲーム中のBGM

	int m_bulletNum = 0;										//現在ゲームシーンに存在する弾、グレネードの総数;

	prefab::CSpriteRender* m_delimitLineSpriteRender = nullptr;	//左右の画面を区切る柱のスプライト
	prefab::CSpriteRender* m_HPCoverSpriteRender = nullptr;		//HPのカバーのスプライト
	prefab::CSpriteRender* m_TimerBaseSpriteRender = nullptr;	//タイマーの表示パネルのスプライト

	static float m_maxTimeLimit;								//ゲームの最大制限時間
	float m_timeLimit = 40.0f;									//ゲームの現在の制限時間
	int m_oldTimeLimit = 0;										//前フレームのゲームの制限時間
	prefab::CSpriteRender* m_onesPlaceSpriteRender = nullptr;	//制限時間の一桁目を表示するスプライト
	prefab::CSpriteRender* m_tensPlaceSpriteRender = nullptr;	//制限時間の二桁目を表示するスプライト

	int m_gameEndCount = 0;										//ゲームの終了が確定してからの経過時間

	float m_measureSecond = 0.0f;								//ゲームスタートのカウント時に1秒を測る用
	int m_startCount = 5;										//ゲームスタートまでのカウント
	prefab::CSpriteRender* m_startSprite = nullptr;				//ゲームスタートのスプライト
	bool m_playCountSEFlag = false;								//カウントダウンの音を鳴らす?
	bool m_isGameEndFirstFrame = true;							//ゲームが終了した直後のフレームか?

	//開始カウントダウン
	bool m_startCount3_Flag = false;							//開始3秒前のカウントダウンを表示した?
	bool m_startCount2_Flag = false;							//開始2秒前のカウントダウンを表示した?
	bool m_startCount1_Flag = false;							//開始1秒前のカウントダウンを表示した?
	prefab::CSpriteRender* m_startCountDown_3_Top = nullptr;	//3秒前のカウントダウンの上側のスプライト
	prefab::CSpriteRender* m_startCountDown_3_Middle = nullptr;	//3秒前のカウントダウンの中央のスプライト
	prefab::CSpriteRender* m_startCountDown_3_Bottom = nullptr;	//3秒前のカウントダウンの下側のスプライト
	prefab::CSpriteRender* m_startCountDown_3_Right = nullptr;	//3秒前のカウントダウンの右側のスプライト
	prefab::CSpriteRender* m_startCountDown_2_Top = nullptr;	//2秒前のカウントダウンの上側のスプライト
	prefab::CSpriteRender* m_startCountDown_2_Middle = nullptr;	//2秒前のカウントダウンの中央のスプライト
	prefab::CSpriteRender* m_startCountDown_2_Bottom = nullptr;	//2秒前のカウントダウンの下側のスプライト
	prefab::CSpriteRender* m_startCountDown_2_Right = nullptr;	//2秒前のカウントダウンの右側のスプライト
	prefab::CSpriteRender* m_startCountDown_2_Left = nullptr;	//2秒前のカウントダウンの左側のスプライト
	prefab::CSpriteRender* m_startCountDown_1 = nullptr;		//1秒前のカウントダウンのスプライト

	//終了カウントダウン
	bool m_finalCount3_Flag = false;							//終了3秒前のカウントダウンを表示した?
	bool m_finalCount2_Flag = false;							//終了2秒前のカウントダウンを表示した?
	bool m_finalCount1_Flag = false;							//終了1秒前のカウントダウンを表示した?
	prefab::CSpriteRender* m_finalCountDown = nullptr;			//終了のカウントダウンを表示するスプライト

	prefab::CFontRender* m_drawFontRender = nullptr;			//引き分けのフォント
	int m_drawFontCount = 300;									//引き分けのフォントを表示する時間

	RoundCounter* m_roundCounter = nullptr;						//プレイヤーの取得ラウンド数の管理クラス
};

