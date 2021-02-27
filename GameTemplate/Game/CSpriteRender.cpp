#include "stdafx.h"
#include "CSpriteRender.h"

namespace prefab
{
	void CSpriteRender::Init(const char* spritePath,UINT width,UINT height)
	{
		SpriteInitData initData;

		initData.m_ddsFilePath[0] = spritePath;

		initData.m_vsEntryPointFunc = "VSMain";

		initData.m_psEntryPoinFunc = "PSMain";

		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		initData.m_width = width;

		initData.m_height = height;

		m_sprite.Init(initData);
	}

	void CSpriteRender::Render(RenderContext& rc, Camera* camera)
	{
		m_sprite.Draw(rc);
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