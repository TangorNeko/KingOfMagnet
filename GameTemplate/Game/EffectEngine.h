#pragma once

#include <map>
#include <string>
/// <summary>
/// エフェクトエンジンクラス。
/// </summary>
class EffectEngine {
	static EffectEngine* m_instance;	//唯一のインスタンス。
	//3D用
	EffekseerRenderer::RendererRef m_renderer[2];	//レンダラー。
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> m_memoryPool[2];	//メモリプール。
	Effekseer::RefPtr<EffekseerRenderer::CommandList> m_commandList[2];			//コマンドリスト。
	Effekseer::ManagerRef m_manager[2];

	//2D用
	EffekseerRenderer::RendererRef m_2Drenderer;//2D用レンダラー。
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> m_2DmemoryPool;	//2D用メモリプール。
	Effekseer::RefPtr<EffekseerRenderer::CommandList> m_2DcommandList;			//コマンドリスト。
	Effekseer::ManagerRef m_2Dmanager;

	//FlyWeight用
	std::map< std::u16string, Effekseer::EffectRef > m_effectMap;
public:
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new EffectEngine;
		}
	}

	/// <summary>
	/// インスタンスの取得。
	/// </summary>
	/// <returns></returns>
	static EffectEngine* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// インスタンスの削除。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;

	}

	/// <summary>
	/// エフェクトが再生中か判定。
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	bool IsPlay(int handle) const
	{
		//NOTE:2つのエフェクトが別々に再生されることはないのでm_manager[0]の結果を返せば大丈夫?
		return m_manager[0]->GetShown(handle);
	}

	/// <summary>
	/// エフェクトのワールド行列を更新。
	/// </summary>
	/// <param name="handle"></param>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	void UpdateEffectWorldMatrix(int handle, Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mTrans, mRot, mScale, mBase;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		mBase = mScale * mRot;
		mBase = mBase * mTrans;

		Effekseer::Matrix43 baseMat;
		baseMat.Value[0][0] = mBase.m[0][0];
		baseMat.Value[0][1] = mBase.m[0][1];
		baseMat.Value[0][2] = mBase.m[0][2];

		baseMat.Value[1][0] = mBase.m[1][0];
		baseMat.Value[1][1] = mBase.m[1][1];
		baseMat.Value[1][2] = mBase.m[1][2];

		baseMat.Value[2][0] = mBase.m[2][0];
		baseMat.Value[2][1] = mBase.m[2][1];
		baseMat.Value[2][2] = mBase.m[2][2];

		baseMat.Value[3][0] = mBase.m[3][0];
		baseMat.Value[3][1] = mBase.m[3][1];
		baseMat.Value[3][2] = mBase.m[3][2];

		m_manager[0]->SetBaseMatrix(handle, baseMat);
		m_manager[1]->SetBaseMatrix(handle, baseMat);
	}

	/// <summary>
	/// エフェクトを再生。
	/// </summary>
	/// <param name="filePath">再生したいエフェクト</param>
	/// <returns>再生したエフェクトのハンドル。</returns>
	int Play(Effekseer::EffectRef effect);

	/// <summary>
	/// エフェクトを停止。
	/// </summary>
	/// <param name="effectHandle"></param>
	void Stop(int effectHandle);

	/// <summary>
	/// エフェクトをロード。
	/// </summary>
	Effekseer::EffectRef LoadEffect(const char16_t* filePath);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間。</param>
	void Update(float deltaTime,int cameraNum);

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw(int cameraNum);

	/// <summary>
	/// 2Dエフェクトが再生中か判定。
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	bool IsPlay2D(int handle) const
	{
		return m_2Dmanager->GetShown(handle);
	}

	/// <summary>
	/// 2Dエフェクトのワールド行列を更新。
	/// </summary>
	/// <param name="handle"></param>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	void UpdateEffectWorldMatrix2D(int handle, Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mTrans, mRot, mScale, mBase;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		mBase = mScale * mRot;
		mBase = mBase * mTrans;

		Effekseer::Matrix43 baseMat;
		baseMat.Value[0][0] = mBase.m[0][0];
		baseMat.Value[0][1] = mBase.m[0][1];
		baseMat.Value[0][2] = mBase.m[0][2];

		baseMat.Value[1][0] = mBase.m[1][0];
		baseMat.Value[1][1] = mBase.m[1][1];
		baseMat.Value[1][2] = mBase.m[1][2];

		baseMat.Value[2][0] = mBase.m[2][0];
		baseMat.Value[2][1] = mBase.m[2][1];
		baseMat.Value[2][2] = mBase.m[2][2];

		baseMat.Value[3][0] = mBase.m[3][0];
		baseMat.Value[3][1] = mBase.m[3][1];
		baseMat.Value[3][2] = mBase.m[3][2];

		m_2Dmanager->SetBaseMatrix(handle, baseMat);
	}

	/// <summary>
	/// 2Dエフェクトを再生。
	/// </summary>
	/// <param name="filePath">再生したいエフェクト</param>
	/// <returns>再生したエフェクトのハンドル。</returns>
	int Play2D(Effekseer::EffectRef effect);

	/// <summary>
	/// 2Dエフェクトを停止。
	/// </summary>
	/// <param name="effectHandle"></param>
	void Stop2D(int effectHandle);

	/// <summary>
	/// 2Dエフェクトをロード。
	/// </summary>
	Effekseer::EffectRef LoadEffect2D(const char16_t* filePath);

	/// <summary>
	/// 2Dエフェクトを更新
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間。</param>
	void Update2D(float deltaTime);

	/// <summary>
	/// 2Dエフェクトを描画。
	/// </summary>
	void Draw2D();
private:
	EffectEngine();
	~EffectEngine();
};