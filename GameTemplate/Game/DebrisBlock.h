#pragma once

class GameScene;

class DebrisBlock : public IGameObject
{
	~DebrisBlock();
	bool Start() override;
	void Update() override;

public:
	/**
	 * @brief �K���L�u���b�N�̍��W��ݒ�
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//���f��
	Vector3 m_position = { 0.0f,0.0f,0.0f };				//���W
	int m_factoryTimer = 0;									//���̓o�[�X�g���ꂽ���̐����^�C�}�[
	GameScene* m_gameScene = nullptr;						//�Q�[���V�[��
};

