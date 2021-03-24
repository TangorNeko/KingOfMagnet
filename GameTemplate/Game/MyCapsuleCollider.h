#pragma once
class MyCapsuleCollider
{
private:
	Vector3 m_capsuleStartPoint;
	Vector3 m_capsuleEndPoint;
	float m_radius;

	/// <summary>
	/// 点と直線の最短距離
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <param name="linePos">直線の一点</param>
	/// <param name="lineDir">直線の一点からの向き</param>
	/// <param name="crossPoint">直線上の最短距離の点(戻り値)</param>
	/// <param name="t">直線上の1点から最短距離の点への単位ベクトル係数(戻り値)</param>
	/// <returns>最短距離</returns>
	float CalcPointLineDist(const Vector3& point, const Vector3& linePos, const Vector3& lineDir, Vector3& crossPoint, float& t);
	
	/// <summary>
	/// 点と線分の最短距離
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <param name="segmentStartPoint">線分の始点</param>
	/// <param name="segmentEndPoint">線分の終点</param>
	/// <param name="minPoint">最短距離の点(戻り値)</param>
	/// <param name="t">端点のベクトル係数?</param>
	/// <returns>最短距離</returns>
	float CalcPointSegmentDist(const Vector3& point, const Vector3& segmentStartPoint, const Vector3& segmentEndPoint, Vector3& minPoint, float& t);

	/// <summary>
	/// 2直線の最短距離を求める
	/// </summary>
	/// <param name="line1StartPoint">直線1の始点</param>
	/// <param name="line1EndPoint">直線1の終点</param>
	/// <param name="line2StartPoint">直線2の始点</param>
	/// <param name="line2EndPoint">直線2の終点</param>
	/// <param name="line1CrossPoint">最短距離の直線1上の点(戻り値)</param>
	/// <param name="line2CrossPoint">最短距離の直線2上の点(戻り値)</param>
	/// <param name="t1">L1のベクトル係数(戻り値)</param>
	/// <param name="t2">L2のベクトル係数(戻り値)</param>
	/// <returns>最短距離</returns>
	float CalcLineLineDist(const Vector3& line1StartPoint, const Vector3& line1EndPoint, const Vector3& line2StartPoint, const Vector3& line2EndPoint, Vector3& line1CrossPoint, Vector3& line2CrossPoint, float& t1, float& t2);

	/// <summary>
	/// 2線分の最短距離を求める
	/// </summary>
	/// <param name="segment1StartPoint">線分1の始点</param>
	/// <param name="segment1EndPoint">線分1の終点</param>
	/// <param name="segment2StartPoint">線分2の始点</param>
	/// <param name="segment2EndPoint">線分2の終点</param>
	/// <param name="segment1CrossPoint">最短距離の線分1上の点</param>
	/// <param name="segment2CrossPoint">最短距離の線分2上の点</param>
	/// <param name="t1">線分1のベクトル係数</param>
	/// <param name="t2">線分2のベクトル係数</param>
	/// <returns>最短距離</returns>
	float CalcSegmentSegmentDist(const Vector3& segment1StartPoint, const Vector3& segment1EndPoint, const Vector3& segment2StartPoint, const Vector3& segment2EndPoint, Vector3& segment1CrossPoint, Vector3& segment2CrossPoint, float& t1, float& t2);

public:
	/// <summary>
	/// カプセルの始点をセットする。
	/// </summary>
	/// <param name="startPoint">始点の位置</param>
	void SetStartPoint(const Vector3& startPoint)
	{
		m_capsuleStartPoint = startPoint;
	}

	/// <summary>
	/// カプセルの終点をセットする。
	/// </summary>
	/// <param name="endPoint">終点の位置</param>
	void SetEndPoint(const Vector3& endPoint)
	{
		m_capsuleEndPoint = endPoint;
	}

	/// <summary>
	/// カプセルの半径をセットする。
	/// </summary>
	/// <param name="radius">半径の長さ</param>
	void SetRadius(const float& radius)
	{
		m_radius = radius;
	}

	/// <summary>
	/// カプセル同士が衝突しているか?
	/// </summary>
	/// <param name="capsule">相手側のカプセル</param>
	/// <returns>衝突しているならtrue</returns>
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

