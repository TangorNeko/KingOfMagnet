#pragma once
#include <map>
#include <utility>
#include "stdafx.h"
#include "CDirectionLight.h"
#include "CPointLight.h"
#include "CSpotLight.h"

//各ライトの最大数　この数はmodel.fxのライト配列の数と一緒にしなければならない
const int MaxDirectionLightNum = 5;
const int MaxPointLightNum = 20;
const int MaxSpotLightNum = 20;

//TODO:ライトを作る際最大数以上の時throwするのではなく一番古いライトを消すような処理にする方がよい?
//勝手に消す(例:一番古いライトタグに対応した数字を-1にし、UpdateLightで-1ならLight.cppでDelete(this)させる)
//と無効なアドレスに値を入れることになりそうだが
//データを使わないだけで消さないようにする(例:古いライトタグに対応した数字を-1にするだけ)
//とそれはそれでメモリの無駄遣いになりそう
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
		Matrix lightCameraProjectionMatrix;								//ライトビュープロジェクション行列
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

	Camera m_lightCamera;//シャドウマップ用のライトの位置のカメラ。とりあえずテスト。
public:
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new CLightManager;
		}
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
	void UpdateEyePos(int camNo) { m_ligData.eyePos = g_camera3D[camNo]->GetPosition(); }

	Camera* GetLightCamera()
	{
		return &m_lightCamera;
	}

	//シャドウ用
	void SetLightCameraPosition(const Vector3& pos)
	{
		m_lightCamera.SetPosition(pos);
		m_lightCamera.Update();
	}

	void SetLightCameraTarget(const Vector3& targetPos)
	{
		m_lightCamera.SetTarget(targetPos);
		m_lightCamera.Update();
	}

	void SetLightCameraUp(const Vector3& up)
	{
		m_lightCamera.SetUp(up);
		m_lightCamera.Update();
	}

	void SetLightCameraViewAngle(const float& deg)
	{
		m_lightCamera.SetViewAngle(Math::DegToRad(deg));
		m_lightCamera.Update();
	}

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
		{
			throw;
		}
	}

	//ディレクションライトを追加する
	int AddDirectionLight(prefab::DirLigData* dirLigData)
	{
		//ライトの数が最初に決めた数以上ならthrowする(いっぱい作るとふつうに起こる)
		if (m_ligData.directionLightNum >= MaxDirectionLightNum)
		{
			throw;
		}

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

		//ライトの数が0以下になる時はおかしくなっているのでthrowする(起こり得ないと信じたい)
		if (m_ligData.pointLightNum < 0)
		{
			throw;
		}
	}

	//ポイントライトを追加する
	int AddPointLight(prefab::PointLigData* pointLigData)
	{
		//ライトの数が最初に決めた数以上ならthrowする(いっぱい作るとふつうに起こる)
		if (m_ligData.pointLightNum >= MaxPointLightNum)
		{
			throw;
		}

		//空きの中で一番先頭位置にデータを格納する
		m_ligData.pointLightArray[m_ligData.pointLightNum] = *pointLigData;

		//作ったライトのタグ番号とデータの格納位置を関連付ける
		m_pointLigMap.insert(std::make_pair(m_pointLigNum, m_ligData.pointLightNum));

		//次にライトが入る位置をずらす
		PointLightPlus();

		//作ったライトのタグ番号を返し、1増やして次のライトの作成に備える
		return m_pointLigNum++;
	}

	//ポイントライトを削除する
	void RemovePointLight(int pointLightTag)
	{
		//タグから削除する位置を取得し、削除するライトをソートで一番終端に持っていく
		for (int i = m_pointLigMap.at(pointLightTag);i < m_ligData.pointLightNum - 1;i++)
		{
			std::swap(m_ligData.pointLightArray[i], m_ligData.pointLightArray[i + 1]);
		}

		//ライトの数を減らすことで空いた位置に次のライトを入れられるようになる
		PointLightMinus();

		//削除したライト以降のライトの位置番号が一つずつずれたのでタグに対応した位置の番号を1ずつ減らす
		for (auto i = m_pointLigMap.upper_bound(pointLightTag);i != m_pointLigMap.end();i++)
		{
			i->second--;
		}
	}

	//ポイントライトの情報を更新する
	void UpdatePointLight(int pointLightTag, prefab::PointLigData* pointLigData)
	{
		m_ligData.pointLightArray[m_pointLigMap.at(pointLightTag)] = *pointLigData;
	}

	//スポットライト用/////////////////////////////////////////////////////////////////////////////////////////////////////

	//スポットライトの数のカウントをプラスする
	int SpotLightPlus() { return m_ligData.spotLightNum++; }

	//スポットライトの数のカウントをマイナスする
	void SpotLightMinus()
	{
		m_ligData.spotLightNum--;

		//ライトの数が0以下になる時はおかしくなっているのでthrowする(起こり得ないと信じたい)
		if (m_ligData.spotLightNum < 0)
		{
			throw;
		}
	}

	//スポットライトを追加する
	int AddSpotLight(prefab::SpotLigData* spotLigData)
	{
		//ライトの数が最初に決めた数以上ならthrowする(いっぱい作るとふつうに起こる)
		if (m_ligData.spotLightNum >= MaxSpotLightNum)
		{
			throw;
		}

		//空きの中で一番先頭位置にデータを格納する
		m_ligData.spotLightArray[m_ligData.spotLightNum] = *spotLigData;

		//作ったライトのタグ番号とデータの格納位置を関連付ける
		m_spotLigMap.insert(std::make_pair(m_spotLigNum, m_ligData.spotLightNum));

		//次にライトが入る位置をずらす
		SpotLightPlus();

		//作ったライトのタグ番号を返し、1増やして次のライト作成に備える
		return m_spotLigNum++;
	}

	//スポットライトを削除する
	void RemoveSpotLight(int spotLightTag)
	{
		//タグから削除する位置を取得し、削除するライトをソートで一番終端に持っていく
		for (int i = m_spotLigMap.at(spotLightTag);i < m_ligData.spotLightNum - 1;i++)
		{
			std::swap(m_ligData.spotLightArray[i], m_ligData.spotLightArray[i + 1]);
		}

		//ライトの数を減らすことで空いた位置に次のライトを入れられるようになる
		SpotLightMinus();

		//削除したライト以降のライトの位置番号が一つずつずれたのでタグに対応した位置の番号を1ずつ減らす
		for (auto i = m_spotLigMap.upper_bound(spotLightTag);i != m_spotLigMap.end();i++)
		{
			i->second--;
		}
	}

	//スポットライトの情報を更新する
	void UpdateSpotLight(int spotLightTag, prefab::SpotLigData* spotLigData)
	{
		m_ligData.spotLightArray[m_spotLigMap.at(spotLightTag)] = *spotLigData;
	}

};

