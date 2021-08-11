#include "stdafx.h"
#include "DeferredRendering.h"

DeferredRendering* DeferredRendering::m_instance = nullptr;

void DeferredRendering::Init()
{
	//G-Bufferのレンダーターゲットを作成

	//アルベドレンダーターゲット
	m_rts[enAlbedo].Create(
		static_cast<int>(FRAME_BUFFER_W),
		static_cast<int>(FRAME_BUFFER_H),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	//法線レンダーターゲット
	m_rts[enNormal].Create(
		static_cast<int>(FRAME_BUFFER_W),
		static_cast<int>(FRAME_BUFFER_H),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN
	);

	//ワールド座標レンダーターゲット
	m_rts[enWorldPos].Create(
		static_cast<int>(FRAME_BUFFER_W),
		static_cast<int>(FRAME_BUFFER_H),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);

	//シャドウカラーレンダーターゲット
	m_rts[enShadowColor].Create(
		static_cast<int>(FRAME_BUFFER_W),
		static_cast<int>(FRAME_BUFFER_H),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);


	//ディファードレンダリングの出力スプライトを初期化
	SpriteInitData deferredSpriteInitData;
	deferredSpriteInitData.m_fxFilePath = "Assets/shader/DeferredSprite.fx";
	deferredSpriteInitData.m_height = FRAME_BUFFER_H;
	deferredSpriteInitData.m_width = FRAME_BUFFER_W;
	deferredSpriteInitData.m_textures[enAlbedo] = &m_rts[enAlbedo].GetRenderTargetTexture();
	deferredSpriteInitData.m_textures[enNormal] = &m_rts[enNormal].GetRenderTargetTexture();
	deferredSpriteInitData.m_textures[enWorldPos] = &m_rts[enWorldPos].GetRenderTargetTexture();
	deferredSpriteInitData.m_textures[enShadowColor] = &m_rts[enShadowColor].GetRenderTargetTexture();
	deferredSpriteInitData.m_expandConstantBuffer[0] = CLightManager::GetInstance()->GetLigDatas();
	deferredSpriteInitData.m_expandConstantBufferSize[0] = CLightManager::GetInstance()->GetLigDataSize();
	m_deferredSprite.Init(deferredSpriteInitData);
}

void DeferredRendering::StartDeferredRendering(RenderContext& rc)
{
	RenderTarget* rts[enGBufferNum] = {
		&m_rts[enAlbedo],
		&m_rts[enNormal],
		&m_rts[enWorldPos],
		&m_rts[enShadowColor]
	};
	rc.WaitUntilToPossibleSetRenderTargets(enGBufferNum, rts);
	rc.SetRenderTargets(enGBufferNum, rts);
	rc.ClearRenderTargetViews(enGBufferNum, rts);
}

void DeferredRendering::EndDeferredRendering(RenderContext& rc)
{
	RenderTarget* rts[enGBufferNum] = {
		&m_rts[enAlbedo],
		&m_rts[enNormal],
		&m_rts[enWorldPos],
		&m_rts[enShadowColor]
	};
	rc.WaitUntilFinishDrawingToRenderTargets(enGBufferNum, rts);

	m_deferredSprite.Draw(rc);
}