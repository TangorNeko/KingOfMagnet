#pragma once

class RoundCounter : public IGameObject
{
	~RoundCounter() override;
	bool Start() override;
	void Update() override;
public:

	/**
	 * @brief 最大ラウンドをセット
	 * @param round 最大ラウンド
	*/
	void SetMaxRound(int round)
	{
		m_maxRound = round;
		//最大ラウンド数から表示するスプライトの数を計算
		m_roundToWin = (m_maxRound / 2) + 1;
	}

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
	 * @brief スプライトをゲーム画面用の位置で表示
	*/
	void MoveToGamePosition();

	/**
	* @brief スプライトを結果画面の位置に移動
	*/
	void MoveToResultPosition();

	/**
	 * @brief スプライトを結果画面用の位置で表示
	*/
	void StartResultMove();

	/**
	 * @brief 現在のラウンドの読み上げ
	*/
	void RoundAnnounce();

private:
	static int m_maxRound;									//最大ラウンド数(偶数)
	static int m_roundToWin;								//勝利に必要なラウンド
	int m_currentRound = 0;									//現在のラウンド数
	int m_latestRoundWinner = -1;							//一番最近のラウンドの勝者
	int m_playerTakeRound[2] = { 0,0 };						//各プレイヤーの取得ラウンド
	bool m_isResult = false;								//勝者表示画面?
	int m_roundResultCount = 0;								//ラウンドごとの結果が再生されるまでのカウント
	prefab::CEffect2D* m_roundGetEffect = nullptr;			//ラウンド獲得表示のエフェクト
	bool m_roundAlreadyAnnounce = false;					//そのラウンドのアナウンスをもうしたか?
	std::vector<prefab::CSpriteRender*> m_player1Sprites;	//プレイヤー1のスプライトのリスト
	std::vector<prefab::CSpriteRender*> m_player2Sprites;	//プレイヤー2のスプライトのリスト

};

