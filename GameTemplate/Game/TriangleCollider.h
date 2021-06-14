#pragma once
class TriangleCollider
{
public:
	/**
	 * @brief 三角形の頂点の座標を設定
	 * @param v1 1つ目の頂点の座標
	 * @param v2 2つ目の頂点の座標
	 * @param v3 3つ目の頂点の座標
	*/
	void SetVertex(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		m_vertex[0] = v1;
		m_vertex[1] = v2;
		m_vertex[2] = v3;
	}

	/**
	 * @brief 線分が三角形に交差している?
	 * @param start 線分の始点
	 * @param end 線分の終点
	 * @param finalCrossPoint 線分の交差点(戻り値) 
	 * @return ヒットしたか(していたらtrue)
	*/
	bool isHit(const Vector3& start, const Vector3& end,Vector3& finalCrossPoint);

private:
	Vector3 m_vertex[3] = { Vector3::Zero,Vector3::Zero,Vector3::Zero };	//当たり判定の三角形の頂点
};

