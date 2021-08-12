#include "stdafx.h"
#include "SkyBoard.h"

namespace
{
	const float SKY_TEXTURE_SIZE = 2824.0f;					//空のテクスチャサイズ
	const Vector3 POSITION_SKY = { 0.0f,10000.0f,0.0f };	//空の板ポリの座標
	const float SKY_SCROLL_VALUE = 0.1f;					//空のテクスチャのUVスクロール量
}

bool SkyBoard::Start()
{
	ModelInitData skyInitData;
	skyInitData.m_tkmFilePath = "Assets/modelData/SkyBoard.tkm";

	//ここでテクスチャのサイズを指定。
	m_skyData.textureSize = SKY_TEXTURE_SIZE;

	//空の初期化データを作成
	skyInitData.m_fxFilePath = "Assets/shader/SkyBoard.fx";
	skyInitData.m_vsEntryPointFunc = "VSMain";
	skyInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
	skyInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	skyInitData.m_modelUpAxis = enModelUpAxisZ;
	skyInitData.m_expandConstantBuffer[0] = &m_skyData;
	skyInitData.m_expandConstantBufferSize[0] = sizeof(m_skyData);

	//空のモデルを2画面分初期化
	for (auto& skymodel : m_skyModel)
	{
		skymodel.Init(skyInitData);
		skymodel.UpdateWorldMatrix(POSITION_SKY, Quaternion::Identity, Vector3::One);
	}

	return true;
}

void SkyBoard::Update()
{
	//テクスチャをスクロールさせる
	m_skyData.scrollingValue += SKY_SCROLL_VALUE;

	//スクロール量が空のテクスチャのサイズを超えたら一周したので0にするようにしている。
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
			//1P画面用。
			m_skyModel[NUMBER_PLAYER1].Draw(rc, camera);
			break;
		case RenderContext::eStep_RenderViewport2:
			//2P画面用。
			m_skyModel[NUMBER_PLAYER2].Draw(rc, camera);
			break;
		default:
			break;
		}
	}
}