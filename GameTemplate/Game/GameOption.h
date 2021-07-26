#pragma once
#include "OptionValue.h"

class GameOption : public IGameObject
{
	~GameOption() override;
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
	float GetBGMVolume() const { return m_BGMVolume.GetValue(); }

	/**
	 * @brief SE全体のボリュームを取得
	 * @return SE全体のボリューム
	*/
	float GetSEVolume() const { return m_SEVolume.GetValue(); }

	/**
	 * @brief プレイヤー1のカメラ感度の数値を取得
	 * @return プレイヤー1のカメラ感度
	*/
	float GetP1Sensitivity() const { return m_P1Sensitivity.GetValue(); }

	/**
	 * @brief プレイヤー2のカメラ感度の数値を取得
	 * @return プレイヤー2のカメラ感度
	*/
	float GetP2Sensitivity() const { return m_P2Sensitivity.GetValue(); }

	/**
	 * @brief ゲームの制限時間の数値を取得
	 * @return ゲームの制限時間
	*/
	float GetTimeLimit() const { return m_gameTimeLimit.GetValue(); }

	/**
	 * @brief 勝利に必要なラウンド数を取得
	 * @return 勝利に必要なラウンド数
	*/
	int GetRoundToWin() const { return static_cast<int>(m_maxRound.GetValue()); }

	/**
	 * @brief 現在のオプション画面の選択深度を取得
	 * @return 選択深度
	*/
	SelectingState GetSelectingState() const { return m_selectingState; }

	/**
	 * @brief 背景スプライトがまだ拡大もしくは縮小中か?
	 * @return 拡大もしくは縮小中ならtrue
	*/
	bool isQueuing() const
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

	/**
	 * @brief 設定の初期化
	*/
	static void Init();

private:
	prefab::CSpriteRender* m_optionBackGroundSprite = nullptr;	//背景スプライト
	bool m_isOpen = false;										//オプションが開いているか								
	static OptionValue m_BGMVolume;								//BGMのボリューム					
	static OptionValue m_SEVolume;								//効果音のボリューム							
	static OptionValue m_P1Sensitivity;							//プレイヤー1のカメラ感度							
	static OptionValue m_P2Sensitivity;							//プレイヤー2のカメラ感度							
	static OptionValue m_gameTimeLimit;							//ゲームの制限時間								
	static OptionValue m_maxRound;								//勝利に必要なラウンド数
	prefab::CFontRender* m_optionFont = nullptr;				//オプションのフォント
	prefab::CFontRender* m_BGMVolumeFont = nullptr;				//BGMのボリュームのフォント
	prefab::CFontRender* m_SEVolumeFont = nullptr;				//効果音のボリュームのフォント
	prefab::CFontRender* m_1PSensitivityFont = nullptr;			//プレイヤー1のカメラ感度のフォント
	prefab::CFontRender* m_2PSensitivityFont = nullptr;			//プレイヤー2のカメラ感度のフォント
	prefab::CFontRender* m_gameTimeLimitFont = nullptr;			//ゲームの制限時間のフォント
	prefab::CFontRender* m_maxRoundFont = nullptr;				//勝利に必要なラウンド数のフォント
	SelectingState m_selectingState = enItem;					//選択深度
	int m_selectingItem = 0;									//選択している項目番号(0:BGM,1:SE,2:1Pカメラ感度,3:2Pカメラ感度,4:ゲーム制限時間,5:勝利に必要なラウンド数)
	OptionValue* m_selectingItemValue = nullptr;				//選択している項目を格納するポインタ
	float m_selectingItemTemporaryValue = 1.0f;					//選択した項目の値を保存しておく(キャンセルした時に戻せるよう)
	prefab::CFontRender* m_selectingItemFont = nullptr;			//選択している項目のフォントを格納するポインタ
	wchar_t m_buffer[256] = L"";								//フォントのSetTextに渡す時の変換用バッファ
	static bool m_isInited;										//初期化されたか
};

