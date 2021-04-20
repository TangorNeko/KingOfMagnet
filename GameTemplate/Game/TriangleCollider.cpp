#include "stdafx.h"
#include "TriangleCollider.h"

bool TriangleCollider::isHit(const Vector3& start, const Vector3& end, Vector3& finalCrossPoint)
{
	//1.�|���S�����܂ޖ������ʂƂ̌�������

	//3�̒��_����m_vertex[0]����L�т�@�������߂�
	Vector3 v1 = m_vertex[1] - m_vertex[0];
	Vector3 v2 = m_vertex[2] - m_vertex[0];

	Vector3 normal;
	normal.Cross(v1, v2);
	normal.Normalize();

	//�@���ƁAm_vertex[0]���画�肷������̊J�n�n�_�ƏI���n�_�œ��ς��Ƃ�B
	Vector3 toStart = start - m_vertex[0];
	toStart.Normalize();

	Vector3 toEnd = end - m_vertex[0];
	toEnd.Normalize();

	//���ς̌��ʂ�0�ȏ�Ȃ�������Ă��Ȃ��̂�false��Ԃ��B
	if (normal.Dot(toStart) * normal.Dot(toEnd) > 0)
	{
		return false;
	}

	//2.�������Ă�����W�����߂�
	toStart = start - m_vertex[0];

	toEnd = end - m_vertex[0];

	float a = normal.Dot(toStart);

	normal *= -1;

	float b = normal.Dot(toEnd);

	Vector3 crossPoint = toStart - toEnd;
	crossPoint *= b / (a + b);
	crossPoint += end;

	//3.��_���O�p�`�̒��ɂ��邩�ǂ����̔���

	Vector3 vA = m_vertex[1] - m_vertex[0];
	Vector3 vB = m_vertex[2] - m_vertex[1];
	Vector3 vC = m_vertex[0] - m_vertex[2];
	Vector3 vD = crossPoint - m_vertex[0];
	Vector3 vE = crossPoint - m_vertex[1];
	Vector3 vF = crossPoint - m_vertex[2];

	vA.Cross(vD);
	vA.Normalize();
	vB.Cross(vE);
	vB.Normalize();
	vC.Cross(vF);
	vC.Normalize();

	if (vA.Dot(vB) > 0 && vA.Dot(vC) > 0)
	{
		//�ŏI�I�Ƀq�b�g���Ă����ꍇ�̂݌�_���i�[�B
		finalCrossPoint = crossPoint;
		return true;
	}
	else
	{
		return false;
	}
}