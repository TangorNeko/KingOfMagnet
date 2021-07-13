#pragma once

class RoundCounter : public IGameObject
{
public:
	~RoundCounter();
private:
	bool Start() override;
	void Update() override;
public:

	/**
	 * @brief 2ラウンド先取して勝利したプレイヤーを取得
	 * @return 勝者プレイヤー番号　まだ決まっていない場合は-1
	*/
	int GetOverAllWinner();

	/**
	 * @brief ラウンドの勝者を登録
	 * @param playerNum ラウンドの勝者のプレイヤー番号
	*/
	void SubmitRoundWinner(int playerNum)
	{
		m_latestRoundWinner = playerNum;
	}

	/**
	 * @brief スプライト非表示
	*/
	void Disable()
	{
		//全てのスプライトのスケールを0にして非表示
		for (auto roundSprite : m_gameRoundSprite)
		{
			roundSprite[0]->SetScale({ 0.0f,0.0f,0.0f });
			roundSprite[1]->SetScale({ 0.0f,0.0f,0.0f });
		}
	}

	/**
	 * @brief スプライトをゲーム画面用の位置で表示
	*/
	void EnableGameRound();

	/**
	 * @brief スプライトを結果画面用の位置で表示
	*/
	void EnableResultRound();

	/**
	 * @brief 現在のラウンドの読み上げ
	*/
	void RoundAnnounce();

private:
	int m_currentRound = 0;								//現在のラウンド数
	int m_latestRoundWinner = -1;						//一番最近のラウンドの勝者
	int m_playerTakeRound[2] = { 0,0 };					//各プレイヤーの取得ラウンド
	bool m_isResult = false;							//勝者表示画面?
	int m_roundResultCount = 0;							//ラウンドごとの結果が再生されるまでのカウント
	prefab::CSpriteRender* m_gameRoundSprite[2][2] =	//取得ラウンド表示スプライト
	{ {nullptr,nullptr},{nullptr,nullptr} };
	prefab::CEffect2D* m_roundGetEffect = nullptr;		//ラウンド獲得表示のエフェクト
	bool m_roundAlreadyAnnounce = false;
};

