#include "stdafx.h"
#include "CDirectionLight.h"

namespace prefab
{
	//ディレクションライトの数を減らした時
	CDirectionLight::~CDirectionLight()
	{
		CLightManager::GetInstance()->RemoveDirectionLight(m_dirLigTag);
	}

	//ディレクションライトの数を増やした時
	bool CDirectionLight::Start()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		m_dirLigTag = CLightManager::GetInstance()->AddDirectionLight(GetLigData());

		return true;
	}

	//毎フレームカメラの位置とディレクションライトのデータを更新
	void CDirectionLight::Update()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		CLightManager::GetInstance()->UpdateDirectionLight(m_dirLigTag,GetLigData());
	}
}