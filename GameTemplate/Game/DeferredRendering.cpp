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
		DXGI_FORMAT_R32G32B32A32_FLOAT,
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

	//ディファードレンダリングの出力スプライトを初期化
	SpriteInitData deferredSpriteInitData;
	deferredSpriteInitData.m_fxFilePath = "Assets/shader/DeferredSprite.fx";
	deferredSpriteInitData.m_height = FRAME_BUFFER_H;
	deferredSpriteInitData.m_width = FRAME_BUFFER_W;
	deferredSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	deferredSpriteInitData.m_textures[enAlbedo] = &m_rts[enAlbedo].GetRenderTargetTexture();
	deferredSpriteInitData.m_textures[enNormal] = &m_rts[enNormal].GetRenderTargetTexture();
	deferredSpriteInitData.m_textures[enWorldPos] = &m_rts[enWorldPos].GetRenderTargetTexture();
	deferredSpriteInitData.m_textures[enShadowmap_Screen1_Near] = &CascadeShadow::GetInstance()->GetShaowMapTexture(0, 0);
	deferredSpriteInitData.m_textures[enShadowmap_Screen1_Middle] = &CascadeShadow::GetInstance()->GetShaowMapTexture(0, 1);
	deferredSpriteInitData.m_textures[enShadowmap_Screen1_Far] = &CascadeShadow::GetInstance()->GetShaowMapTexture(0, 2);
	deferredSpriteInitData.m_textures[enShadowmap_Screen2_Near] = &CascadeShadow::GetInstance()->GetShaowMapTexture(1, 0);
	deferredSpriteInitData.m_textures[enShadowmap_Screen2_Middle] = &CascadeShadow::GetInstance()->GetShaowMapTexture(1, 1);
	deferredSpriteInitData.m_textures[enShadowmap_Screen2_Far] = &CascadeShadow::GetInstance()->GetShaowMapTexture(1, 2);
	deferredSpriteInitData.m_expandConstantBuffer[0] = CLightManager::GetInstance()->GetLigDatas();
	deferredSpriteInitData.m_expandConstantBufferSize[0] = CLightManager::GetInstance()->GetLigDataSize();
	deferredSpriteInitData.m_expandConstantBuffer[1] = CascadeShadow::GetInstance()->GetLVPCMatrix(0);
	deferredSpriteInitData.m_expandConstantBufferSize[1] = sizeof(Matrix) * 6;
	m_deferredSprite.Init(deferredSpriteInitData);
}

void DeferredRendering::StartDeferredRendering(RenderContext& rc)
{
	RenderTarget* rts[enGBufferNum] = {
		&m_rts[enAlbedo],
		&m_rts[enNormal],
		&m_rts[enWorldPos],
	};

	//シザリング矩形をセット
	D3D12_RECT shadowRect;
	shadowRect.left = 0;
	shadowRect.top = 0;
	shadowRect.right = m_rts[enAlbedo].GetWidth();
	shadowRect.bottom = m_rts[enAlbedo].GetHeight();
	rc.SetScissorRect(shadowRect);

	//レンダーターゲットをセットできるまで待機
	rc.WaitUntilToPossibleSetRenderTargets(enGBufferNum, rts);

	//レンダーターゲットをセット
	rc.SetRenderTargets(enGBufferNum, rts);
	
	//レンダーターゲットをクリア
	rc.ClearRenderTargetViews(enGBufferNum, rts);
}

void DeferredRendering::EndDeferredRendering(RenderContext& rc)
{
	RenderTarget* rts[enGBufferNum] = {
		&m_rts[enAlbedo],
		&m_rts[enNormal],
		&m_rts[enWorldPos],
	};

	//書き込み終了まで待機
	rc.WaitUntilFinishDrawingToRenderTargets(enGBufferNum, rts);
}