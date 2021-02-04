#pragma once
#include "stdafx.h"
#include "CDirectionLight.h"
#include "CPointLight.h"

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
		prefab::PointLigData pointLightArray[10];
		Vector3 eyePos;
		int directionLightNum = 0;
		int pointLightNum = 0;
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

	void UpdateEyePos() { m_ligData.eyePos = g_camera3D->GetPosition(); }

	//�f�B���N�V�������C�g�p
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

	//�|�C���g���C�g�p
	int PointLightPlus() { return m_ligData.pointLightNum++; }

	void PointLightMinus()
	{
		m_ligData.pointLightNum--;
		if (m_ligData.pointLightNum < 0)
			throw;
	}

	int AddPointLight(prefab::PointLigData* pointLigData)
	{
		if (m_ligData.pointLightNum >= 9)
			throw;

		m_ligData.pointLightArray[m_ligData.pointLightNum] = *pointLigData;

		return PointLightPlus();
	}

	void UpdatePointLight(int pointLightNum, prefab::PointLigData* pointLigData)
	{
		m_ligData.pointLightArray[pointLightNum] = *pointLigData;
	}
};

