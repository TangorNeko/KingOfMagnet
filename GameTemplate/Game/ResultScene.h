#pragma once
class ResultScene:public IGameObject
{
	bool Start();
	void Update();
public:
	~ResultScene();
	void ResultDisplay();

	prefab::CDirectionLight* m_stageLight = nullptr;

	prefab::CSpriteRender* m_Down_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Up_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Under_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Lose_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Win_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Command_SpriteRender = nullptr;
	prefab::CSpriteRender* m_BG_SpriteRender = nullptr;

	prefab::CSkinModelRender* m_P1_skinModelRender = nullptr;
	prefab::CSkinModelRender* m_P2_skinModelRender = nullptr;

	//勝者を照らすライト
	prefab::CDirectionLight* m_winnerDirectionLight = nullptr;

	enum {
		enAnimationClip_Win,
		enAnimationClip_Lose,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num];

	//勝者、敗者のポジション
	Vector3 m_WinnerPos = { 60,25,100 };
	Vector3 m_LoserPos = { -50,40,100 };
	//カメラのポジション
	Vector3 m_cameraPos = { 0,90,250 };

	Vector3 m_DownPos = { Vector3::Zero };
	Vector3 m_UpPos = { Vector3::Zero };
	Vector3 m_UnderPos = { Vector3::Zero };
	Vector3 m_LosePos = { 350.0f,220.0f,0.0f };
	Vector3 m_WinPos = { -200.0f,280.0f,0.0f };
	Vector3 m_CommandPos = { 0.0f,-275.0f,0.0f };
	bool m_RetryOn = true;
	int m_loserNum = 0;
};

