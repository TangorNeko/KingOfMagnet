#include "stdafx.h"
#include "CSkinModelRender.h"

namespace prefab
{
	/// <summary>
	/// ���f���̏������֐��B
	/// </summary>
	/// <param name="modelPath">���f���t�@�C���̃p�X(.tkm)</param>
	/// <param name="skeletonPath">�X�P���g���t�@�C���̃p�X(.tks)</param>
	/// <param name="buffer">���C�g�̒萔�o�b�t�@</param>
	void CSkinModelRender::Init(const char* modelPath, const char* skeletonPath)
	{
		m_skeleton.Init(skeletonPath);

		ModelInitData initData;

		initData.m_tkmFilePath = modelPath;

		initData.m_fxFilePath = "Assets/shader/model.fx";

		initData.m_vsEntryPointFunc = "VSMain";

		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		initData.m_skeleton = &m_skeleton;

		//TODO:�����ɂ��ė��p�҂ɓ��͂�����K�v�����肻���B
		initData.m_modelUpAxis = enModelUpAxisZ;

	
		//�萔�o�b�t�@�����f���ɕR�t����
		initData.m_expandConstantBufferSize = CLightManager::GetInstance()->GetDataSize();
		initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLigDatas();

		m_model.Init(initData);
	}

	//���f���p�X�����ŁB�R�s�[���Ă�������
	void CSkinModelRender::Init(const char* modelPath)
	{
		ModelInitData initData;

		initData.m_tkmFilePath = modelPath;

		initData.m_fxFilePath = "Assets/shader/model.fx";

		//initData.m_vsEntryPointFunc = "VSMain";

		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//TODO:�����ɂ��ė��p�҂ɓ��͂�����K�v�����肻���B
		initData.m_modelUpAxis = enModelUpAxisZ;

		//�萔�o�b�t�@�����f���ɕR�t����
		initData.m_expandConstantBufferSize = CLightManager::GetInstance()->GetDataSize();
		initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLigDatas();

		m_model.Init(initData);
	}

	//TODO:���f���̕`����ۂ��AUpdate�̂悤�ɖ��t���[���Ă΂�Ă���悤�����ڍׂ��킩��Ȃ�
	void CSkinModelRender::Render(RenderContext& rc,Camera* camera)
	{
		m_model.Draw(rc,camera);
	}

	/// <summary>
	/// ���f���̏����X�V����B
	/// </summary>
	void CSkinModelRender::UpdateModel()
	{
		m_model.UpdateWorldMatrix(m_position, m_qRot, m_scale);
		m_skeleton.Update(m_model.GetWorldMatrix());
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
}