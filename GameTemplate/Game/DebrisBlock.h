#pragma once

class SampleScene;

class DebrisBlock : public IGameObject
{
	~DebrisBlock();
	bool Start() override;
	void Update() override;

public:

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//���f��

	Vector3 m_position = { 0.0f,0.0f,0.0f };//���W

	int m_factoryTimer = 0;//���̓o�[�X�g���ꂽ���̐����^�C�}�[

	SampleScene* m_gameScene = nullptr;//�Q�[���V�[��
};

