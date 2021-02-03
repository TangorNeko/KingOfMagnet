#pragma once
#include "stdafx.h"
#include "CDirectionLight.h"

//TODO:���C�g���������̎d�g�݂��ł��Ă��Ȃ��B���炩�Ɉ���ʍs
//�폜�����ۂɃ\�[�g�ō��ɂ߂���directionLightNum�����炷?
//���������CDirectionLight����AddDirectionLight�̎��Ɏ󂯎�����Ɩ��̔ԍ��������ɂȂ肻���Ȃ̂�
//UpdateDirectionLight�̎��Ɏ󂯎�����i���o�[����X�V��̃i���o�[��n�����炤�ꂵ���B
class CLightManager
{
private:
	CLightManager() = default;
	~CLightManager() = default;

	static CLightManager* m_instance;

	//���C�g�̏��ƃ��C�g�̐����W�߂��f�[�^(�萔�o�b�t�@�Ƃ���HLSL�ɑ�����\����)
	struct LigDatas
	{
		prefab::DirLigData directionLightArray[5];
		int directionLightNum = 0;
	};

	LigDatas m_ligData;
	int m_size = sizeof(m_ligData);
public:
	static void CreateInstance()
	{
		if (!m_instance)
			m_instance = new CLightManager;
	}

	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	static CLightManager* GetInstance() { return m_instance; }

	LigDatas* GetLigDatas() { return &m_ligData; }

	int GetDataSize() { return m_size; }

	int DirectionLightPlus() { return m_ligData.directionLightNum++; }


	//WARNING:���炩�Ɋ댯 ���Ԗڂ̃��C�g���������c������K�v������
	void DirectionLightMinus() {
		m_ligData.directionLightNum--;
		if (m_ligData.directionLightNum < 0)
			throw;
	}

	int AddDirectionLight(prefab::DirLigData* dirLigData)
	{
		if (m_ligData.directionLightNum >= 4)
			throw;

		m_ligData.directionLightArray[m_ligData.directionLightNum] = *dirLigData;

		return DirectionLightPlus();
	}

	void UpdateDirectionLight(int directionLightNum, prefab::DirLigData* dirLigData)
	{
		m_ligData.directionLightArray[directionLightNum] = *dirLigData;
	}
};

