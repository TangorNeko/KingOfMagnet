#include "stdafx.h"
#include "CSkinModelRender.h"

namespace prefab
{
	void CSkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum)
	{
		//���f���̏������f�[�^
		ModelInitData initData;

		//�e�̏������f�[�^
		ModelInitData shadowModelInitData;

		//���f���̃t�@�C���p�X�̎w��
		initData.m_tkmFilePath = modelPath;
		shadowModelInitData.m_tkmFilePath = modelPath;

		//�V�F�[�_�[�p�X�̎w��
		initData.m_fxFilePath = "Assets/shader/deferredModel.fx";
		shadowModelInitData.m_fxFilePath = "Assets/shader/shadow.fx";

		//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		initData.m_vsEntryPointFunc = "VSMain";
		shadowModelInitData.m_vsEntryPointFunc = "VSMain";

		//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";
		shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//�X�P���g�������݂��Ă��鎞�̓X�P���g����������
		if (skeletonPath != nullptr)
		{
			m_skeleton.Init(skeletonPath);
			initData.m_skeleton = &m_skeleton;
			shadowModelInitData.m_skeleton = &m_skeleton;
		}

		//�J���[�o�b�t�@�̃t�H�[�}�b�g���w��
		initData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		shadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32_FLOAT;

		//���f���f�[�^�̏�����̎����w��
		initData.m_modelUpAxis = enModelUpAxisZ;
		shadowModelInitData.m_modelUpAxis = enModelUpAxisZ;

		//���f���ɉe�𗎂Ƃ����߂ɉe�̃e�N�X�`����R�t����
		initData.m_expandShaderResoruceView = &PostEffectManager::GetInstance()->GetBlurShadowMap();

		//�萔�o�b�t�@�����f���ɕR�t����
		initData.m_expandConstantBufferSize[0] = CLightManager::GetInstance()->GetLigDataSize();
		shadowModelInitData.m_expandConstantBufferSize[0] = CLightManager::GetInstance()->GetLigCameraDataSize();
		initData.m_expandConstantBuffer[0] = CLightManager::GetInstance()->GetLigDatas();
		shadowModelInitData.m_expandConstantBuffer[0] = CLightManager::GetInstance()->GetLigCameraDatas();

		initData.m_expandConstantBufferSize[1] = CLightManager::GetInstance()->GetLigCameraDataSize();
		initData.m_expandConstantBuffer[1] = CLightManager::GetInstance()->GetLigCameraDatas();

		//���f���̏�����
		m_model[eModel_View1].Init(initData);
		m_model[eModel_View2].Init(initData);

		//�e�`�惂�f���̏�����
		m_model[eModel_Shadow].Init(shadowModelInitData);

		//�A�j���[�V�����֘A�̏�����
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
		//�����_�[�R���e�L�X�g�̕`���ŕ���
		if (rc.GetRenderMode() == RenderContext::enRenderMode_Deferred)
		{
			switch (rc.GetRenderStep()) {
			case RenderContext::eStep_RenderViewport1:
				//���1�ɕ`��
				m_model[eModel_View1].Draw(rc, camera);
				break;
			case RenderContext::eStep_RenderViewport2:
				//���2�ɕ`��
				m_model[eModel_View2].Draw(rc, camera);
				break;
			case RenderContext::eStep_RenderShadowMap:
				//�e����郂�f���̎������e��`��
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
		//���f�����A�b�v�f�[�g
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
		//2��ʗp��CSkinModelRender��Model�N���X��2�ێ����Ă��邪�A�ǂ�����J�����ȊO�����Ȃ̂�0�Ԗڂ̃��f���Ŕ��肷��B
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
			//���o�^
			tkmFile = new TkmFile;
			tkmFile->Load(tkmFilePath);
			ResourceBankManager::GetInstance()->RegistTkmFileToBank(tkmFilePath, tkmFile);
		}
	}
}