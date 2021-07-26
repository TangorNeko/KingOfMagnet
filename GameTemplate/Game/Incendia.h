#pragma once

class GameScene;

class Incendia :public IGameObject
{
	~Incendia() override;
	bool Start() override;
	void Update() override;
	
public:

	/**
	 * @brief �����̍��W��ݒ肷��
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	prefab::CEffect* m_effect = nullptr;		//����̃G�t�F�N�g
	int m_damageCountdown[2] = {0,0};			//�_���[�W���󂯂�܂ł̃J�E���g(2�L������)	
	int m_deleteCountdown = 180;				//�R�������鎞��
	Vector3 m_position = { 0.0f,0.0f,0.0f };	//�����̍��W
	bool m_deleteFlag = false;					//�폜���邩?(�����͏I�������?)
	GameScene* m_gameScene = nullptr;			//�Q�[���V�[��
};
