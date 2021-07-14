#pragma once

class GameOption : public IGameObject
{
	~GameOption();
	bool Start() override;
	void Update() override;
public:
	//現在の選択深度
	enum SelectingState
	{
		enItem,		//オプション項目
		enNumeric	//項目の数値
	};

public:
	/**
	 * @brief オプション画面を開く
	*/
	void Open();

	/**
	 * @brief オプション画面を閉じる
	*/
	void Close();

	/**
	 * @brief BGM全体のボリュームを取得
	 * @return BGM全体のボリューム
	*/
	float GetBGMVolume() { return m_BGMVolume; }

	/**
	 * @brief SE全体のボリュームを取得
	 * @return SE全体のボリューム
	*/
	float GetSEVolume() { return m_SEVolume; }

	/**
	 * @brief プレイヤー1のカメラ感度の数値を取得
	 * @return プレイヤー1のカメラ感度
	*/
	float GetP1Sensitivity() { return m_P1Sensitivity; }

	/**
	 * @brief プレイヤー2のカメラ感度の数値を取得
	 * @return プレイヤー2のカメラ感度
	*/
	float GetP2Sensitivity() { return m_P2Sensitivity; }

	/**
	 * @brief ゲームの制限時間の数値を取得
	 * @return ゲームの制限時間
	*/
	float GetTimeLimit() { return m_gameTimeLimit; }

	/**
	 * @brief 勝利に必要なラウンド数を取得
	 * @return 勝利に必要なラウンド数
	*/
	int GetRoundToWin() { return static_cast<int>(m_roundToWin); }

	/**
	 * @brief 現在のオプション画面の選択深度を取得
	 * @return 選択深度
	*/
	SelectingState GetSelectingState() { return m_selectingState; }

	/**
	 * @brief 背景スプライトがまだ拡大もしくは縮小中か?
	 * @return 拡大もしくは縮小中ならtrue
	*/
	bool isQueuing()
	{
		return m_optionBackGroundSprite->GetSpriteSupporter().GetSpriteScaleListLen() != 0;
	}

	/**
	 * @brief ファイルからオプション読み込み
	 * @return 成功?
	*/
	static bool ReadOption();

	/**
	 * @brief ファイルにオプション書き込み
	 * @return 成功?
	*/
	bool WriteOption();

private:
	prefab::CSpriteRender* m_optionBackGroundSprite = nullptr;	//背景スプライト
	bool m_isOpen = false;										//オプションが開いているか
	static float m_BGMVolume;									//BGMのボリューム
	static float m_SEVolume;									//効果音のボリューム
	static float m_P1Sensitivity;								//プレイヤー1のカメラ感度
	static float m_P2Sensitivity;								//プレイヤー2のカメラ感度
	static float m_gameTimeLimit;								//ゲームの制限時間
	static float m_roundToWin;									//勝利に必要なラウンド数
	prefab::CFontRender* m_optionFont = nullptr;				//オプションのフォント
	prefab::CFontRender* m_BGMVolumeFont = nullptr;				//BGMのボリュームのフォント
	prefab::CFontRender* m_SEVolumeFont = nullptr;				//効果音のボリュームのフォント
	prefab::CFontRender* m_1PSensitivityFont = nullptr;			//プレイヤー1のカメラ感度のフォント
	prefab::CFontRender* m_2PSensitivityFont = nullptr;			//プレイヤー2のカメラ感度のフォント
	prefab::CFontRender* m_gameTimeLimitFont = nullptr;			//ゲームの制限時間のフォント
	prefab::CFontRender* m_roundToWinFont = nullptr;			//勝利に必要なラウンド数のフォント
	SelectingState m_selectingState = enItem;					//選択深度
	int m_selectingItem = 0;									//選択している項目番号(0:BGM,1:SE,2:1Pカメラ感度,3:2Pカメラ感度,4:ゲーム制限時間,5:勝利に必要なラウンド数)
	float* m_selectingItemValue = nullptr;						//選択している項目の数値を格納するポインタ
	float m_selectingItemTemporaryValue = 1.0f;					//選択した項目の値を保存しておく(キャンセルした時に戻せるよう)
	prefab::CFontRender* m_selectingItemFont = nullptr;			//選択している項目のフォントを格納するポインタ
	wchar_t m_buffer[256] = L"";								//フォントのSetTextに渡す時の変換用バッファ
};

