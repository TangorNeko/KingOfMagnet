#include "stdafx.h"
#include "CSpriteRender.h"

namespace prefab
{
	void CSpriteRender::Init(const char* spritePath,UINT width,UINT height)
	{
		//スプライトの初期化データ
		SpriteInitData initData;

		//スプライトのテクスチャパス
		initData.m_ddsFilePath[0] = spritePath;

		//スプライトの頂点シェーダーのエントリー関数名の指定
		initData.m_vsEntryPointFunc = "VSMain";

		//スプライトのモードに応じて使用するピクセルシェーダーのエントリー関数を変更
		switch (m_spriteMode)
		{
		case prefab::CSpriteRender::Normal:
			initData.m_psEntryPoinFunc = "PSMain";
			break;
		case prefab::CSpriteRender::Transition:
			initData.m_psEntryPoinFunc = "PS_Transition";
			m_sprite.SetAlpha(0.0f);
			break;
		}

		//シェーダーファイルパスの指定
		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		//スプライトの幅を指定
		initData.m_width = width;

		//スプライトの高さを指定
		initData.m_height = height;

		//スプライトのアルファブレンドモードを透過に指定
		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		//スプライトのカラーバッファのフォーマットを指定
		initData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		//スプライトの初期化
		m_sprite.Init(initData);

		if (m_useSpriteSupporterFlag == true)
		{
			//スプライトサポーターに自分の存在を伝える
			m_spriteSupporter.SpriteRenderSetting(this);
		}
	}

	void CSpriteRender::Update()
	{
		if (m_useSpriteSupporterFlag == true)
		{
			//スプライトサポーターの更新
			m_spriteSupporter.SpriteSupporter_Update();
		}
	}

	void CSpriteRender::Render(RenderContext& rc, Camera* camera)
	{
		//描画画面が一致し、モデルと同タイミングで描画する設定なら描画
		if (rc.GetRenderStep() == m_drawScreen && m_isPostRender == false)
		{
			m_sprite.Draw(rc);
		}
	}

	void CSpriteRender::PostRender(RenderContext& rc, Camera* camera)
	{
		//描画画面が一致し、モデルが描画された後で描画する設定なら描画
		if (rc.GetRenderStep() == m_drawScreen && m_isPostRender == true)
		{
			m_sprite.Draw(rc);
		}
	}

	void CSpriteRender::SetPosition(Vector3 pos)
	{
		m_position = pos;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void CSpriteRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void CSpriteRender::SetScale(Vector3 scale)
	{
		m_scale = scale;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void CSpriteRender::SetPivot(Vector2 pivot)
	{
		m_pivot = pivot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}
}