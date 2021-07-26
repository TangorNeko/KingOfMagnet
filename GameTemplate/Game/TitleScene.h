#pragma once

class GameOption;
class TitleScene:public IGameObject
{
	bool Start() override;
	void Update() override;
	~TitleScene() override;

	/**
	 * @brief 選択しているコマンドに応じてメニューバーを移動させる
	*/
	void CommandMove();

	/**
	 * @brief コマンドを決定した時の動作
	*/
	void CommandSelectMove();

private:
	//コマンドの種類
	enum TitleCommand {
		TC_Start,	//スタート
		TC_Option,	//オプション
		TC_Exit		//終了
	};
	TitleCommand m_titleCommand = TC_Start;							//現在選択中のコマンド
	bool m_selectFlag = false;										//コマンド決定済？
	bool m_selectMoveFlag = false;									//コマンド移動中？
	bool m_comandUpDown = false;									//コマンドの方向はどっち? false:上 true:下
	int m_commandTimer = 0;											//コマンド用タイマー
	prefab::CSkinModelRender* m_BG_ModelRender = nullptr;			//背景のモデル
	prefab::CPointLight* m_BGLight = nullptr;						//背景のモデルを照らすライト
	prefab::CSpriteRender* m_Monitor_SpriteRender = nullptr;		//モニターの画像
	prefab::CSpriteRender* m_MonitorLine_SpriteRender = nullptr;	//モニターのラインの画像
	prefab::CSpriteRender* m_Rogo_SpriteRender = nullptr;			//タイトルロゴの画像
	prefab::CSpriteRender* m_Option_SpriteRender = nullptr;			//オプションの画像
	prefab::CSpriteRender* m_Start_SpriteRender = nullptr;			//スタートの画像
	prefab::CSpriteRender* m_Exit_SpriteRender = nullptr;			//終了の画像
	prefab::CSpriteRender* m_Arrow_SpriteRender = nullptr;			//選択用の矢印の画像
	prefab::CSoundSource* m_titleBGM = nullptr;						//BGMのサウンドソース
	GameOption* m_option = nullptr;									//オプション画面
};

