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
		m_playerTakeRound[playerNum]++;
	}

	void Disable()
	{
		for (auto font : tmp_gameRoundFont)
		{
			font->SetScale({ 0.0f,0.0f });
		}
		
		for (auto font : tmp_resultRoundFont)
		{
			font->SetScale({ 0.0f,0.0f });
		}
	}

	void EnableGameRound()
	{
		for (auto font : tmp_gameRoundFont)
		{
			font->SetScale({ 1.0f,1.0f });
		}
	}

	void EnableResultRound()
	{
		for (auto font : tmp_resultRoundFont)
		{
			font->SetScale({ 1.0f,1.0f });
		}

		switch (m_latestRoundWinner)
		{
		case 0:
			tmp_roundEffect->SetPosition({ -500.0f,-250.0f,0.0f });
			break;
		case 1:
			tmp_roundEffect->SetPosition({ 500.0f,-250.0f,0.0f });
			break;
		}

		m_isResult = true;
	}

private:
	int m_currentRound = 0;					//現在のラウンド数
	int m_latestRoundWinner = -1;			//一番最近のラウンドの勝者
	int m_playerTakeRound[2] = { 0,0 };		//各プレイヤーの取得ラウンド
	bool m_isResult = false;
	int m_twinkleCount = 0;

	prefab::CFontRender* tmp_gameRoundFont[2] = { nullptr,nullptr };//ゲーム中画面の仮表示フォント
	prefab::CFontRender* tmp_resultRoundFont[2] = { nullptr,nullptr };//リザルト中の仮表示フォント
	prefab::CEffect2D* tmp_roundEffect = nullptr; //ラウンド獲得表示のエフェクト
};

