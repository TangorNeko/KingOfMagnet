#include "stdafx.h"
#include "PostEffectManager.h"
//#include "GaussianBlur.h"

PostEffectManager* PostEffectManager::m_instance = nullptr;

void PostEffectManager::Init(bool bloomMode)
{
	m_bloomMode = bloomMode;

	if (m_bloomMode == true)
	{
		//最終的に画面に出力する用のレンダーターゲット
		m_mainRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//輝度を抽出したものが格納されるレンダーターゲット
		m_luminnceRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//ピクセルシェーダーPSSamplingLuminanceで、メインレンダーターゲットから輝度を抽出する用のスプライト。
		SpriteInitData luminanceSpriteInitData;
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		luminanceSpriteInitData.m_width = 1280;
		luminanceSpriteInitData.m_height = 720;
		luminanceSpriteInitData.m_textures[0] =	&m_mainRenderTarget.GetRenderTargetTexture();

		m_luminanceSprite.Init(luminanceSpriteInitData);
		
		//輝度抽出したスプライトをもとにブラーをかけさせるようにセット。
		m_gaussianBlur.Init(&m_luminnceRenderTarget.GetRenderTargetTexture());

		//輝度抽出されたものにブラーをかけたスプライト。
		SpriteInitData bokeLuminanceSpriteInitData;
		bokeLuminanceSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		bokeLuminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		bokeLuminanceSpriteInitData.m_width = 1280;
		bokeLuminanceSpriteInitData.m_height = 720;
		bokeLuminanceSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		bokeLuminanceSpriteInitData.m_textures[0] = &m_gaussianBlur.GetBokeTexture();

		m_bokeLuminanceSprite.Init(bokeLuminanceSpriteInitData);

		//最終的にmainRenderTargetには元の画面にブルームをかけたものが出力されるので、それを画面に出力する用のスプライト。
		SpriteInitData copyToBufferSpriteInitData;
		copyToBufferSpriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		copyToBufferSpriteInitData.m_width = 1280;
		copyToBufferSpriteInitData.m_height = 720;
		copyToBufferSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyToFrameBufferSprite.Init(copyToBufferSpriteInitData);
	}
}

void PostEffectManager::BeforeRender(RenderContext& rc)
{
	if (m_bloomMode == true)
	{
		//メインレンダーターゲットを描画先にセットする。
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVCpuDescriptorHandle());
		rc.ClearRenderTargetView(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetRTVClearColor());
		rc.ClearDepthStencilView(m_mainRenderTarget.GetDSVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVClearValue());
		//この後、各モデルのドローコールが呼ばれる(はず)。
	}
}

void PostEffectManager::AfterRender(RenderContext& rc)
{
	if (m_bloomMode == true)
	{
		//メインレンダーターゲットが描画を終えるまで待つ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		//輝度抽出用のレンダーターゲットに変更する。
		rc.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);
		rc.SetRenderTarget(m_luminnceRenderTarget.GetRTVCpuDescriptorHandle(), m_luminnceRenderTarget.GetDSVCpuDescriptorHandle());
		rc.ClearRenderTargetView(m_luminnceRenderTarget.GetRTVCpuDescriptorHandle(), m_luminnceRenderTarget.GetRTVClearColor());
		//輝度抽出を行う
		m_luminanceSprite.Draw(rc);
		//輝度抽出用のレンダーターゲットが描画を終えるまで待つ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

		//ガウシアンブラーを実行する
		m_gaussianBlur.ExecuteOnGPU(rc, 20);

		//メインレンダーターゲットが使えるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVCpuDescriptorHandle());
		//輝度抽出したものをボカした物をメインレンダーターゲットに加算合成
		//この時点でメインレンダーターゲットには元の画像とブラーの画像が合わさったものになる。
		m_bokeLuminanceSprite.Draw(rc);
		//メインレンダーターゲットが描画を終えるまで待つ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		//描画先をフレームバッファにする。
		rc.SetRenderTarget(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), g_graphicsEngine->GetCurrentFrameBuffuerDSV());

		//メインレンダーターゲットに描かれていた最終イメージを画面上に描く。
		m_copyToFrameBufferSprite.Draw(rc);
	}
}