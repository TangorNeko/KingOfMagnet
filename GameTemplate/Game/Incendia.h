#pragma once

class GameScene;

class Incendia :public IGameObject
{
	~Incendia();
	bool Start();
	void Update();

private:

	//�_���[�W���󂯂�܂ł̃J�E���g(2�L������)
	int m_damageCountdown[2] = {0,0};

	//�R�������鎞��
	int m_deleteCountdown = 180;

	//�G�t�F�N�g
	prefab::CEffect* m_effect = nullptr;

	//�Q�[���V�[��
	GameScene* m_gameScene = nullptr;

	bool m_deleteFlag = false;
public:
	Vector3 m_position;
};
