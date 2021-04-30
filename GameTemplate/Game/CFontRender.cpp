#include "stdafx.h"
#include "CFontRender.h"

namespace prefab
{
	void CFontRender::PostRender(RenderContext& rc, Camera* camera)
	{
		if (rc.GetRenderStep() == m_drawScreen)
		{
			m_font.Begin(rc);
			if (m_drawScreen == AllScreen)
			{
				//1‰æ–Ê‚Ì‚Í‘å‚«‚³‚»‚Ì‚Ü‚ÜB
				m_font.Draw(m_text.c_str(), m_position, m_color, m_rotation, m_scale, m_pivot);
			}
			else
			{
				//2‰æ–Ê•ªŠ„‚Ì‚Íc’·‚É’×‚ê‚é‚Ì‚Å‰¡‚É2”{L‚Î‚·B
				m_font.Draw(m_text.c_str(), m_position, m_color, m_rotation, { m_scale.x * 2.0f,m_scale.y}, m_pivot);
			}
			m_font.End(rc);
		}
	}
}