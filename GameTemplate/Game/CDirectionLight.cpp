#include "stdafx.h"
#include "CDirectionLight.h"

namespace prefab
{
	//�f�B���N�V�������C�g�̐������炵����
	CDirectionLight::~CDirectionLight()
	{
		CLightManager::GetInstance()->RemoveDirectionLight(m_dirLigTag);
	}

	//�f�B���N�V�������C�g�̐��𑝂₵����
	bool CDirectionLight::Start()
	{
		m_dirLigTag = CLightManager::GetInstance()->AddDirectionLight(GetLigData());

		return true;
	}

	//���t���[���J�����̈ʒu�ƃf�B���N�V�������C�g�̃f�[�^���X�V
	void CDirectionLight::Update()
	{
		CLightManager::GetInstance()->UpdateDirectionLight(m_dirLigTag,GetLigData());
	}
}