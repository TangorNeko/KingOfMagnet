#include "stdafx.h"
#include "CSpotLight.h"

namespace prefab
{
	//スポットライトの数を減らした時
	CSpotLight::~CSpotLight()
	{
		CLightManager::GetInstance()->RemoveSpotLight(m_spotLigTag);
	}

	//スポットライトの数を増やした時
	bool CSpotLight::Start()
	{
		m_spotLigTag = CLightManager::GetInstance()->AddSpotLight(GetLigData());

		return true;
	}

	//毎フレームカメラの位置とスポットライトのデータを更新
	void CSpotLight::Update()
	{
		CLightManager::GetInstance()->UpdateSpotLight(m_spotLigTag, GetLigData());
	}
}