#pragma once
class Incendia :public IGameObject
{
	~Incendia();
	bool Start();
	void Update();
public:

	Vector3 m_position;

	//�_���[�W���󂯂�܂ł̃J�E���g(2�L������)
	int m_damageCountdown[2] = {0,0};

	//�R�������鎞��
	int m_deleteCountdown = 180;

	//�G�t�F�N�g
	prefab::CEffect* m_effect = nullptr;
};
