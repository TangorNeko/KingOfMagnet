#include "stdafx.h"
#include "CPointLight.h"

namespace prefab
{
	//�|�C���g���C�g�̐������炵����
	CPointLight::~CPointLight()
	{
		CLightManager::GetInstance()->RemovePointLight(m_pointLigTag);
	}

	//�|�C���g���C�g�̐��𑝂₵����
	bool CPointLight::Start()
	{
		m_pointLigTag = CLightManager::GetInstance()->AddPointLight(GetLigData());

		return true;
	}

	//���t���[���J�����̈ʒu�ƃ|�C���g���C�g�̃f�[�^���X�V
	void CPointLight::Update()
	{
		CLightManager::GetInstance()->UpdatePointLight(m_pointLigTag, GetLigData());
	}
}