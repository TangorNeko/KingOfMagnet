#pragma once

class GameOption : public IGameObject
{
	~GameOption();
	bool Start() override;
	void Update() override;

	prefab::CSpriteRender* m_optionBackGroundSprite = nullptr;//背景スプライト

	bool isOpen = false;//オプションが開いているか

	static float m_BGMVolume;//BGMのボリューム
	static float m_SEVolume;//効果音のボリューム
	static float m_1PSensitivity;//プレイヤー1のカメラ感度
	static float m_2PSensitivity;//プレイヤー2のカメラ感度
	float m_selectItemTemporaryValue;//選択した項目の値を保存しておく(キャンセルした時に戻せるよう)

	prefab::CFontRender* m_optionFont = nullptr;//オプションのフォント
	prefab::CFontRender* m_BGMVolumeFont = nullptr;//BGMのボリュームのフォント
	prefab::CFontRender* m_SEVolumeFont = nullptr;//効果音のボリュームのフォント
	prefab::CFontRender* m_1PSensitivityFont = nullptr;//プレイヤー1のカメラ感度のフォント
	prefab::CFontRender* m_2PSensitivityFont = nullptr;//プレイヤー2のカメラ感度のフォント

public:
	//現在の選択深度
	enum SelectingState
	{
		enItem,//オプション項目
		enNumeric//項目の数値
	};

private:
	//選択深度
	SelectingState m_selectingState = enItem;

	//選択している項目番号(0:BGM,1:SE,2:1Pカメラ感度,3:2Pカメラ感度)
	int m_selectingItem = 0;

	//選択している項目の数値を格納するポインタ
	float* m_selectingItemValue = nullptr;

	//選択している項目のフォントを格納するポインタ
	prefab::CFontRender* m_selectingItemFont = nullptr;

	//フォントのSetTextに渡す時の変換用バッファ
	wchar_t m_buffer[256];
public:
	//オプションを開く
	void Open();

	//オプションを閉じる
	void Close();

	//BGMボリュームの数値を取得
	float GetBGMVolume() { return m_BGMVolume; }

	//SEボリュームの数値を取得
	float GetSEVolume() { return m_SEVolume; }

	//1Pのカメラ感度の数値を取得
	float Get1PSensitivity() { return m_1PSensitivity; }

	//2Pのカメラ感度の数値を取得
	float Get2PSensitivity() { return m_2PSensitivity; }

	SelectingState GetSelectingState() { return m_selectingState; }
};

