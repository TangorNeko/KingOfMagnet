/*!
 * @brief	エフェクト
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
		//再生中のエフェクトを停止する。
		EffectEngine::GetInstance()->Stop(m_handle);
		//新しく再生。
		m_handle = EffectEngine::GetInstance()->Play(m_effect);
	}

	void CEffect::Stop()
	{
		//再生中のエフェクトを停止する。
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
