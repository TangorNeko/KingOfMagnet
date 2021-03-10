#pragma once
class TriangleCollider
{
private:
	Vector3 m_vertex[3] = { Vector3::Zero,Vector3::Zero,Vector3::Zero };
public:
	void SetVertex(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		m_vertex[0] = v1;
		m_vertex[1] = v2;
		m_vertex[2] = v3;
	}

	bool isHit(const Vector3& start, const Vector3& end);
};

