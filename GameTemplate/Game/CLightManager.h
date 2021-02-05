#pragma once
#include <map>
#include <utility>
#include "stdafx.h"
#include "CDirectionLight.h"
#include "CPointLight.h"
#include "CSpotLight.h"

//各ライトの最大数　この数はmodel.fxのライト配列の数と一緒にしなければならない
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

	//ライトの情報とカメラの位置とライトの数を集めたデータ(定数バッファとしてHLSLに送られる構造体)
	struct LigDatas
	{
		prefab::DirLigData directionLightArray[MaxDirectionLightNum];	//ディレクションライトのデータの配列
		prefab::PointLigData pointLightArray[MaxPointLightNum];			//ポイントライトのデータの配列
		prefab::SpotLigData spotLightArray[MaxSpotLightNum];			//スポットライトのデータの配列
		Vector3 eyePos;													//カメラの位置
		int directionLightNum = 0;										//ディレクションライトの数
		int pointLightNum = 0;											//ポイントライトの数
		int spotLightNum = 0;											//スポットライトの数
	};

	LigDatas m_ligData;				//ライトのデータ
	int m_size = sizeof(m_ligData);	//ライトのデータのサイズ
	
	int m_dirLigNum = 0;				//次に作られるディレクションライトに付与するタグ番号(≒これまでに何個ディレクションライトが作られたか)
	int m_pointLigNum = 0;				//次に作られるポイントライトに付与するタグ番号(≒これまでに何個ポイントライトが作られたか)
	int m_spotLigNum = 0;				//次に作られるスポットライトに付与するタグ番号(≒これまでに何個スポットライトが作られたか)
	std::map<int, int> m_dirLigMap;		//ディレクションライトのタグから現在のディレクションライトの位置を返してくれるMap
	std::map<int, int> m_pointLigMap;	//ポイントライトのタグから現在のポイントライトの位置を返してくれるMap
	std::map<int, int> m_spotLigMap;	//スポットライトのタグから現在のスポットライトの位置を返してくれるMap
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

	//ライトのデータの塊を取得する(定数バッファに渡す用)
	LigDatas* GetLigDatas() { return &m_ligData; }

	//ライトのデータの塊のサイズを取得する(定数バッファに渡す用)
	int GetDataSize() { return m_size; }

	//カメラのポジションを更新する
	void UpdateEyePos() { m_ligData.eyePos = g_camera3D->GetPosition(); }

	//ディレクションライト用////////////////////////////////////////////////////////////////////////////////////////////////
	//Mapを使って安全にライトを削除できる仕組みを作ってみた。オブジェクトの削除がうまくいっていないためまだ検証できていない
	//TODO:ライトの削除の検証

	//ディレクションライトの数のカウントをプラスする
	int DirectionLightPlus() { return m_ligData.directionLightNum++; }

	//ディレクションライトの数のカウントをマイナスする
	void DirectionLightMinus() {
		m_ligData.directionLightNum--;

		//ライトの数が0以下になる時はおかしくなっているのでthrowする(起こり得ないと信じたい)
		if (m_ligData.directionLightNum < 0)
			throw;
	}

	//ディレクションライトを追加する
	int AddDirectionLight(prefab::DirLigData* dirLigData)
	{
		//ライトの数が最初に決めた数以上ならthrowする(いっぱい作るとふつうに起こる)
		if (m_ligData.directionLightNum >= MaxDirectionLightNum)
			throw;

		//空きの中で一番先頭位置にデータを格納する
		m_ligData.directionLightArray[m_ligData.directionLightNum] = *dirLigData;

		//作ったライトのタグ番号とデータの格納位置を関連付ける
		m_dirLigMap.insert(std::make_pair(m_dirLigNum, m_ligData.directionLightNum));

		//次にライトが入る位置をずらす
		DirectionLightPlus();

		//作ったライトのタグ番号を返し、1増やして次のライトの作成に備える
		return m_dirLigNum++;
	}

	//ディレクションライトを削除する
	void RemoveDirectionLight(int directionLightTag)
	{
		//タグから削除する位置を取得し、削除するライトをソートで一番終端に持っていく
		for (int i = m_dirLigMap.at(directionLightTag);i < m_ligData.directionLightNum - 1;i++)
		{
			std::swap(m_ligData.directionLightArray[i], m_ligData.directionLightArray[i + 1]);
		}

		//ライトの数を減らすことで空いた位置に次のライトを入れられるようになる
		DirectionLightMinus();

		//削除したライト以降のライトの位置番号が一つずつずれたのでタグに対応した位置の番号を1ずつ減らす
		for (auto i = m_dirLigMap.upper_bound(directionLightTag);i != m_dirLigMap.end();i++)
		{
			i->second--;
		}
	}

	//ディレクションライトの情報を更新する
	void UpdateDirectionLight(int directionLightTag, prefab::DirLigData* dirLigData)
	{
		//タグから取得したライトの位置のデータを更新する。
		m_ligData.directionLightArray[m_dirLigMap.at(directionLightTag)] = *dirLigData;
	}

	//ポイントライト用///////////////////////////////////////////////////////////////////////////////////////////////////////

	//ポイントライトの数のカウントをプラスする
	int PointLightPlus() { return m_ligData.pointLightNum++; }

	//ポイントライトの数のカウントをマイナスする
	//WARNING:明らかに危険 何番目のライトを消すか把握する必要がある
	void PointLightMinus()
	{
		m_ligData.pointLightNum--;
		if (m_ligData.pointLightNum < 0)
			throw;
	}

	//ポイントライトを追加する
	int AddPointLight(prefab::PointLigData* pointLigData)
	{
		if (m_ligData.pointLightNum >= MaxPointLightNum)
			throw;

		m_ligData.pointLightArray[m_ligData.pointLightNum] = *pointLigData;

		return PointLightPlus();
	}

	//ポイントライトの情報を更新する
	void UpdatePointLight(int pointLightNum, prefab::PointLigData* pointLigData)
	{
		m_ligData.pointLightArray[pointLightNum] = *pointLigData;
	}

	//スポットライト用/////////////////////////////////////////////////////////////////////////////////////////////////////

	//スポットライトの数のカウントをプラスする
	int SpotLightPlus() { return m_ligData.spotLightNum++; }

	//スポットライトの数のカウントをマイナスする
	void SpotLightMinus()
	{
		m_ligData.spotLightNum--;
		if (m_ligData.spotLightNum < 0)
			throw;
	}

	//スポットライトを追加する
	int AddSpotLight(prefab::SpotLigData* spotLigData)
	{
		if (m_ligData.spotLightNum >= MaxSpotLightNum)
			throw;

		m_ligData.spotLightArray[m_ligData.spotLightNum] = *spotLigData;

		return SpotLightPlus();
	}

	//スポットライトの情報を更新する
	void UpdateSpotLight(int spotLightNum, prefab::SpotLigData* spotLigData)
	{
		m_ligData.spotLightArray[spotLightNum] = *spotLigData;
	}

};

