#pragma once

class RoundCounter : public IGameObject
{
public:
	RoundCounter();
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
		m_playerTakeRound[playerNum]++;
	}

	void Disable()
	{
		for (auto font : tmp_playerRoundFont)
		{
			font->SetScale({ 0.0f,0.0f });
		}
	}

	void Enable()
	{
		for (auto font : tmp_playerRoundFont)
		{
			font->SetScale({ 1.0f,1.0f });
		}
	}

private:
	int m_currentRound = 0;					//現在のラウンド数
	int m_playerTakeRound[2] = { 0,0 };		//各プレイヤーの取得ラウンド

	prefab::CFontRender* tmp_playerRoundFont[2] = { nullptr,nullptr };//プレイヤー仮表示フォント
};

