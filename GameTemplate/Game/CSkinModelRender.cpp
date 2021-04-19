#include "stdafx.h"
#include "CSkinModelRender.h"

namespace prefab
{
	/// <summary>
	/// ���f���̏������֐��B�A�j���[�V�������B
	/// </summary>
	/// <param name="modelPath">���f���t�@�C���̃p�X(.tkm)</param>
	/// <param name="skeletonPath">�X�P���g���t�@�C���̃p�X(.tks)</param>
	/// <param name="animationClips">�A�j���[�V�����N���b�v�̔z��</param>
	/// <param name="animationClipNum">�A�j���[�V�����N���b�v�̐�</param>
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

		//�萔�o�b�t�@�����f���ɕR�t����
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
	/// ���f���̏������֐��B
	/// </summary>
	/// <param name="modelPath">���f���t�@�C���̃p�X(.tkm)</param>
	/// <param name="skeletonPath">�X�P���g���t�@�C���̃p�X(.tks)</param>
	void CSkinModelRender::Init(const char* modelPath, const char* skeletonPath)
	{
		Init(modelPath, skeletonPath, nullptr, 0);
	}

	/// <summary>
	/// ���f���̏������֐��B���f���p�X�����ŁB
	/// </summary>
	/// <param name="modelPath">���f���t�@�C���̃p�X(.tkm)</param>
	void CSkinModelRender::Init(const char* modelPath)
	{
		Init(modelPath, nullptr, nullptr, 0);
	}

	//���f���̕`��B
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
	/// ���f���̏����X�V����B
	/// </summary>
	void CSkinModelRender::UpdateModel()
	{
		for (auto& model : m_model) {
			model.UpdateWorldMatrix(m_position, m_qRot, m_scale);
			m_skeleton.Update(model.GetWorldMatrix());
		}
	}

	/// <summary>
	/// ���f���̍��W��ݒ肷��B
	/// </summary>
	/// <param name="pos">���W(Vector3)</param>
	void CSkinModelRender::SetPosition(Vector3 pos)
	{
		m_position = pos;

		UpdateModel();
	}

	/// <summary>
	/// ���f���̉�]��ݒ肷��B
	/// </summary>
	/// <param name="qRot">��](Quaternion)</param>
	void CSkinModelRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;

		UpdateModel();
	}

	/// <summary>
	/// ���f���̊g�嗦��ݒ肷��B
	/// </summary>
	/// <param name="scale">�g�嗦(Vector3)</param>
	void CSkinModelRender::SetScale(Vector3 scale)
	{
		m_scale = scale;

		UpdateModel();
	}

	/// <summary>
	/// ���f���Ɛ������������Ă���?
	/// </summary>
	/// <param name="start">�����̎n�_</param>
	/// <param name="end">�����̏I�_</param>
	/// <param name="crossPoint">�����̌����_(�߂�l�A�Q�Ǝ󂯎��)</param>
	/// <returns>�������Ă��邩(���Ă���:true ���Ă��Ȃ�:false)</returns>
	bool CSkinModelRender::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint)
	{
		//2��ʗp��CSkinModelRender��Model�N���X��2�ێ����Ă��邪�A�ǂ�����J�����ȊO�����Ȃ̂�0�Ԗڂ̃��f���Ŕ��肷��B
		return m_model[0].isLineHitModel(start, end, crossPoint);
	}

	/// <summary>
	/// �{�[���̖��O���炻�̃{�[���̃��[���h�s����擾����B
	/// </summary>
	/// <param name="boneName">�{�[���̖��O</param>
	/// <returns></returns>
	Matrix CSkinModelRender::GetWorldMatrixFromBoneName(const wchar_t* boneName)
	{
		int boneNo = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneNo)->GetWorldMatrix();
	}
}