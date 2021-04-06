#include "stdafx.h"
#include "PostEffectManager.h"
//#include "GaussianBlur.h"

PostEffectManager* PostEffectManager::m_instance = nullptr;

void PostEffectManager::Init(bool bloomMode)
{
	m_bloomMode = bloomMode;

	if (m_bloomMode == true)
	{
		m_mainRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		m_luminnceRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		SpriteInitData luminanceSpriteInitData;
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		luminanceSpriteInitData.m_width = 1280;
		luminanceSpriteInitData.m_height = 720;
		luminanceSpriteInitData.m_textures[0] =	&m_mainRenderTarget.GetRenderTargetTexture();

		m_luminanceSprite.Init(luminanceSpriteInitData);
		

		m_gaussianBlur.Init(&m_luminnceRenderTarget.GetRenderTargetTexture());


		SpriteInitData finalSpriteInitData;
		finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		finalSpriteInitData.m_vsEntryPointFunc = "VSMain";
		finalSpriteInitData.m_width = 1280;
		finalSpriteInitData.m_height = 720;
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		finalSpriteInitData.m_textures[0] = &m_gaussianBlur.GetBokeTexture();

		m_finalSprite.Init(finalSpriteInitData);

		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		spriteInitData.m_width = 1280;
		spriteInitData.m_height = 720;

		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		m_testSprite.Init(spriteInitData);
	}
}

void PostEffectManager::BeforeRender(RenderContext& rc)
{
	if (m_bloomMode == true)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVCpuDescriptorHandle());
		rc.ClearRenderTargetView(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetRTVClearColor());
		rc.ClearDepthStencilView(m_mainRenderTarget.GetDSVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVClearValue());
	}
}

void PostEffectManager::AfterRender(RenderContext& rc)
{
	if (m_bloomMode == true)
	{
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		// step-10 輝度抽出
		//輝度抽出用のレンダリングターゲットに変更
		rc.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTarget(m_luminnceRenderTarget.GetRTVCpuDescriptorHandle(), m_luminnceRenderTarget.GetDSVCpuDescriptorHandle());
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_luminnceRenderTarget.GetRTVCpuDescriptorHandle(), m_luminnceRenderTarget.GetRTVClearColor());
		//輝度抽出を行う
		m_luminanceSprite.Draw(rc);
		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);
		// step-11 ガウシアンブラーを実行する
		m_gaussianBlur.ExecuteOnGPU(rc, 20);

		// step-12 ボケ画像をメインレンダリングターゲットに加算合成
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVCpuDescriptorHandle());
		//最終合成
		m_finalSprite.Draw(rc);
		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		rc.SetRenderTarget(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), g_graphicsEngine->GetCurrentFrameBuffuerDSV());

		m_testSprite.Draw(rc);
	}
}