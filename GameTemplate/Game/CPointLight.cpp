#include "stdafx.h"
#include "CPointLight.h"

namespace prefab
{
	CPointLight::~CPointLight()
	{
		CLightManager::GetInstance()->PointLightMinus();
	}

	bool CPointLight::Start()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		m_pointLigNum = CLightManager::GetInstance()->AddPointLight(GetLigData());

		return true;
	}

	void CPointLight::Update()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		CLightManager::GetInstance()->UpdatePointLight(m_pointLigNum, GetLigData());
	}
}