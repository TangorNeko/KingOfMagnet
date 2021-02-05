#include "stdafx.h"
#include "CSpotLight.h"

namespace prefab
{
	//�X�|�b�g���C�g�̐������炵����
	CSpotLight::~CSpotLight()
	{
		CLightManager::GetInstance()->SpotLightMinus();
	}

	//�X�|�b�g���C�g�̐��𑝂₵����
	bool CSpotLight::Start()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		m_spotLigNum = CLightManager::GetInstance()->AddSpotLight(GetLigData());

		return true;
	}

	//���t���[���J�����̈ʒu�ƃX�|�b�g���C�g�̃f�[�^���X�V
	void CSpotLight::Update()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		CLightManager::GetInstance()->UpdateSpotLight(m_spotLigNum, GetLigData());
	}
}