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
	Vector3 m_winnerVec;//�s�҂��珟�҂Ɍ������x�N�g��
	Vector3 m_winnerWaistPos;//���҂̍��̈ʒu
	GameScene* m_gameScene = nullptr;
	int m_LoseCameraLoop = 0;
	int m_playerNum = 0;
	int m_loserNum = 0;
	int m_LastCameraStatus = 0;
	float m_coef = 0.0f;//�x�N�^�[�Ɋ|����l(coefficient)
	bool m_isWinAnimOn = false;//�����A�j���[�V�����𗬂���

	//���s�\��
	prefab::CSpriteRender* m_resultWinnerSprite = nullptr;		//���҂�\������X�v���C�g(��P)
	prefab::CSpriteRender* m_resultWinSprite = nullptr;			//�����̃X�v���C�g(WIN!)

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

