#pragma once

class GameScene;

class Flash:public IGameObject
{
	~Flash();
	bool Start();
	void Update();

public:

	/**
	 * @brief �����̍��W��ݒ肷��
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief �t���b�V�����g�p�����v���C���[��ݒ�
	 * @param playerNum �t���b�V�����g�p�����v���C���[�̃v���C���[�ԍ�
	*/
	void SetParentNum(const int playerNum) { m_parentNum = playerNum; }

private:
	prefab::CSpriteRender* m_spriteRender = nullptr;	//�t���b�V���̃X�v���C�g
	float m_Alpha = 1.0f;								//�t���b�V���̃X�v���C�g�̓��ߗ�
	int m_parentNum;									//�t���b�V�����g�p�����v���C���[�̃v���C���[�ԍ�
	bool m_flashFlag = false;							//�t���b�V���͔�������?
	prefab::CEffect* m_effect = nullptr;				//�t���b�V�������̃G�t�F�N�g
	Vector3 m_position = {0.0f,0.0f,0.0f};				//�����̍��W
	bool m_deleteFlag = false;							//�폜���邩?(�����͏I�������?)
	GameScene* m_gameScene = nullptr;					//�Q�[���V�[��
};

