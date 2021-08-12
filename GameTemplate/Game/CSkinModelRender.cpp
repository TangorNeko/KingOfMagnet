#include "stdafx.h"
#include "CSkinModelRender.h"

namespace prefab
{
	void CSkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum)
	{
		//モデルの初期化データ
		ModelInitData initData;

		//影の初期化データ
		ModelInitData shadowModelInitData;

		//モデルのファイルパスの指定
		initData.m_tkmFilePath = modelPath;
		shadowModelInitData.m_tkmFilePath = modelPath;

		//シェーダーパスの指定
		initData.m_fxFilePath = "Assets/shader/deferredModel.fx";
		shadowModelInitData.m_fxFilePath = "Assets/shader/shadow.fx";

		//シェーダーの頂点シェーダーのエントリー関数名の指定
		initData.m_vsEntryPointFunc = "VSMain";
		shadowModelInitData.m_vsEntryPointFunc = "VSMain";

		//シェーダーのピクセルシェーダーのエントリー関数名の指定
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";
		shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//スケルトンが存在している時はスケルトンを初期化
		if (skeletonPath != nullptr)
		{
			m_skeleton.Init(skeletonPath);
			initData.m_skeleton = &m_skeleton;
			shadowModelInitData.m_skeleton = &m_skeleton;
		}

		//カラーバッファのフォーマットを指定
		initData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		shadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32_FLOAT;

		//モデルデータの上方向の軸を指定
		initData.m_modelUpAxis = enModelUpAxisZ;
		shadowModelInitData.m_modelUpAxis = enModelUpAxisZ;

		//モデルに影を落とすために影のテクスチャを紐付ける
		initData.m_expandShaderResoruceView = &PostEffectManager::GetInstance()->GetBlurShadowMap();

		//定数バッファをモデルに紐付ける
		initData.m_expandConstantBufferSize[0] = CLightManager::GetInstance()->GetLigDataSize();
		shadowModelInitData.m_expandConstantBufferSize[0] = CLightManager::GetInstance()->GetLigCameraDataSize();
		initData.m_expandConstantBuffer[0] = CLightManager::GetInstance()->GetLigDatas();
		shadowModelInitData.m_expandConstantBuffer[0] = CLightManager::GetInstance()->GetLigCameraDatas();

		initData.m_expandConstantBufferSize[1] = CLightManager::GetInstance()->GetLigCameraDataSize();
		initData.m_expandConstantBuffer[1] = CLightManager::GetInstance()->GetLigCameraDatas();

		//モデルの初期化
		m_model[eModel_View1].Init(initData);
		m_model[eModel_View2].Init(initData);

		//影描画モデルの初期化
		m_model[eModel_Shadow].Init(shadowModelInitData);

		//アニメーション関連の初期化
		m_animationClips = animClips;
		m_animationClipNum = animClipNum;

		if (m_animationClips != nullptr)
		{
			m_animation.Init(m_skeleton, m_animationClips, m_animationClipNum);
		}
	}

	void CSkinModelRender::Init(const char* modelPath, const char* skeletonPath)
	{
		Init(modelPath, skeletonPath, nullptr, 0);
	}

	void CSkinModelRender::Init(const char* modelPath)
	{
		Init(modelPath, nullptr, nullptr, 0);
	}

	void CSkinModelRender::Render(RenderContext& rc, Camera* camera)
	{
		//レンダーコンテキストの描画先で分岐
		if (rc.GetRenderMode() == RenderContext::enRenderMode_Deferred)
		{
			switch (rc.GetRenderStep()) {
			case RenderContext::eStep_RenderViewport1:
				//画面1に描画
				m_model[eModel_View1].Draw(rc, camera);
				break;
			case RenderContext::eStep_RenderViewport2:
				//画面2に描画
				m_model[eModel_View2].Draw(rc, camera);
				break;
			case RenderContext::eStep_RenderShadowMap:
				//影を作るモデルの時だけ影を描画
				if (m_isShadowCaster)
				{
					m_model[eModel_Shadow].Draw(rc, camera);
				}
				break;
			}
		}
	}

	void CSkinModelRender::UpdateModel()
	{
		//モデルをアップデート
		for (auto& model : m_model) {
			model.UpdateWorldMatrix(m_position, m_qRot, m_scale);
			m_skeleton.Update(model.GetWorldMatrix());
		}
	}

	void CSkinModelRender::SetPosition(Vector3 pos)
	{
		m_position = pos;

		UpdateModel();
	}

	void CSkinModelRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;

		UpdateModel();
	}

	void CSkinModelRender::SetScale(Vector3 scale)
	{
		m_scale = scale;

		UpdateModel();
	}

	bool CSkinModelRender::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint)
	{
		//2画面用にCSkinModelRenderはModelクラスを2つ保持しているが、どちらもカメラ以外同じなので0番目のモデルで判定する。
		return m_model[0].isLineHitModel(start, end, crossPoint);
	}

	Matrix CSkinModelRender::GetWorldMatrixFromBoneName(const wchar_t* boneName)
	{
		int boneNo = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneNo)->GetWorldMatrix();
	}

	void CSkinModelRender::PreLoadModel(const char* tkmFilePath)
	{
		TkmFile* tkmFile = ResourceBankManager::GetInstance()->GetTkmFileFromBank(tkmFilePath);

		if (tkmFile == nullptr)
		{
			//未登録
			tkmFile = new TkmFile;
			tkmFile->Load(tkmFilePath);
			ResourceBankManager::GetInstance()->RegistTkmFileToBank(tkmFilePath, tkmFile);
		}
	}
}