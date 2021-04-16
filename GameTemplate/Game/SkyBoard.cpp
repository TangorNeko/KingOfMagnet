#include "stdafx.h"
#include "SkyBoard.h"

bool SkyBoard::Start()
{
	ModelInitData skyInitData;
	skyInitData.m_tkmFilePath = "Assets/modelData/SkyBoard.tkm";

	//ここでテクスチャのサイズを指定。
	m_skyData.textureSize = 2824.0f;

	skyInitData.m_fxFilePath = "Assets/shader/SkyBoard.fx";
	skyInitData.m_vsEntryPointFunc = "VSMain";
	skyInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
	skyInitData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	skyInitData.m_modelUpAxis = enModelUpAxisZ;
	skyInitData.m_expandConstantBuffer = &m_skyData;
	skyInitData.m_expandConstantBufferSize = sizeof(m_skyData);

	m_skyModel[0].Init(skyInitData);
	m_skyModel[1].Init(skyInitData);

	m_skyModel[0].UpdateWorldMatrix({ 0.0f,10000.0f,0.0f }, Quaternion::Identity,Vector3::One);
	m_skyModel[1].UpdateWorldMatrix({ 0.0f,10000.0f,0.0f }, Quaternion::Identity, Vector3::One);


	return true;
}

void SkyBoard::Update()
{
	//テクスチャを0.25ずつスクロール。
	m_skyData.scrollingValue += 0.1f;

	//空のテクスチャのサイズがを超えたら0にするようにしている。
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
		//1P画面用。
		m_skyModel[0].Draw(rc, camera);
		break;
	case RenderContext::eStep_RenderViewport2:
		//2P画面用。
		m_skyModel[1].Draw(rc, camera);
		break;
	default:
		break;
	}
}