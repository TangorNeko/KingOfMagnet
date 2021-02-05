#include "stdafx.h"
#include "CDirectionLight.h"

namespace prefab
{
	//�f�B���N�V�������C�g�̐������炵����
	CDirectionLight::~CDirectionLight()
	{
		CLightManager::GetInstance()->DirectionLightMinus();
	}

	//�f�B���N�V�������C�g�̐��𑝂₵����
	bool CDirectionLight::Start()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		m_dirLigNum = CLightManager::GetInstance()->AddDirectionLight(GetLigData());

		return true;
	}

	//���t���[���J�����̈ʒu�ƃf�B���N�V�������C�g�̃f�[�^���X�V
	void CDirectionLight::Update()
	{
		CLightManager::GetInstance()->UpdateEyePos();
		CLightManager::GetInstance()->UpdateDirectionLight(m_dirLigNum,GetLigData());
	}
}