#pragma once
class MyCapsuleCollider
{
private:
	Vector3 m_capsuleStartPoint;
	Vector3 m_capsuleEndPoint;
	float m_radius;

	/// <summary>
	/// �_�ƒ����̍ŒZ����
	/// </summary>
	/// <param name="point">�_�̍��W</param>
	/// <param name="linePos">�����̈�_</param>
	/// <param name="lineDir">�����̈�_����̌���</param>
	/// <param name="crossPoint">������̍ŒZ�����̓_(�߂�l)</param>
	/// <param name="t">�������1�_����ŒZ�����̓_�ւ̒P�ʃx�N�g���W��(�߂�l)</param>
	/// <returns>�ŒZ����</returns>
	float CalcPointLineDist(const Vector3& point, const Vector3& linePos, const Vector3& lineDir, Vector3& crossPoint, float& t);
	
	/// <summary>
	/// �_�Ɛ����̍ŒZ����
	/// </summary>
	/// <param name="point">�_�̍��W</param>
	/// <param name="segmentStartPoint">�����̎n�_</param>
	/// <param name="segmentEndPoint">�����̏I�_</param>
	/// <param name="minPoint">�ŒZ�����̓_(�߂�l)</param>
	/// <param name="t">�[�_�̃x�N�g���W��?</param>
	/// <returns>�ŒZ����</returns>
	float CalcPointSegmentDist(const Vector3& point, const Vector3& segmentStartPoint, const Vector3& segmentEndPoint, Vector3& minPoint, float& t);

	/// <summary>
	/// 2�����̍ŒZ���������߂�
	/// </summary>
	/// <param name="line1StartPoint">����1�̎n�_</param>
	/// <param name="line1EndPoint">����1�̏I�_</param>
	/// <param name="line2StartPoint">����2�̎n�_</param>
	/// <param name="line2EndPoint">����2�̏I�_</param>
	/// <param name="line1CrossPoint">�ŒZ�����̒���1��̓_(�߂�l)</param>
	/// <param name="line2CrossPoint">�ŒZ�����̒���2��̓_(�߂�l)</param>
	/// <param name="t1">L1�̃x�N�g���W��(�߂�l)</param>
	/// <param name="t2">L2�̃x�N�g���W��(�߂�l)</param>
	/// <returns>�ŒZ����</returns>
	float CalcLineLineDist(const Vector3& line1StartPoint, const Vector3& line1EndPoint, const Vector3& line2StartPoint, const Vector3& line2EndPoint, Vector3& line1CrossPoint, Vector3& line2CrossPoint, float& t1, float& t2);

	/// <summary>
	/// 2�����̍ŒZ���������߂�
	/// </summary>
	/// <param name="segment1StartPoint">����1�̎n�_</param>
	/// <param name="segment1EndPoint">����1�̏I�_</param>
	/// <param name="segment2StartPoint">����2�̎n�_</param>
	/// <param name="segment2EndPoint">����2�̏I�_</param>
	/// <param name="segment1CrossPoint">�ŒZ�����̐���1��̓_</param>
	/// <param name="segment2CrossPoint">�ŒZ�����̐���2��̓_</param>
	/// <param name="t1">����1�̃x�N�g���W��</param>
	/// <param name="t2">����2�̃x�N�g���W��</param>
	/// <returns>�ŒZ����</returns>
	float CalcSegmentSegmentDist(const Vector3& segment1StartPoint, const Vector3& segment1EndPoint, const Vector3& segment2StartPoint, const Vector3& segment2EndPoint, Vector3& segment1CrossPoint, Vector3& segment2CrossPoint, float& t1, float& t2);

public:
	/// <summary>
	/// �J�v�Z���̎n�_���Z�b�g����B
	/// </summary>
	/// <param name="startPoint">�n�_�̈ʒu</param>
	void SetStartPoint(const Vector3& startPoint)
	{
		m_capsuleStartPoint = startPoint;
	}

	/// <summary>
	/// �J�v�Z���̏I�_���Z�b�g����B
	/// </summary>
	/// <param name="endPoint">�I�_�̈ʒu</param>
	void SetEndPoint(const Vector3& endPoint)
	{
		m_capsuleEndPoint = endPoint;
	}

	/// <summary>
	/// �J�v�Z���̔��a���Z�b�g����B
	/// </summary>
	/// <param name="radius">���a�̒���</param>
	void SetRadius(const float& radius)
	{
		m_radius = radius;
	}

	/// <summary>
	/// �J�v�Z�����m���Փ˂��Ă��邩?
	/// </summary>
	/// <param name="capsule">���葤�̃J�v�Z��</param>
	/// <returns>�Փ˂��Ă���Ȃ�true</returns>
	bool isHitCapsule(const MyCapsuleCollider& capsule)
	{
		Vector3 crossPoint1, crossPoint2;
		float t1, t2;

		float distance = CalcSegmentSegmentDist(m_capsuleStartPoint, m_capsuleEndPoint,
			capsule.m_capsuleStartPoint, capsule.m_capsuleEndPoint,
			crossPoint1, crossPoint2, t1, t2);

		return (distance <= m_radius + capsule.m_radius);
	}
};

