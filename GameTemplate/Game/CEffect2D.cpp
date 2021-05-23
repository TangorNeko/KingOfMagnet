#include "stdafx.h"
#include "CEffect2D.h"

namespace prefab
{
	CEffect2D::CEffect2D()
	{
	}
	CEffect2D::~CEffect2D()
	{

	}

	void CEffect2D::Init(const char16_t* filePath)
	{
		m_effect = EffectEngine::GetInstance()->LoadEffect2D(filePath);

	}
	void CEffect2D::Play()
	{
		//�Đ����̃G�t�F�N�g���~����B
		EffectEngine::GetInstance()->Stop2D(m_handle);
		//�V�����Đ��B
		m_handle = EffectEngine::GetInstance()->Play2D(m_effect);
	}

	void CEffect2D::Stop()
	{
		//�Đ����̃G�t�F�N�g���~����B
		EffectEngine::GetInstance()->Stop2D(m_handle);
	}

	void CEffect2D::Update()
	{
		//x���W�����]����̂Ŕ��]�����ēn���B
		Vector3 xReversePos = { -m_position.x,m_position.y,m_position.z };

		EffectEngine::GetInstance()->UpdateEffectWorldMatrix2D(
			m_handle,
			xReversePos,
			m_rotation,
			m_scale
		);
	}
}