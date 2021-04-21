#pragma once

class Character_base;

class GameScene : public IGameObject
{
	bool Start() override;
	~GameScene();
	void Update() override;


	Character_base* m_player1 = nullptr;
	Character_base* m_player2 = nullptr;

public:

	//プレイヤーを登録する。
	void RegisterPlayer(Character_base* player1, Character_base* player2)
	{
		m_player1 = player1;
		m_player2 = player2;
	}
};

