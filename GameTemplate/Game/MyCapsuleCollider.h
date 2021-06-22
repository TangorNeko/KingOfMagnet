#pragma once
class MyCapsuleCollider
{
private:
	
	/**
	 * @brief 点と直線の最短距離
	 * @param point 点の座標
	 * @param linePos 直線の一点
	 * @param lineDir 直線の一点からの向き
	 * @param crossPoint 直線上の最短距離の点(戻り値)
	 * @param t 直線上の1点から最短距離の点への単位ベクトル係数(戻り値)
	 * @return 最短距離
	*/
	float CalcPointLineDist(const Vector3& point, const Vector3& linePos, const Vector3& lineDir, Vector3& crossPoint, float& t);
	
	/**
	 * @brief 点と線分の最短距離
	 * @param point 点の座標
	 * @param segmentStartPoint 線分の始点
	 * @param segmentEndPoint 線分の終点
	 * @param minPoint 最短距離の点(戻り値)
	 * @param t 端点のベクトル係数(戻り値)
	 * @return 最短距離
	*/
	float CalcPointSegmentDist(const Vector3& point, const Vector3& segmentStartPoint, const Vector3& segmentEndPoint, Vector3& minPoint, float& t);

	/**
	 * @brief 2直線の最短距離を求める
	 * @param line1StartPoint 直線1の始点
	 * @param line1EndPoint 直線1の終点
	 * @param line2StartPoint 直線2の始点
	 * @param line2EndPoint 直線2の終点
	 * @param line1CrossPoint 最短距離の直線1上の点(戻り値)
	 * @param line2CrossPoint 最短距離の直線2上の点(戻り値)
	 * @param t1 L1のベクトル係数(戻り値)
	 * @param t2 L2のベクトル係数(戻り値)
	 * @return 
	*/
	float CalcLineLineDist(const Vector3& line1StartPoint, const Vector3& line1EndPoint, const Vector3& line2StartPoint, const Vector3& line2EndPoint, Vector3& line1CrossPoint, Vector3& line2CrossPoint, float& t1, float& t2);

	/**
	 * @brief 2線分の最短距離を求める
	 * @param segment1StartPoint 線分1の始点
	 * @param segment1EndPoint 線分1の終点
	 * @param segment2StartPoint 線分2の始点
	 * @param segment2EndPoint 線分2の終点
	 * @param segment1CrossPoint 最短距離の線分1上の点
	 * @param segment2CrossPoint 最短距離の線分2上の点
	 * @param t1 線分1のベクトル係数(戻り値)
	 * @param t2 線分2のベクトル係数(戻り値)
	 * @return 最短距離
	*/
	float CalcSegmentSegmentDist(const Vector3& segment1StartPoint, const Vector3& segment1EndPoint, const Vector3& segment2StartPoint, const Vector3& segment2EndPoint, Vector3& segment1CrossPoint, Vector3& segment2CrossPoint, float& t1, float& t2);

public:

	/**
	 * @brief カプセルの始点をセットする
	 * @param startPoint 始点の位置
	*/
	void SetStartPoint(const Vector3& startPoint)
	{
		m_capsuleStartPoint = startPoint;
	}

	/**
	 * @brief カプセルの終点をセットする
	 * @param endPoint 終点の位置
	*/
	void SetEndPoint(const Vector3& endPoint)
	{
		m_capsuleEndPoint = endPoint;
	}

	/**
	 * @brief カプセルの半径をセットする
	 * @param radius 半径の長さ
	*/
	void SetRadius(const float& radius)
	{
		m_radius = radius;
	}

	/**
	 * @brief カプセル同士が衝突しているか?
	 * @param capsule 相手側のカプセル
	 * @return 衝突しているならtrue
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
	Vector3 m_capsuleStartPoint;	//カプセルの始点
	Vector3 m_capsuleEndPoint;		//カプセルの終点
	float m_radius;					//カプセルの半径
};

