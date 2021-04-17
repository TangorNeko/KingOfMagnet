#include "stdafx.h"
#include "CSkinModelRender.h"

namespace prefab
{
	/// <summary>
	/// モデルの初期化関数。アニメーションつき。
	/// </summary>
	/// <param name="modelPath">モデルファイルのパス(.tkm)</param>
	/// <param name="skeletonPath">スケルトンファイルのパス(.tks)</param>
	/// <param name="animationClips">アニメーションクリップの配列</param>
	/// <param name="animationClipNum">アニメーションクリップの数</param>
	void CSkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum)
	{
		ModelInitData initData;
		ModelInitData shadowModelInitData;

		initData.m_tkmFilePath = modelPath;
		shadowModelInitData.m_tkmFilePath = modelPath;

		initData.m_fxFilePath = "Assets/shader/shadowReceiver.fx";
		shadowModelInitData.m_fxFilePath = "Assets/shader/shadow.fx";

		initData.m_vsEntryPointFunc = "VSMain";
		shadowModelInitData.m_vsEntryPointFunc = "VSMain";

		initData.m_vsSkinEntryPointFunc = "VSSkinMain";
		shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		if (skeletonPath != nullptr)
		{
			m_skeleton.Init(skeletonPath);
			initData.m_skeleton = &m_skeleton;
			shadowModelInitData.m_skeleton = &m_skeleton;
		}

		initData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		shadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32_FLOAT;

		initData.m_modelUpAxis = enModelUpAxisZ;
		shadowModelInitData.m_modelUpAxis = enModelUpAxisZ;

		initData.m_expandShaderResoruceView = &PostEffectManager::GetInstance()->GetBlurShadowMap();

		//定数バッファをモデルに紐付ける
		initData.m_expandConstantBufferSize = CLightManager::GetInstance()->GetDataSize();
		shadowModelInitData.m_expandConstantBufferSize = CLightManager::GetInstance()->GetDataSize();
		initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLigDatas();
		shadowModelInitData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLigDatas();

		m_model[eModel_View1].Init(initData);
		m_model[eModel_View2].Init(initData);

		//initData.m_fxFilePath = "Assets/shader/shadow.fx";
		//initData.m_colorBufferFormat = DXGI_FORMAT_R32G32_FLOAT;

		m_model[eModel_Shadow].Init(shadowModelInitData);

		m_animationClips = animClips;
		m_animationClipNum = animClipNum;

		if (m_animationClips != nullptr)
		{
			m_animation.Init(m_skeleton, m_animationClips, m_animationClipNum);
		}
	}

	/// <summary>
	/// モデルの初期化関数。
	/// </summary>
	/// <param name="modelPath">モデルファイルのパス(.tkm)</param>
	/// <param name="skeletonPath">スケルトンファイルのパス(.tks)</param>
	void CSkinModelRender::Init(const char* modelPath, const char* skeletonPath)
	{
		Init(modelPath, skeletonPath, nullptr, 0);
	}

	/// <summary>
	/// モデルの初期化関数。モデルパスだけ版。
	/// </summary>
	/// <param name="modelPath">モデルファイルのパス(.tkm)</param>
	void CSkinModelRender::Init(const char* modelPath)
	{
		Init(modelPath, nullptr, nullptr, 0);
	}

	//モデルの描画。
	void CSkinModelRender::Render(RenderContext& rc, Camera* camera)
	{
		switch (rc.GetRenderStep()) {
		case RenderContext::eStep_RenderViewport1:
			m_model[eModel_View1].Draw(rc, camera);
			break;
		case RenderContext::eStep_RenderViewport2:
			m_model[eModel_View2].Draw(rc, camera);
			break;
		case RenderContext::eStep_RenderShadowMap:
			if (m_isShadowCaster)
			{
				m_model[eModel_Shadow].Draw(rc, camera);
			}
			break;
		}
	}

	/// <summary>
	/// モデルの情報を更新する。
	/// </summary>
	void CSkinModelRender::UpdateModel()
	{
		for (auto& model : m_model) {
			model.UpdateWorldMatrix(m_position, m_qRot, m_scale);
			m_skeleton.Update(model.GetWorldMatrix());
		}
	}

	/// <summary>
	/// モデルの座標を設定する。
	/// </summary>
	/// <param name="pos">座標(Vector3)</param>
	void CSkinModelRender::SetPosition(Vector3 pos)
	{
		m_position = pos;

		UpdateModel();
	}

	/// <summary>
	/// モデルの回転を設定する。
	/// </summary>
	/// <param name="qRot">回転(Quaternion)</param>
	void CSkinModelRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;

		UpdateModel();
	}

	/// <summary>
	/// モデルの拡大率を設定する。
	/// </summary>
	/// <param name="scale">拡大率(Vector3)</param>
	void CSkinModelRender::SetScale(Vector3 scale)
	{
		m_scale = scale;

		UpdateModel();
	}

	/// <summary>
	/// モデルと線分が交差している?
	/// </summary>
	/// <param name="start">線分の始点</param>
	/// <param name="end">線分の終点</param>
	/// <param name="crossPoint">線分の交差点(戻り値、参照受け取り)</param>
	/// <returns>交差しているか(している:true していない:false)</returns>
	bool CSkinModelRender::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint)
	{
		//2画面用にCSkinModelRenderはModelクラスを2つ保持しているが、どちらもカメラ以外同じなので0番目のモデルで判定する。
		return m_model[0].isLineHitModel(start, end, crossPoint);
	}

	/// <summary>
	/// ボーンの名前からそのボーンのワールド行列を取得する。
	/// </summary>
	/// <param name="boneName">ボーンの名前</param>
	/// <returns></returns>
	Matrix CSkinModelRender::GetWorldMatrixFromBoneName(const wchar_t* boneName)
	{
		int boneNo = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneNo)->GetWorldMatrix();
	}
}