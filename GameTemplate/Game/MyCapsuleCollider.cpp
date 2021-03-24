#include "stdafx.h"
#include "MyCapsuleCollider.h"

	/// <summary>
	/// �_�ƒ����̍ŒZ����
	/// </summary>
	/// <param name="point">�_�̈ʒu</param>
	/// <param name="linePos">�����̈�_</param>
	/// <param name="lineDir">�����̈�_����̌���</param>
	/// <param name="crossPoint">������̍ŒZ�����̓_(�߂�l)</param>
	/// <param name="t">�������1�_����ŒZ�����̓_�ւ̒P�ʃx�N�g���W��(�߂�l)</param>
	/// <returns></returns>
float MyCapsuleCollider::CalcPointLineDist(const Vector3& point, const Vector3& linePos, const Vector3& lineDir, Vector3& crossPoint, float& t)
{
	float lenSqV = lineDir.Dot(lineDir);
	t = 0.0f;

	if (lenSqV > 0.0f)
	{
		t = lineDir.Dot(point - linePos) / lenSqV;
	}

	crossPoint = linePos + lineDir * t;

	return (crossPoint - point).Length();
}

	/// <summary>
	/// �_�Ɛ����̍ŒZ����
	/// </summary>
	/// <param name="point">�_�̍��W</param>
	/// <param name="segmentStartPoint">�����̎n�_</param>
	/// <param name="segmentEndPoint">�����̏I�_</param>
	/// <param name="minPoint">�ŒZ�����̓_(�߂�l)</param>
	/// <param name="t">�[�_�̃x�N�g���W��?</param>
	/// <returns></returns>
float MyCapsuleCollider::CalcPointSegmentDist(const Vector3& point, const Vector3& segmentStartPoint, const Vector3& segmentEndPoint, Vector3& minPoint, float& t)
{
	//�����̌������擾
	Vector3 segment = segmentEndPoint - segmentStartPoint;
	Vector3 segmentDir = segment;
	segmentDir.Normalize();

	//�����̒����A�����̑��̍��W�At���Z�o
	float len = CalcPointLineDist(point, segmentStartPoint, segmentDir, minPoint, t);
	
	//NOTE:�T�C�g�ł�t�͐����̎n�_�I�_�Ɛ����Ȑ�����݂͂łĂ��邩�̔��f�p�ʒu�ɂȂ��Ă��邪
	//�����Ŋ���Ȃ��ƃ_�����ۂ���?
	if (segment.Length() > FLT_EPSILON)
	{
		t /= segment.Length();
	}

	//��Point,segmentStartPoint,segmentEndPoint���݊p�Ȃ�
	//�������O���ōŒZ�_�͐����̎n�_
	Vector3 v1 = point - segmentStartPoint;
	v1.Normalize();
	Vector3 v2 = segmentEndPoint - segmentStartPoint;
	v2.Normalize();
	if (v1.Dot(v2) < 0)
	{
		minPoint = segmentStartPoint;
		return (segmentStartPoint - point).Length();
	}

	//��Point,segmentEndPoint,segmentStartPoint���݊p�Ȃ�
	//�������O���ōŒZ�_�͐����̏I�_
	v1 = point - segmentEndPoint;
	v1.Normalize();
	v2 = segmentStartPoint - segmentEndPoint;
	v2.Normalize();
	if (v1.Dot(v2) < 0)
	{
		minPoint = segmentEndPoint;
		return (segmentEndPoint - point).Length();
	}

	//�ǂ���̏������������Ȃ��Ȃ�ŒZ�_�͒����̎��Ɠ����B
	return len;
}

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
	/// <returns></returns>
float MyCapsuleCollider::CalcLineLineDist(const Vector3& line1StartPoint, const Vector3& line1EndPoint, const Vector3& line2StartPoint, const Vector3& line2EndPoint, Vector3& line1CrossPoint, Vector3& line2CrossPoint, float& t1, float& t2)
{
	//2���������s?
	Vector3 line1Dir = line1EndPoint - line1StartPoint;
	line1Dir.Normalize();
	Vector3 line2Dir = line2EndPoint - line2StartPoint;
	line2Dir.Normalize();

	float t = line1Dir.Dot(line2Dir);
	if (fabs(t) > (1 - FLT_EPSILON))
	{
		float len = CalcPointLineDist(line1StartPoint,line2StartPoint,line2Dir,line2CrossPoint,t2);
		line1CrossPoint = line1StartPoint;
		t1 = 0.0f;

		return len;
	}

	//2�����͂˂���֌W
	float DV1V2 = t;
	float DV1V1 = line1Dir.Dot(line1Dir);
	float DV2V2 = line2Dir.Dot(line2Dir);

	Vector3 P21P11 = line1StartPoint - line2StartPoint;
	t1 = (DV1V2 * line2Dir.Dot(P21P11) - DV2V2 * line1Dir.Dot(P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
	line1CrossPoint = line1StartPoint + line1Dir * t1;

	t2 = line2Dir.Dot(line1CrossPoint - line2StartPoint) / DV2V2;
	line2CrossPoint = line2StartPoint + line2Dir * t2;

	return (line2CrossPoint - line1CrossPoint).Length();
}

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
float MyCapsuleCollider::CalcSegmentSegmentDist(const Vector3& segment1StartPoint, const Vector3& segment1EndPoint, const Vector3& segment2StartPoint, const Vector3& segment2EndPoint, Vector3& segment1CrossPoint, Vector3& segment2CrossPoint, float& t1, float& t2)
{
	Vector3 segment1 = segment1EndPoint - segment1StartPoint;
	Vector3 segment2 = segment2EndPoint - segment2StartPoint;

	//����1���k�ނ��Ă���?
	if (segment1.Length() < FLT_EPSILON)
	{
		//����2���k�ނ��Ă���?
		if (segment2.Length() < FLT_EPSILON)
		{
			//�_�Ɠ_�̋����̖��ɂȂ�
			float len = (segment2StartPoint - segment1StartPoint).Length();
			segment1CrossPoint = segment1StartPoint;
			segment2CrossPoint = segment2StartPoint;
			t1 = t2 = 0.0f;
			return len;
		}
		else
		{
			//����1�̎n�_�Ɛ���2�̍ŒZ�������ɂȂ�
			float len = CalcPointSegmentDist(segment1StartPoint, segment2StartPoint, segment2EndPoint, segment2CrossPoint, t2);
			segment1CrossPoint = segment1StartPoint;
			t1 = 0.0f;

			//�W�����������͂ݏo�Ă��܂��Ă�����N�����v����B
			if (t2 < 0.0f)
			{
				t2 = 0.0f;
			}
			else if (t2 > 1.0f)
			{
				t2 = 1.0f;
			}

			//���̒i�K����t2�͐����̉����̒������Ƃ��������Ȃ̂ŁA
			//�����̒����������ċ����ɂȂ����B
			t2 *= segment2.Length();

			return len;
		}
	}
	else if (segment2.Length() < FLT_EPSILON)//����2���k�ނ��Ă���?
	{
		//����2�̎n�_�Ɛ���1�̍ŒZ���ɂȂ�
		float len = CalcPointSegmentDist(segment2StartPoint, segment1StartPoint, segment1EndPoint, segment1CrossPoint, t1);

		//�W�����������͂ݏo�Ă��܂��Ă�����N�����v����B
		if (t1 < 0.0f)
		{
			t1 = 0.0f;
		}
		else if (t1 > 1.0f)
		{
			t1 = 1.0f;
		}

		//���̒i�K����t1�͐����̉����̒������Ƃ��������Ȃ̂ŁA
		//�����̒����������ċ����ɂȂ����B
		t1 *= segment1.Length();

		segment2CrossPoint = segment2StartPoint;
		t2 = 0.0f;

		return len;
	}

	//��������͐������m



	//2���������s?
	Vector3 segment1Dir = segment1;
	segment1Dir.Normalize();
	Vector3 segment2Dir = segment2;
	segment2Dir.Normalize();

	float t = segment1Dir.Dot(segment2Dir);
	if (fabs(t) > (1 - FLT_EPSILON))
	{
		//���s�������琂���̒[�_�̈���ŒZ�����̓_�ɉ����肷��
		t1 = 0.0f;
		segment1CrossPoint = segment1StartPoint;

		float len = CalcPointSegmentDist(segment1StartPoint, segment2StartPoint, segment2EndPoint, segment2CrossPoint, t2);

		//t2�������͈͓̔��Ȃ画�芮��
		if (0.0f <= t2 && t2 <= 1.0f)
		{
			return len;
		}
	}
	else
	{
		//�����͂˂���̊֌W
		//2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
		float len = CalcLineLineDist(segment1StartPoint, segment1EndPoint, segment2StartPoint, segment2EndPoint, segment1CrossPoint, segment2CrossPoint, t1, t2);

		//CalcPointSegment�ł�t1,t2�͐����̉����̒������Ƃ��������ɂȂ邪�A
		//CalcLineLine�ł͂Ȃ��Ă��Ȃ��̂Ő����̒����Ŋ���B

		t1 /= segment1.Length();
		t2 /= segment2.Length();

		//t1�At2�Ƃ��ɐ����͈͓̔��Ȃ画�芮��
		if (0.0f <= t1 && t1 <= 1.0f && 0.0f <= t2 && t2 <= 1.0f)
		{
			return len;
		}
	}

	//�����̑��������̊O�ɂ���B
	//����1��t1��0�`1�ɃN�����v���Đ��������낷�B
	if (t1 < 0.0f)
	{
		t1 = 0.0f;
	}
	else if (t1 > 1.0f)
	{
		t1 = 1.0f;
	}

	segment1CrossPoint = segment1StartPoint + segment1Dir * segment1.Length() * t;

	float len = CalcPointSegmentDist(segment1CrossPoint, segment2StartPoint, segment2EndPoint, segment2CrossPoint, t2);

	//t2�������͈͓̔��Ȃ画�芮��
	if (0.0f <= t2 && t2 <= 1.0f)
	{
		return len;
	}

	//����2���̍ŒZ����������2���O���Ȃ̂�t2���N�����v���A����1�ɐ������~�낷
	if (t2 < 0.0f)
	{
		t2 = 0.0f;
	}
	else if (t2 > 1.0f)
	{
		t2 = 1.0f;
	}

	segment2CrossPoint = segment2StartPoint + segment2Dir * segment2.Length() * t2;

	len = CalcPointSegmentDist(segment2CrossPoint, segment1StartPoint, segment1EndPoint, segment1CrossPoint, t1);

	//t1�������͈͓̔��Ȃ画�芮��
	if (0.0f <= t1 && t1 <= 1.0f)
	{
		return len;
	}

	//�o���̒[�_���ŒZ�����B
	if (t1 < 0.0f)
	{
		t1 = 0.0f;
	}
	else if (t1 > 1.0f)
	{
		t1 = 1.0f;
	}

	segment1CrossPoint = segment1StartPoint + segment1Dir * segment1.Length() * t1;

	return (segment2CrossPoint - segment1CrossPoint).Length();
};