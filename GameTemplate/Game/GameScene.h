#pragma once

class Character_base;
class CDirectionLight;
class CLevelRender2D;
class PopRandItem;
class BackGround;
class SkyBoard;

class GameScene : public IGameObject
{
	bool Start() override;
	~GameScene();
	void Update() override;


	Character_base* m_player1 = nullptr;
	Character_base* m_player2 = nullptr;

	prefab::CDirectionLight* m_stageLight = nullptr;
	prefab::CLevelRender2D* m_level2D = nullptr;
	PopRandItem* m_itemgenerator = nullptr;
	BackGround* m_backGround = nullptr;
	SkyBoard* m_sky = nullptr;

	bool m_gameEndFlag = false;
	int m_gameEndCount = 0;

public:

	//プレイヤーを登録する。
	void RegisterPlayer(Character_base* player1, Character_base* player2)
	{
		m_player1 = player1;
		m_player2 = player2;
	}

	void SetGameEndFlag(bool flag)
	{
		m_gameEndFlag = flag;
	}

	bool GetGameEndFlag()
	{
		return m_gameEndFlag;
	}
};

