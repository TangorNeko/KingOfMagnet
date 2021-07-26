#pragma once
class GameScene;

class Repulsion : public IGameObject
{
	~Repulsion() override;
	bool Start() override;
	void Update() override;

public:

	/**
	 * @brief �˗͏��̍��W��ݒ�
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief �˗͏��̉�]��ݒ�
	 * @param qRot ��]
	*/
	void SetRotation(const Quaternion& qRot)
	{
		m_qRot = qRot;
	}

private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;				//�˗͏��̃��f��
	Vector3 m_position = Vector3::Zero;									//�˗͏��̍��W
	Quaternion m_qRot = Quaternion::Identity;							//�˗͏��̉�]
	prefab::CSoundSource* m_repulsionStaySE[2] = { nullptr,nullptr };	//�˗͏��ɏ�葱���Ă��鎞�̌��ʉ��A2�v���C���[��
	bool m_isPlayEnterSE[2] = { false,false };							//������u�Ԃ�SE��炵�����ǂ���(2�v���C���[��)
	bool m_isPlayStaySE[2] = { false,false };							//��葱���Ă��鎞��SE��炵�����ǂ���(2�v���C���[��)
	float m_staySEVolume[2] = { 1.0f,1.0f };							//��葱���Ă��鎞��SE�̃{�����[��
	GameScene* m_gameScene = nullptr;									//�Q�[���V�[��
};

