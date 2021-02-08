#include "stdafx.h"
#include "CSpotLight.h"

namespace prefab
{
	//�X�|�b�g���C�g�̐������炵����
	CSpotLight::~CSpotLight()
	{
		CLightManager::GetInstance()->RemoveSpotLight(m_spotLigTag);
	}

	//�X�|�b�g���C�g�̐��𑝂₵����
	bool CSpotLight::Start()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		m_spotLigTag = CLightManager::GetInstance()->AddSpotLight(GetLigData());

		return true;
	}

	//���t���[���J�����̈ʒu�ƃX�|�b�g���C�g�̃f�[�^���X�V
	void CSpotLight::Update()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		CLightManager::GetInstance()->UpdateSpotLight(m_spotLigTag, GetLigData());
	}
}