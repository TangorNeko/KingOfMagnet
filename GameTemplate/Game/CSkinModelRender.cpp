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
		m_skeleton.Init(skeletonPath);

		ModelInitData initData;

		//TODO:NewGOするたびにロードが呼ばれないようにしたい。
		//initData.m_tkmFile = tkmFileManager::GetInstance()->GetTkmFile(modelPath);

		initData.m_tkmFilePath = modelPath;

		initData.m_fxFilePath = "Assets/shader/model.fx";

		initData.m_vsEntryPointFunc = "VSMain";

		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		initData.m_skeleton = &m_skeleton;

		//TODO:引数にして利用者に入力させる必要がありそう。
		initData.m_modelUpAxis = enModelUpAxisZ;


		//定数バッファをモデルに紐付ける
		initData.m_expandConstantBufferSize = CLightManager::GetInstance()->GetDataSize();
		initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLigDatas();

		for (auto& model : m_model) {
			model.Init(initData);
		}

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
		m_skeleton.Init(skeletonPath);

		ModelInitData initData;

		//initData.m_tkmFile = tkmFileManager::GetInstance()->GetTkmFile(modelPath);

		initData.m_tkmFilePath = modelPath;

		initData.m_fxFilePath = "Assets/shader/model.fx";

		initData.m_vsEntryPointFunc = "VSMain";

		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		initData.m_skeleton = &m_skeleton;

		//TODO:引数にして利用者に入力させる必要がありそう。
		initData.m_modelUpAxis = enModelUpAxisZ;

	
		//定数バッファをモデルに紐付ける
		initData.m_expandConstantBufferSize = CLightManager::GetInstance()->GetDataSize();
		initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLigDatas();

		for (auto& model : m_model) {
			model.Init(initData);
		}
	}

	/// <summary>
	/// モデルの初期化関数。モデルパスだけ版。コピーしてきただけ
	/// </summary>
	/// <param name="modelPath">モデルファイルのパス(.tkm)</param>
	void CSkinModelRender::Init(const char* modelPath)
	{
		ModelInitData initData;

		//initData.m_tkmFile = tkmFileManager::GetInstance()->GetTkmFile(modelPath);

		initData.m_tkmFilePath = modelPath;

		initData.m_fxFilePath = "Assets/shader/model.fx";

		//initData.m_vsEntryPointFunc = "VSMain";

		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//TODO:引数にして利用者に入力させる必要がありそう。
		initData.m_modelUpAxis = enModelUpAxisZ;

		//定数バッファをモデルに紐付ける
		initData.m_expandConstantBufferSize = CLightManager::GetInstance()->GetDataSize();
		initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLigDatas();

		m_model[eModel_View1].Init(initData);
		m_model[eModel_View2].Init(initData);
	}

	//TODO:モデルの描画っぽい、Updateのように毎フレーム呼ばれているようだが詳細がわからない
	void CSkinModelRender::Render(RenderContext& rc,Camera* camera)
	{
		switch (rc.GetRenderStep()) {
		case RenderContext::eStep_RenderViewport1:
			m_model[eModel_View1].Draw(rc, camera);
			break;
		case RenderContext::eStep_RenderViewport2:
			m_model[eModel_View2].Draw(rc, camera);
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