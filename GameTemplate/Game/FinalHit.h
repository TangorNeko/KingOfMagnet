#pragma once

class GameScene;
class Player;
class FinalHit : public IGameObject
{
public:
	~FinalHit();
	bool Start()override;
	void Update()override;

	void SetLoserNum(int num) { m_loserNum = num; }
	int GetLoserNum() { return m_loserNum; }
private:
	Player* m_loser = nullptr;
	Player* m_winner = nullptr;
	Vector3 m_sequenceCameraPos = Vector3::Zero;
	Vector3 m_winnerVec;//敗者から勝者に向かうベクトル
	Vector3 m_winnerWaistPos;//勝者の腰の位置
	GameScene* m_gameScene = nullptr;
	int m_LoseCameraLoop = 0;
	int m_playerNum = 0;
	int m_loserNum = 0;
	int m_LastCameraStatus = 0;
	float m_coef = 0.0f;//ベクターに掛ける値(coefficient)
	bool m_isWinAnimOn = false;//勝利アニメーションを流すか

	//勝敗表示
	prefab::CSpriteRender* m_resultWinnerSprite = nullptr;		//勝者を表示するスプライト(○P)
	prefab::CSpriteRender* m_resultWinSprite = nullptr;			//勝利のスプライト(WIN!)

	enum FinalCameraState
	{
		enRight,
		enLeft,
		enFront,
		enLoserBack,
		enToWinner,
	};
	FinalCameraState m_finalCameraState=enRight;
};

