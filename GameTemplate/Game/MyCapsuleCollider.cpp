#include "stdafx.h"
#include "MyCapsuleCollider.h"

	/// <summary>
	/// 点と直線の最短距離
	/// </summary>
	/// <param name="point">点の位置</param>
	/// <param name="linePos">直線の一点</param>
	/// <param name="lineDir">直線の一点からの向き</param>
	/// <param name="crossPoint">直線上の最短距離の点(戻り値)</param>
	/// <param name="t">直線上の1点から最短距離の点への単位ベクトル係数(戻り値)</param>
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
	/// 点と線分の最短距離
	/// </summary>
	/// <param name="point">点の座標</param>
	/// <param name="segmentStartPoint">線分の始点</param>
	/// <param name="segmentEndPoint">線分の終点</param>
	/// <param name="minPoint">最短距離の点(戻り値)</param>
	/// <param name="t">端点のベクトル係数?</param>
	/// <returns></returns>
float MyCapsuleCollider::CalcPointSegmentDist(const Vector3& point, const Vector3& segmentStartPoint, const Vector3& segmentEndPoint, Vector3& minPoint, float& t)
{
	//線分の向きを取得
	Vector3 segment = segmentEndPoint - segmentStartPoint;
	Vector3 segmentDir = segment;
	segmentDir.Normalize();

	//垂線の長さ、垂線の足の座標、tを算出
	float len = CalcPointLineDist(point, segmentStartPoint, segmentDir, minPoint, t);
	
	//NOTE:サイトではtは線分の始点終点と垂直な線からはみでているかの判断用位置になっているが
	//長さで割らないとダメっぽそう?
	if (segment.Length() > FLT_EPSILON)
	{
		t /= segment.Length();
	}

	//∠Point,segmentStartPoint,segmentEndPointが鈍角なら
	//線分より外側で最短点は線分の始点
	Vector3 v1 = point - segmentStartPoint;
	v1.Normalize();
	Vector3 v2 = segmentEndPoint - segmentStartPoint;
	v2.Normalize();
	if (v1.Dot(v2) < 0)
	{
		minPoint = segmentStartPoint;
		return (segmentStartPoint - point).Length();
	}

	//∠Point,segmentEndPoint,segmentStartPointが鈍角なら
	//線分より外側で最短点は線分の終点
	v1 = point - segmentEndPoint;
	v1.Normalize();
	v2 = segmentStartPoint - segmentEndPoint;
	v2.Normalize();
	if (v1.Dot(v2) < 0)
	{
		minPoint = segmentEndPoint;
		return (segmentEndPoint - point).Length();
	}

	//どちらの条件も満たさないなら最短点は直線の時と同じ。
	return len;
}

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
	/// <returns></returns>
float MyCapsuleCollider::CalcLineLineDist(const Vector3& line1StartPoint, const Vector3& line1EndPoint, const Vector3& line2StartPoint, const Vector3& line2EndPoint, Vector3& line1CrossPoint, Vector3& line2CrossPoint, float& t1, float& t2)
{
	//2直線が平行?
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

	//2直線はねじれ関係
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
float MyCapsuleCollider::CalcSegmentSegmentDist(const Vector3& segment1StartPoint, const Vector3& segment1EndPoint, const Vector3& segment2StartPoint, const Vector3& segment2EndPoint, Vector3& segment1CrossPoint, Vector3& segment2CrossPoint, float& t1, float& t2)
{
	Vector3 segment1 = segment1EndPoint - segment1StartPoint;
	Vector3 segment2 = segment2EndPoint - segment2StartPoint;

	//線分1が縮退している?
	if (segment1.Length() < FLT_EPSILON)
	{
		//線分2も縮退している?
		if (segment2.Length() < FLT_EPSILON)
		{
			//点と点の距離の問題になる
			float len = (segment2StartPoint - segment1StartPoint).Length();
			segment1CrossPoint = segment1StartPoint;
			segment2CrossPoint = segment2StartPoint;
			t1 = t2 = 0.0f;
			return len;
		}
		else
		{
			//線分1の始点と線分2の最短距離問題になる
			float len = CalcPointSegmentDist(segment1StartPoint, segment2StartPoint, segment2EndPoint, segment2CrossPoint, t2);
			segment1CrossPoint = segment1StartPoint;
			t1 = 0.0f;

			//係数が線分よりはみ出てしまっていたらクランプする。
			if (t2 < 0.0f)
			{
				t2 = 0.0f;
			}
			else if (t2 > 1.0f)
			{
				t2 = 1.0f;
			}

			//今の段階だとt2は線分の何割の長さかという数字なので、
			//線分の長さをかけて距離になおす。
			t2 *= segment2.Length();

			return len;
		}
	}
	else if (segment2.Length() < FLT_EPSILON)//線分2が縮退している?
	{
		//線分2の始点と線分1の最短問題になる
		float len = CalcPointSegmentDist(segment2StartPoint, segment1StartPoint, segment1EndPoint, segment1CrossPoint, t1);

		//係数が線分よりはみ出てしまっていたらクランプする。
		if (t1 < 0.0f)
		{
			t1 = 0.0f;
		}
		else if (t1 > 1.0f)
		{
			t1 = 1.0f;
		}

		//今の段階だとt1は線分の何割の長さかという数字なので、
		//線分の長さをかけて距離になおす。
		t1 *= segment1.Length();

		segment2CrossPoint = segment2StartPoint;
		t2 = 0.0f;

		return len;
	}

	//ここからは線分同士



	//2直線が平行?
	Vector3 segment1Dir = segment1;
	segment1Dir.Normalize();
	Vector3 segment2Dir = segment2;
	segment2Dir.Normalize();

	float t = segment1Dir.Dot(segment2Dir);
	if (fabs(t) > (1 - FLT_EPSILON))
	{
		//平行だったら垂線の端点の一つを最短距離の点に仮決定する
		t1 = 0.0f;
		segment1CrossPoint = segment1StartPoint;

		float len = CalcPointSegmentDist(segment1StartPoint, segment2StartPoint, segment2EndPoint, segment2CrossPoint, t2);

		//t2が線分の範囲内なら判定完了
		if (0.0f <= t2 && t2 <= 1.0f)
		{
			return len;
		}
	}
	else
	{
		//線分はねじれの関係
		//2直線間の最短距離を求めて仮のt1,t2を求める
		float len = CalcLineLineDist(segment1StartPoint, segment1EndPoint, segment2StartPoint, segment2EndPoint, segment1CrossPoint, segment2CrossPoint, t1, t2);

		//CalcPointSegmentではt1,t2は線分の何割の長さかという数字になるが、
		//CalcLineLineではなっていないので線分の長さで割る。

		t1 /= segment1.Length();
		t2 /= segment2.Length();

		//t1、t2ともに線分の範囲内なら判定完了
		if (0.0f <= t1 && t1 <= 1.0f && 0.0f <= t2 && t2 <= 1.0f)
		{
			return len;
		}
	}

	//垂線の足が線分の外にある。
	//線分1のt1を0～1にクランプして垂線を下ろす。
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

	//t2が線分の範囲内なら判定完了
	if (0.0f <= t2 && t2 <= 1.0f)
	{
		return len;
	}

	//線分2側の最短距離が線分2より外側なのでt2をクランプし、線分1に垂線を降ろす
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

	//t1が線分の範囲内なら判定完了
	if (0.0f <= t1 && t1 <= 1.0f)
	{
		return len;
	}

	//双方の端点が最短距離。
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