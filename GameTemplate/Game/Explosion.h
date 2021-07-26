#pragma once
class Explosion :public IGameObject
{
	~Explosion() override;
	bool Start() override;
	void Update() override;

public:

	/**
	 * @brief �����̍��W��ݒ肷��
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	prefab::CEffect* m_effect = nullptr;	//�����̃G�t�F�N�g
	Vector3 m_position = Vector3::Zero;		//�����̍��W
	bool m_deleteFlag = false;				//�폜���邩?(�����͏I�������?)
};
