#include "stdafx.h"
#include "DropItem_base.h"

void DropItem_base::ItemMotion()
{
	angle += 0.5f;
	m_qrot.SetRotationDeg(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(m_qrot);
	if (m_position.y == 0.0f&&m_Upmotion==false)
	{
		m_Upmotion = true;
	}
	else if(m_position.y == 25.0f && m_Upmotion == true)
	{
		m_Upmotion = false;
	}
	if (m_Upmotion == true) 
	{
		m_position.y += 0.25f;
	}
	else
	{
		m_position.y -= 0.25f;
	}

	m_skinModelRender->SetPosition(m_position);
}