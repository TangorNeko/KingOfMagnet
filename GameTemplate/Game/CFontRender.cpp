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
				//1��ʂ̎��͑傫�����̂܂܁B
				m_font.Draw(m_text.c_str(), m_position, m_color, m_rotation, m_scale, m_pivot);
			}
			else
			{
				//2��ʕ����̎��͏c���ɒׂ��̂ŉ���2�{�L�΂��B
				m_font.Draw(m_text.c_str(), m_position, m_color, m_rotation, { m_scale.x * 2.0f,m_scale.y}, m_pivot);
			}
			m_font.End(rc);
		}
	}
}