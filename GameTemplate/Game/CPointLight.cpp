#include "stdafx.h"
#include "CPointLight.h"

namespace prefab
{
	//ポイントライトの数を減らした時
	CPointLight::~CPointLight()
	{
		CLightManager::GetInstance()->PointLightMinus();
	}

	//ポイントライトの数を増やした時
	bool CPointLight::Start()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		m_pointLigTag = CLightManager::GetInstance()->AddPointLight(GetLigData());

		return true;
	}

	//毎フレームカメラの位置とポイントライトのデータを更新
	void CPointLight::Update()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		CLightManager::GetInstance()->UpdatePointLight(m_pointLigTag, GetLigData());
	}
}