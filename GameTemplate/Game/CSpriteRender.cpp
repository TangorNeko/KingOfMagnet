#include "stdafx.h"
#include "CSpriteRender.h"

namespace prefab
{
	/// <summary>
	/// スプライトの初期化関数
	/// </summary>
	/// <param name="spritePath">スプライトファイルのパス(.dds)</param>
	/// <param name="width">スプライトの幅</param>
	/// <param name="height">スプライトの高さ</param>
	void CSpriteRender::Init(const char* spritePath,UINT width,UINT height)
	{
		SpriteInitData initData;

		initData.m_ddsFilePath[0] = spritePath;

		initData.m_vsEntryPointFunc = "VSMain";

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

		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		initData.m_width = width;

		initData.m_height = height;

		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		initData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_sprite.Init(initData);

		//スプライトサポーターに自分の存在を伝える
		m_spriteSupporter.SpriteRenderSetting(this);
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	void CSpriteRender::Update()
	{
		//スプライトサポーターの更新
		m_spriteSupporter.SpriteSupporter_Update();
	}

	//スプライトのドローコール
	void CSpriteRender::Render(RenderContext& rc, Camera* camera)
	{
		if (rc.GetRenderStep() == m_drawScreen && m_isPostRender == false)
		{
			m_sprite.Draw(rc);
		}
	}

	//スプライトのドローコール
	void CSpriteRender::PostRender(RenderContext& rc, Camera* camera)
	{
		if (rc.GetRenderStep() == m_drawScreen && m_isPostRender == true)
		{
			m_sprite.Draw(rc);
		}
	}

	/// <summary>
	/// スプライトの座標を設定する。
	/// </summary>
	/// <param name="pos"></param>
	void CSpriteRender::SetPosition(Vector3 pos)
	{
		m_position = pos;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	/// <summary>
	/// スプライトの回転を設定する。
	/// </summary>
	/// <param name="qRot"></param>
	void CSpriteRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	//スプライトの拡大率を設定する。
	void CSpriteRender::SetScale(Vector3 scale)
	{
		m_scale = scale;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	//スプライトのピボットを設定する。
	void CSpriteRender::SetPivot(Vector2 pivot)
	{
		m_pivot = pivot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}
}