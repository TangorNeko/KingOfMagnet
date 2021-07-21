#pragma once

class Repulsion;
class BackGround : public IGameObject
{
	~BackGround() override;
	bool Start() override;
	void Update()override;
public:
	/**
	 * @brief �������X�e�[�W�̃��f���Ƀq�b�g(����)���Ă��邩?
	 * @param start �����̊J�n�n�_
	 * @param end �����̏I���n�_
	 * @param crossPoint �q�b�g�����n�_(�Q�Ǝ󂯎��)
	 * @return �q�b�g����?
	*/
	bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint) const;

	/**
	 * @brief �󂯎�����n�_(�G�̈ʒu)����ł��������X�|�[���n�_��Ԃ��B
	 * @param enemyPos �G�̍��W
	 * @return ���X�|�[���n�_�̒��Ŏ󂯎�����G�̍��W�����ԉ������̂̍��W
	*/
	Vector3 CalcRespawnPoint(const Vector3& enemyPos) const;
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//�X�e�[�W�̃��f��
	Vector3 m_position = { 0.0f,0.0f,0.0f };				//�X�e�[�W�̃��f���̍��W
	prefab::CLevel m_level;									//���X�|�[���n�_��ǂݍ��ޗp�̃��f���B
	prefab::CSpotLight* m_southSpotLight = nullptr;			//�X�e�[�W�̌����Ƃ炷���C�g
	prefab::CSpotLight* m_northSpotLight = nullptr;			//�X�e�[�W�̌����Ƃ炷���C�g
	PhysicsStaticObject m_physicsStaticObject;				//CharaCon�Ƃ̓����蔻��Ɏg�p����ÓI�����I�u�W�F�N�g
	std::vector<Vector3> m_respawnPoints;					//���X�|�[���n�_�̌����i�[����R���e�i
};

