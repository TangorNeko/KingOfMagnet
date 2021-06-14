#pragma once
class TriangleCollider
{
public:
	/**
	 * @brief �O�p�`�̒��_�̍��W��ݒ�
	 * @param v1 1�ڂ̒��_�̍��W
	 * @param v2 2�ڂ̒��_�̍��W
	 * @param v3 3�ڂ̒��_�̍��W
	*/
	void SetVertex(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		m_vertex[0] = v1;
		m_vertex[1] = v2;
		m_vertex[2] = v3;
	}

	/**
	 * @brief �������O�p�`�Ɍ������Ă���?
	 * @param start �����̎n�_
	 * @param end �����̏I�_
	 * @param finalCrossPoint �����̌����_(�߂�l) 
	 * @return �q�b�g������(���Ă�����true)
	*/
	bool isHit(const Vector3& start, const Vector3& end,Vector3& finalCrossPoint);

private:
	Vector3 m_vertex[3] = { Vector3::Zero,Vector3::Zero,Vector3::Zero };	//�����蔻��̎O�p�`�̒��_
};

