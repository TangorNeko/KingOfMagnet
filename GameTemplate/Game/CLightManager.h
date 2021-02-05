#pragma once
#include "stdafx.h"
#include "CDirectionLight.h"
#include "CPointLight.h"
#include "CSpotLight.h"

const int MaxDirectionLightNum = 5;
const int MaxPointLightNum = 10;
const int MaxSpotLightNum = 10;

//TODO:ライトを消す時の仕組みができていない。明らかに一方通行
//削除した際にソートで左につめた後directionLightNumを減らす?
//そうするとCDirectionLight側でAddDirectionLightの時に受け取った照明の番号が無効になりそうなので
//UpdateDirectionLightの時に受け取ったナンバーから更新後のナンバーを渡せたらうれしい。
class CLightManager
{
private:
	CLightManager() = default;
	~CLightManager() = default;

	static CLightManager* m_instance;

	//ライトの情報とライトの数を集めたデータ(定数バッファとしてHLSLに送られる構造体)
	struct LigDatas
	{
		prefab::DirLigData directionLightArray[MaxDirectionLightNum];
		prefab::PointLigData pointLightArray[MaxPointLightNum];
		prefab::SpotLigData spotLightArray[MaxSpotLightNum];
		Vector3 eyePos;
		int directionLightNum = 0;
		int pointLightNum = 0;
		int spotLightNum = 0;
	};

	LigDatas m_ligData;
	int m_size = sizeof(m_ligData);
public:
	static void CreateInstance()
	{
		if (!m_instance)
			m_instance = new CLightManager;
	}

	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	static CLightManager* GetInstance() { return m_instance; }

	LigDatas* GetLigDatas() { return &m_ligData; }

	int GetDataSize() { return m_size; }

	void UpdateEyePos() { m_ligData.eyePos = g_camera3D->GetPosition(); }

	//ディレクションライト用
	int DirectionLightPlus() { return m_ligData.directionLightNum++; }

	//WARNING:明らかに危険 何番目のライトを消すか把握する必要がある
	void DirectionLightMinus() {
		m_ligData.directionLightNum--;
		if (m_ligData.directionLightNum < 0)
			throw;
	}

	int AddDirectionLight(prefab::DirLigData* dirLigData)
	{
		if (m_ligData.directionLightNum >= MaxDirectionLightNum)
			throw;

		m_ligData.directionLightArray[m_ligData.directionLightNum] = *dirLigData;

		return DirectionLightPlus();
	}

	void UpdateDirectionLight(int directionLightNum, prefab::DirLigData* dirLigData)
	{
		m_ligData.directionLightArray[directionLightNum] = *dirLigData;
	}

	//ポイントライト用
	int PointLightPlus() { return m_ligData.pointLightNum++; }

	void PointLightMinus()
	{
		m_ligData.pointLightNum--;
		if (m_ligData.pointLightNum < 0)
			throw;
	}

	int AddPointLight(prefab::PointLigData* pointLigData)
	{
		if (m_ligData.pointLightNum >= MaxPointLightNum)
			throw;

		m_ligData.pointLightArray[m_ligData.pointLightNum] = *pointLigData;

		return PointLightPlus();
	}

	void UpdatePointLight(int pointLightNum, prefab::PointLigData* pointLigData)
	{
		m_ligData.pointLightArray[pointLightNum] = *pointLigData;
	}

	//スポットライト用
	int SpotLightPlus() { return m_ligData.spotLightNum++; }

	void SpotLightMinus()
	{
		m_ligData.spotLightNum--;
		if (m_ligData.spotLightNum < 0)
			throw;
	}

	int AddSpotLight(prefab::SpotLigData* spotLigData)
	{
		if (m_ligData.spotLightNum >= MaxSpotLightNum)
			throw;

		m_ligData.spotLightArray[m_ligData.spotLightNum] = *spotLigData;

		return SpotLightPlus();
	}

	void UpdateSpotLight(int spotLightNum, prefab::SpotLigData* spotLigData)
	{
		m_ligData.spotLightArray[spotLightNum] = *spotLigData;
	}

};

