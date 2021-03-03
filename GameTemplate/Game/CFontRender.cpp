#include "stdafx.h"
#include "CFontRender.h"

namespace prefab
{
	void CFontRender::Render(RenderContext& rc, Camera* camera)
	{
		if (m_drawScreen == AllScreen || rc.GetRenderStep() == m_drawScreen)
		{
			m_font.Begin(rc);
			m_font.Draw(m_text,m_position,m_color,m_rotation,m_scale,m_pivot);
			m_font.End(rc);
		}
	}
}