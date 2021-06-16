#pragma once
class ResultScene:public IGameObject
{
	bool Start();
	void Update();
public:
	~ResultScene();
	void ResultDisplay();

	prefab::CSpriteRender* m_Down_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Up_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Under_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Lose_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Win_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Command_SpriteRender = nullptr;
	prefab::CSpriteRender* m_BG_SpriteRender = nullptr;

	prefab::CSkinModelRender* m_P1_skinModelRender = nullptr;
	prefab::CSkinModelRender* m_P2_skinModelRender = nullptr;

	//èüé“Çè∆ÇÁÇ∑ÉâÉCÉg
	prefab::CDirectionLight* m_directionLight = nullptr;

	enum {
		enAnimationClip_Win,
		enAnimationClip_Lose,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num];

	bool m_RetryOn = true;
	int m_loserNum = 0;

	bool m_moveEndFlag = false;		//à⁄ìÆäÆóπ
	int m_moveTimer = 0;
	const int MoveLimit = 120;
	bool m_win_lose_MoveFlag = false;

	//BGM
	prefab::CSoundSource* ssBGM = nullptr;
};

