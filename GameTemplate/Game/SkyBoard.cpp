#include "stdafx.h"
#include "SkyBoard.h"

bool SkyBoard::Start()
{
	ModelInitData skyInitData;
	skyInitData.m_tkmFilePath = "Assets/modelData/SkyBoard.tkm";

	//�����Ńe�N�X�`���̃T�C�Y���w��B
	m_skyData.textureSize = 2824.0f;

	skyInitData.m_fxFilePath = "Assets/shader/SkyBoard.fx";
	skyInitData.m_vsEntryPointFunc = "VSMain";
	skyInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
	skyInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	skyInitData.m_modelUpAxis = enModelUpAxisZ;
	skyInitData.m_expandConstantBuffer[0] = &m_skyData;
	skyInitData.m_expandConstantBufferSize[0] = sizeof(m_skyData);

	m_skyModel[0].Init(skyInitData);
	m_skyModel[1].Init(skyInitData);

	m_skyModel[0].UpdateWorldMatrix({ 0.0f,10000.0f,0.0f }, Quaternion::Identity,Vector3::One);
	m_skyModel[1].UpdateWorldMatrix({ 0.0f,10000.0f,0.0f }, Quaternion::Identity, Vector3::One);


	return true;
}

void SkyBoard::Update()
{
	//�e�N�X�`����0.25���X�N���[���B
	m_skyData.scrollingValue += 0.1f;

	//��̃e�N�X�`���̃T�C�Y���𒴂�����0�ɂ���悤�ɂ��Ă���B
	if (m_skyData.scrollingValue >= m_skyData.textureSize)
	{
		m_skyData.scrollingValue = 0;
	}
}

void SkyBoard::Render(RenderContext& rc, Camera* camera)
{
	switch (rc.GetRenderStep())
	{
	case RenderContext::eStep_RenderViewport1:
		//1P��ʗp�B
		m_skyModel[0].Draw(rc, camera);
		break;
	case RenderContext::eStep_RenderViewport2:
		//2P��ʗp�B
		m_skyModel[1].Draw(rc, camera);
		break;
	default:
		break;
	}
}