#include "stdafx.h"
#include "CDirectionLight.h"

namespace prefab
{
	//ディレクションライトの数を減らした時
	CDirectionLight::~CDirectionLight()
	{
		CLightManager::GetInstance()->DirectionLightMinus();
	}

	//ディレクションライトの数を増やした時
	bool CDirectionLight::Start()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		m_dirLigNum = CLightManager::GetInstance()->AddDirectionLight(GetLigData());

		return true;
	}

	//毎フレームカメラの位置とディレクションライトのデータを更新
	void CDirectionLight::Update()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		CLightManager::GetInstance()->UpdateDirectionLight(m_dirLigNum,GetLigData());
	}
}