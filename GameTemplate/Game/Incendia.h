#pragma once
class Incendia :public IGameObject
{
	~Incendia();
	bool Start();
	void Update();
public:

	Vector3 m_position;

	int m_deletetime = 500;

	//�_���[�W���󂯂�܂ł̃J�E���g(2�L������)
	int m_damageCountdown[2] = {0,0};

	//�R�������鎞��
	int m_deleteCountdown = 500;
};
