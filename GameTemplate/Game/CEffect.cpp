/*!
 * @brief	�G�t�F�N�g
 */

#include "stdafx.h"
#include "CEffect.h"

namespace prefab
{

	CEffect::CEffect()
	{
	}
	CEffect::~CEffect()
	{

	}


	void CEffect::Init(const char16_t* filePath)
	{
		m_effect = EffectEngine::GetInstance()->LoadEffect(filePath);

	}
	void CEffect::Play()
	{
		//�Đ����̃G�t�F�N�g���~����B
		EffectEngine::GetInstance()->Stop(m_handle);
		//�V�����Đ��B
		m_handle = EffectEngine::GetInstance()->Play(m_effect);
	}

	void CEffect::Stop()
	{
		//�Đ����̃G�t�F�N�g���~����B
		EffectEngine::GetInstance()->Stop(m_handle);
	}

	void CEffect::Update()
	{
		EffectEngine::GetInstance()->UpdateEffectWorldMatrix(
			m_handle,
			m_position,
			m_rotation,
			m_scale
		);
	}
}
