#pragma once
class MyCapsuleCollider
{
private:
	
	/**
	 * @brief �_�ƒ����̍ŒZ����
	 * @param point �_�̍��W
	 * @param linePos �����̈�_
	 * @param lineDir �����̈�_����̌���
	 * @param crossPoint ������̍ŒZ�����̓_(�߂�l)
	 * @param t �������1�_����ŒZ�����̓_�ւ̒P�ʃx�N�g���W��(�߂�l)
	 * @return �ŒZ����
	*/
	float CalcPointLineDist(const Vector3& point, const Vector3& linePos, const Vector3& lineDir, Vector3& crossPoint, float& t);
	
	/**
	 * @brief �_�Ɛ����̍ŒZ����
	 * @param point �_�̍��W
	 * @param segmentStartPoint �����̎n�_
	 * @param segmentEndPoint �����̏I�_
	 * @param minPoint �ŒZ�����̓_(�߂�l)
	 * @param t �[�_�̃x�N�g���W��(�߂�l)
	 * @return �ŒZ����
	*/
	float CalcPointSegmentDist(const Vector3& point, const Vector3& segmentStartPoint, const Vector3& segmentEndPoint, Vector3& minPoint, float& t);

	/**
	 * @brief 2�����̍ŒZ���������߂�
	 * @param line1StartPoint ����1�̎n�_
	 * @param line1EndPoint ����1�̏I�_
	 * @param line2StartPoint ����2�̎n�_
	 * @param line2EndPoint ����2�̏I�_
	 * @param line1CrossPoint �ŒZ�����̒���1��̓_(�߂�l)
	 * @param line2CrossPoint �ŒZ�����̒���2��̓_(�߂�l)
	 * @param t1 L1�̃x�N�g���W��(�߂�l)
	 * @param t2 L2�̃x�N�g���W��(�߂�l)
	 * @return 
	*/
	float CalcLineLineDist(const Vector3& line1StartPoint, const Vector3& line1EndPoint, const Vector3& line2StartPoint, const Vector3& line2EndPoint, Vector3& line1CrossPoint, Vector3& line2CrossPoint, float& t1, float& t2);

	/**
	 * @brief 2�����̍ŒZ���������߂�
	 * @param segment1StartPoint ����1�̎n�_
	 * @param segment1EndPoint ����1�̏I�_
	 * @param segment2StartPoint ����2�̎n�_
	 * @param segment2EndPoint ����2�̏I�_
	 * @param segment1CrossPoint �ŒZ�����̐���1��̓_
	 * @param segment2CrossPoint �ŒZ�����̐���2��̓_
	 * @param t1 ����1�̃x�N�g���W��(�߂�l)
	 * @param t2 ����2�̃x�N�g���W��(�߂�l)
	 * @return �ŒZ����
	*/
	float CalcSegmentSegmentDist(const Vector3& segment1StartPoint, const Vector3& segment1EndPoint, const Vector3& segment2StartPoint, const Vector3& segment2EndPoint, Vector3& segment1CrossPoint, Vector3& segment2CrossPoint, float& t1, float& t2);

public:

	/**
	 * @brief �J�v�Z���̎n�_���Z�b�g����
	 * @param startPoint �n�_�̈ʒu
	*/
	void SetStartPoint(const Vector3& startPoint)
	{
		m_capsuleStartPoint = startPoint;
	}

	/**
	 * @brief �J�v�Z���̏I�_���Z�b�g����
	 * @param endPoint �I�_�̈ʒu
	*/
	void SetEndPoint(const Vector3& endPoint)
	{
		m_capsuleEndPoint = endPoint;
	}

	/**
	 * @brief �J�v�Z���̔��a���Z�b�g����
	 * @param radius ���a�̒���
	*/
	void SetRadius(const float& radius)
	{
		m_radius = radius;
	}

	/**
	 * @brief �J�v�Z�����m���Փ˂��Ă��邩?
	 * @param capsule ���葤�̃J�v�Z��
	 * @return �Փ˂��Ă���Ȃ�true
	*/
	bool isHitCapsule(const MyCapsuleCollider& capsule)
	{
		Vector3 crossPoint1, crossPoint2;
		float t1, t2;

		float distance = CalcSegmentSegmentDist(m_capsuleStartPoint, m_capsuleEndPoint,
			capsule.m_capsuleStartPoint, capsule.m_capsuleEndPoint,
			crossPoint1, crossPoint2, t1, t2);

		return (distance <= m_radius + capsule.m_radius);
	}

private:
	Vector3 m_capsuleStartPoint;	//�J�v�Z���̎n�_
	Vector3 m_capsuleEndPoint;		//�J�v�Z���̏I�_
	float m_radius;					//�J�v�Z���̔��a
};

