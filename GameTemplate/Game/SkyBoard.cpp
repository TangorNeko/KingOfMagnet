#include "stdafx.h"
#include "SkyBoard.h"

namespace
{
	const float SKY_TEXTURE_SIZE = 2824.0f;					//��̃e�N�X�`���T�C�Y
	const Vector3 POSITION_SKY = { 0.0f,10000.0f,0.0f };	//��̔|���̍��W
	const float SKY_SCROLL_VALUE = 0.1f;					//��̃e�N�X�`����UV�X�N���[����
}

bool SkyBoard::Start()
{
	ModelInitData skyInitData;
	skyInitData.m_tkmFilePath = "Assets/modelData/SkyBoard.tkm";

	//�����Ńe�N�X�`���̃T�C�Y���w��B
	m_skyData.textureSize = SKY_TEXTURE_SIZE;

	//��̏������f�[�^���쐬
	skyInitData.m_fxFilePath = "Assets/shader/SkyBoard.fx";
	skyInitData.m_vsEntryPointFunc = "VSMain";
	skyInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
	skyInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	skyInitData.m_modelUpAxis = enModelUpAxisZ;
	skyInitData.m_expandConstantBuffer[0] = &m_skyData;
	skyInitData.m_expandConstantBufferSize[0] = sizeof(m_skyData);

	//��̃��f����2��ʕ�������
	for (auto& skymodel : m_skyModel)
	{
		skymodel.Init(skyInitData);
		skymodel.UpdateWorldMatrix(POSITION_SKY, Quaternion::Identity, Vector3::One);
	}

	return true;
}

void SkyBoard::Update()
{
	//�e�N�X�`�����X�N���[��������
	m_skyData.scrollingValue += SKY_SCROLL_VALUE;

	//�X�N���[���ʂ���̃e�N�X�`���̃T�C�Y�𒴂������������̂�0�ɂ���悤�ɂ��Ă���B
	if (m_skyData.scrollingValue >= m_skyData.textureSize)
	{
		m_skyData.scrollingValue = 0;
	}
}

void SkyBoard::Render(RenderContext& rc, Camera* camera)
{
	if (rc.GetRenderMode() == RenderContext::enRenderMode_Forward)
	{
		switch (rc.GetRenderStep())
		{
		case RenderContext::eStep_RenderViewport1:
			//1P��ʗp�B
			m_skyModel[NUMBER_PLAYER1].Draw(rc, camera);
			break;
		case RenderContext::eStep_RenderViewport2:
			//2P��ʗp�B
			m_skyModel[NUMBER_PLAYER2].Draw(rc, camera);
			break;
		default:
			break;
		}
	}
}