#include "stdafx.h"
#include "CSpotLight.h"

namespace prefab
{
	//スポットライトの数を減らした時
	CSpotLight::~CSpotLight()
	{
		CLightManager::GetInstance()->SpotLightMinus();
	}

	//スポットライトの数を増やした時
	bool CSpotLight::Start()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		m_spotLigNum = CLightManager::GetInstance()->AddSpotLight(GetLigData());

		return true;
	}

	//毎フレームカメラの位置とスポットライトのデータを更新
	void CSpotLight::Update()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		CLightManager::GetInstance()->UpdateSpotLight(m_spotLigNum, GetLigData());
	}
}