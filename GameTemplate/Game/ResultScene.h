#pragma once
class ResultScene:public IGameObject
{
	bool Start();
	void Update();
	~ResultScene();

public:

	/**
	 * @brief 敗者の番号を設定
	 * @param playerNum 敗者のプレイヤー番号
	*/
	void SetLoserNum(int playerNum)
	{
		m_loserNum = playerNum;
	}

	enum {
		enAnimationClip_Win,
		enAnimationClip_Lose,
		enAnimationClip_Num,
	};

private:
	AnimationClip m_animationClips[enAnimationClip_Num];		//キャラクターのアニメーション
	prefab::CSkinModelRender* m_P1_skinModelRender = nullptr;	//プレイヤー1のモデル
	prefab::CSkinModelRender* m_P2_skinModelRender = nullptr;	//プレイヤー2のモデル
	prefab::CDirectionLight* m_directionLight = nullptr;		//勝者を照らすライト
	prefab::CSpriteRender* m_Down_SpriteRender = nullptr;		//下側のシャッターのスプライト
	prefab::CSpriteRender* m_Up_SpriteRender = nullptr;			//上側のシャッターのスプライト
	prefab::CSpriteRender* m_Under_SpriteRender = nullptr;		//プレイヤーを表示する窓のスプライト
	prefab::CSpriteRender* m_Lose_SpriteRender = nullptr;		//LOSEのスプライト
	prefab::CSpriteRender* m_Win_SpriteRender = nullptr;		//WINのスプライト
	prefab::CSpriteRender* m_Command_SpriteRender = nullptr;	//リトライ、タイトルへのスプライト
	prefab::CSpriteRender* m_BG_SpriteRender = nullptr;			//背景のスプライト
	bool m_RetryOn = true;										//リトライのコマンド選択中?(falseならタイトルへのコマンド選択中)
	int m_loserNum = 0;											//敗者のプレイヤー番号
	bool m_moveEndFlag = false;									//移動完了
	int m_moveTimer = 0;										//スプライトの移動タイマー
	bool m_win_lose_MoveFlag = false;							//Winの文字を拡大縮小させる?
	prefab::CSoundSource* m_resultBGM = nullptr;				//リザルトBGM
};

