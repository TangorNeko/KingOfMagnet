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

		initData.m_psEntryPoinFunc = "PSMain";

		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		initData.m_width = width;

		initData.m_height = height;

		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		initData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_sprite.Init(initData);
	}

	//スプライトのドローコール
	void CSpriteRender::PostRender(RenderContext& rc, Camera* camera)
	{
		if (m_drawScreen == AllScreen || rc.GetRenderStep() == m_drawScreen)
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