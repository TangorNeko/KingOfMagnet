#pragma once
class Explosion :public IGameObject
{
	~Explosion();
	bool Start();
	void Update();

public:

	/**
	 * @brief �����̍��W��ݒ肷��
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	prefab::CEffect* m_effect = nullptr;	//�����̃G�t�F�N�g
	Vector3 m_position;						//�����̍��W
	bool m_deleteFlag = false;				//�폜���邩?(�����͏I�������?)
};
