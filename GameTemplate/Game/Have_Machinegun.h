#pragma once
class Have_Machinegun:public IGameObject
{
	~Have_Machinegun();
	bool Start();
	void Update();
public:
	int m_loopcount = 0;
	int m_deletetime = 500;
	int m_bulletNum = 100;
	int m_playerNum=0;
};

