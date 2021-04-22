#include "stdafx.h"
#include "TriangleCollider.h"

bool TriangleCollider::isHit(const Vector3& start, const Vector3& end, Vector3& finalCrossPoint)
{
	//1.ポリゴンを含む無限平面との交差判定

	//3つの頂点からm_vertex[0]から伸びる法線を求める
	Vector3 v1 = m_vertex[1] - m_vertex[0];
	Vector3 v2 = m_vertex[2] - m_vertex[0];

	Vector3 normal;
	normal.Cross(v1, v2);
	normal.Normalize();

	//法線と、m_vertex[0]から判定する線分の開始地点と終了地点で内積をとる。
	Vector3 toStart = start - m_vertex[0];
	toStart.Normalize();

	Vector3 toEnd = end - m_vertex[0];
	toEnd.Normalize();

	//内積の結果が0以上なら交差していないのでfalseを返す。
	if (normal.Dot(toStart) * normal.Dot(toEnd) > 0)
	{
		return false;
	}

	//2.交差している座標を求める
	toStart = start - m_vertex[0];

	toEnd = end - m_vertex[0];

	float a = normal.Dot(toStart);

	normal *= -1;

	float b = normal.Dot(toEnd);

	Vector3 crossPoint = toStart - toEnd;
	crossPoint *= b / (a + b);
	crossPoint += end;

	//3.交点が三角形の中にあるかどうかの判定

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
		//最終的にヒットしていた場合のみ交点を格納。
		finalCrossPoint = crossPoint;
		return true;
	}
	else
	{
		return false;
	}
}