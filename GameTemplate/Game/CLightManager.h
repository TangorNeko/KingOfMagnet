#pragma once
#include "stdafx.h"
#include "CDirectionLight.h"
#include "CPointLight.h"

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
		prefab::DirLigData directionLightArray[5];
		prefab::PointLigData pointLightArray[10];
		Vector3 eyePos;
		int directionLightNum = 0;
		int pointLightNum = 0;
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
		if (m_ligData.directionLightNum >= 4)
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
		if (m_ligData.pointLightNum >= 9)
			throw;

		m_ligData.pointLightArray[m_ligData.pointLightNum] = *pointLigData;

		return PointLightPlus();
	}

	void UpdatePointLight(int pointLightNum, prefab::PointLigData* pointLigData)
	{
		m_ligData.pointLightArray[pointLightNum] = *pointLigData;
	}
};

