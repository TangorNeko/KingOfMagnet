#pragma once
#include <map>
#include <utility>
#include "stdafx.h"
#include "CDirectionLight.h"
#include "CPointLight.h"
#include "CSpotLight.h"

//�e���C�g�̍ő吔�@���̐���model.fx�̃��C�g�z��̐��ƈꏏ�ɂ��Ȃ���΂Ȃ�Ȃ�
const int MaxDirectionLightNum = 5;
const int MaxPointLightNum = 20;
const int MaxSpotLightNum = 20;

//TODO:���C�g�����ۍő吔�ȏ�̎�throw����̂ł͂Ȃ���ԌÂ����C�g�������悤�ȏ����ɂ�������悢?
//����ɏ���(��:��ԌÂ����C�g�^�O�ɑΉ�����������-1�ɂ��AUpdateLight��-1�Ȃ�Light.cpp��Delete(this)������)
//�Ɩ����ȃA�h���X�ɒl�����邱�ƂɂȂ肻������
//�f�[�^���g��Ȃ������ŏ����Ȃ��悤�ɂ���(��:�Â����C�g�^�O�ɑΉ�����������-1�ɂ��邾��)
//�Ƃ���͂���Ń������̖��ʌ����ɂȂ肻��
class CLightManager
{
private:
	CLightManager() = default;
	~CLightManager() = default;

	static CLightManager* m_instance;

	//���C�g�̏��ƃJ�����̈ʒu�ƃ��C�g�̐����W�߂��f�[�^(�萔�o�b�t�@�Ƃ���HLSL�ɑ�����\����)
	struct LigDatas
	{
		prefab::DirLigData directionLightArray[MaxDirectionLightNum];	//�f�B���N�V�������C�g�̃f�[�^�̔z��
		prefab::PointLigData pointLightArray[MaxPointLightNum];			//�|�C���g���C�g�̃f�[�^�̔z��
		prefab::SpotLigData spotLightArray[MaxSpotLightNum];			//�X�|�b�g���C�g�̃f�[�^�̔z��
		Matrix lightCameraProjectionMatrix;								//���C�g�r���[�v���W�F�N�V�����s��
		Vector3 eyePos;													//�J�����̈ʒu
		int directionLightNum = 0;										//�f�B���N�V�������C�g�̐�
		int pointLightNum = 0;											//�|�C���g���C�g�̐�
		int spotLightNum = 0;											//�X�|�b�g���C�g�̐�
	};
	LigDatas m_ligData;				//���C�g�̃f�[�^
	int m_size = sizeof(m_ligData);	//���C�g�̃f�[�^�̃T�C�Y
	
	int m_dirLigNum = 0;				//���ɍ����f�B���N�V�������C�g�ɕt�^����^�O�ԍ�(������܂łɉ��f�B���N�V�������C�g�����ꂽ��)
	int m_pointLigNum = 0;				//���ɍ����|�C���g���C�g�ɕt�^����^�O�ԍ�(������܂łɉ��|�C���g���C�g�����ꂽ��)
	int m_spotLigNum = 0;				//���ɍ����X�|�b�g���C�g�ɕt�^����^�O�ԍ�(������܂łɉ��X�|�b�g���C�g�����ꂽ��)
	std::map<int, int> m_dirLigMap;		//�f�B���N�V�������C�g�̃^�O���猻�݂̃f�B���N�V�������C�g�̈ʒu��Ԃ��Ă����Map
	std::map<int, int> m_pointLigMap;	//�|�C���g���C�g�̃^�O���猻�݂̃|�C���g���C�g�̈ʒu��Ԃ��Ă����Map
	std::map<int, int> m_spotLigMap;	//�X�|�b�g���C�g�̃^�O���猻�݂̃X�|�b�g���C�g�̈ʒu��Ԃ��Ă����Map

	Camera m_lightCamera;//�V���h�E�}�b�v�p�̃��C�g�̈ʒu�̃J�����B�Ƃ肠�����e�X�g�B
public:
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new CLightManager;
		}
	}

	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	static CLightManager* GetInstance() { return m_instance; }

	//���C�g�̃f�[�^�̉���擾����(�萔�o�b�t�@�ɓn���p)
	LigDatas* GetLigDatas() { return &m_ligData; }

	//���C�g�̃f�[�^�̉�̃T�C�Y���擾����(�萔�o�b�t�@�ɓn���p)
	int GetDataSize() { return m_size; }

	//�J�����̃|�W�V�������X�V����
	void UpdateEyePos(int camNo) { m_ligData.eyePos = g_camera3D[camNo]->GetPosition(); }

	Camera* GetLightCamera()
	{
		return &m_lightCamera;
	}

	//�V���h�E�p
	void SetLightCameraPosition(const Vector3& pos)
	{
		m_lightCamera.SetPosition(pos);
		m_lightCamera.Update();
	}

	void SetLightCameraTarget(const Vector3& targetPos)
	{
		m_lightCamera.SetTarget(targetPos);
		m_lightCamera.Update();
	}

	void SetLightCameraUp(const Vector3& up)
	{
		m_lightCamera.SetUp(up);
		m_lightCamera.Update();
	}

	void SetLightCameraViewAngle(const float& deg)
	{
		m_lightCamera.SetViewAngle(Math::DegToRad(deg));
		m_lightCamera.Update();
	}

	//�f�B���N�V�������C�g�p////////////////////////////////////////////////////////////////////////////////////////////////
	//Map���g���Ĉ��S�Ƀ��C�g���폜�ł���d�g�݂�����Ă݂��B�I�u�W�F�N�g�̍폜�����܂������Ă��Ȃ����߂܂����؂ł��Ă��Ȃ�
	//TODO:���C�g�̍폜�̌���

	//�f�B���N�V�������C�g�̐��̃J�E���g���v���X����
	int DirectionLightPlus() { return m_ligData.directionLightNum++; }

	//�f�B���N�V�������C�g�̐��̃J�E���g���}�C�i�X����
	void DirectionLightMinus() {
		m_ligData.directionLightNum--;

		//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
		if (m_ligData.directionLightNum < 0)
		{
			throw;
		}
	}

	//�f�B���N�V�������C�g��ǉ�����
	int AddDirectionLight(prefab::DirLigData* dirLigData)
	{
		//���C�g�̐����ŏ��Ɍ��߂����ȏ�Ȃ�throw����(�����ς����Ƃӂ��ɋN����)
		if (m_ligData.directionLightNum >= MaxDirectionLightNum)
		{
			throw;
		}

		//�󂫂̒��ň�Ԑ擪�ʒu�Ƀf�[�^���i�[����
		m_ligData.directionLightArray[m_ligData.directionLightNum] = *dirLigData;

		//��������C�g�̃^�O�ԍ��ƃf�[�^�̊i�[�ʒu���֘A�t����
		m_dirLigMap.insert(std::make_pair(m_dirLigNum, m_ligData.directionLightNum));

		//���Ƀ��C�g������ʒu�����炷
		DirectionLightPlus();

		//��������C�g�̃^�O�ԍ���Ԃ��A1���₵�Ď��̃��C�g�̍쐬�ɔ�����
		return m_dirLigNum++;
	}

	//�f�B���N�V�������C�g���폜����
	void RemoveDirectionLight(int directionLightTag)
	{
		//�^�O����폜����ʒu���擾���A�폜���郉�C�g���\�[�g�ň�ԏI�[�Ɏ����Ă���
		for (int i = m_dirLigMap.at(directionLightTag);i < m_ligData.directionLightNum - 1;i++)
		{
			std::swap(m_ligData.directionLightArray[i], m_ligData.directionLightArray[i + 1]);
		}

		//���C�g�̐������炷���Ƃŋ󂢂��ʒu�Ɏ��̃��C�g��������悤�ɂȂ�
		DirectionLightMinus();

		//�폜�������C�g�ȍ~�̃��C�g�̈ʒu�ԍ���������ꂽ�̂Ń^�O�ɑΉ������ʒu�̔ԍ���1�����炷
		for (auto i = m_dirLigMap.upper_bound(directionLightTag);i != m_dirLigMap.end();i++)
		{
			i->second--;
		}
	}

	//�f�B���N�V�������C�g�̏����X�V����
	void UpdateDirectionLight(int directionLightTag, prefab::DirLigData* dirLigData)
	{
		//�^�O����擾�������C�g�̈ʒu�̃f�[�^���X�V����B
		m_ligData.directionLightArray[m_dirLigMap.at(directionLightTag)] = *dirLigData;
	}

	//�|�C���g���C�g�p///////////////////////////////////////////////////////////////////////////////////////////////////////

	//�|�C���g���C�g�̐��̃J�E���g���v���X����
	int PointLightPlus() { return m_ligData.pointLightNum++; }

	//�|�C���g���C�g�̐��̃J�E���g���}�C�i�X����
	//WARNING:���炩�Ɋ댯 ���Ԗڂ̃��C�g���������c������K�v������
	void PointLightMinus()
	{
		m_ligData.pointLightNum--;

		//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
		if (m_ligData.pointLightNum < 0)
		{
			throw;
		}
	}

	//�|�C���g���C�g��ǉ�����
	int AddPointLight(prefab::PointLigData* pointLigData)
	{
		//���C�g�̐����ŏ��Ɍ��߂����ȏ�Ȃ�throw����(�����ς����Ƃӂ��ɋN����)
		if (m_ligData.pointLightNum >= MaxPointLightNum)
		{
			throw;
		}

		//�󂫂̒��ň�Ԑ擪�ʒu�Ƀf�[�^���i�[����
		m_ligData.pointLightArray[m_ligData.pointLightNum] = *pointLigData;

		//��������C�g�̃^�O�ԍ��ƃf�[�^�̊i�[�ʒu���֘A�t����
		m_pointLigMap.insert(std::make_pair(m_pointLigNum, m_ligData.pointLightNum));

		//���Ƀ��C�g������ʒu�����炷
		PointLightPlus();

		//��������C�g�̃^�O�ԍ���Ԃ��A1���₵�Ď��̃��C�g�̍쐬�ɔ�����
		return m_pointLigNum++;
	}

	//�|�C���g���C�g���폜����
	void RemovePointLight(int pointLightTag)
	{
		//�^�O����폜����ʒu���擾���A�폜���郉�C�g���\�[�g�ň�ԏI�[�Ɏ����Ă���
		for (int i = m_pointLigMap.at(pointLightTag);i < m_ligData.pointLightNum - 1;i++)
		{
			std::swap(m_ligData.pointLightArray[i], m_ligData.pointLightArray[i + 1]);
		}

		//���C�g�̐������炷���Ƃŋ󂢂��ʒu�Ɏ��̃��C�g��������悤�ɂȂ�
		PointLightMinus();

		//�폜�������C�g�ȍ~�̃��C�g�̈ʒu�ԍ���������ꂽ�̂Ń^�O�ɑΉ������ʒu�̔ԍ���1�����炷
		for (auto i = m_pointLigMap.upper_bound(pointLightTag);i != m_pointLigMap.end();i++)
		{
			i->second--;
		}
	}

	//�|�C���g���C�g�̏����X�V����
	void UpdatePointLight(int pointLightTag, prefab::PointLigData* pointLigData)
	{
		m_ligData.pointLightArray[m_pointLigMap.at(pointLightTag)] = *pointLigData;
	}

	//�X�|�b�g���C�g�p/////////////////////////////////////////////////////////////////////////////////////////////////////

	//�X�|�b�g���C�g�̐��̃J�E���g���v���X����
	int SpotLightPlus() { return m_ligData.spotLightNum++; }

	//�X�|�b�g���C�g�̐��̃J�E���g���}�C�i�X����
	void SpotLightMinus()
	{
		m_ligData.spotLightNum--;

		//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
		if (m_ligData.spotLightNum < 0)
		{
			throw;
		}
	}

	//�X�|�b�g���C�g��ǉ�����
	int AddSpotLight(prefab::SpotLigData* spotLigData)
	{
		//���C�g�̐����ŏ��Ɍ��߂����ȏ�Ȃ�throw����(�����ς����Ƃӂ��ɋN����)
		if (m_ligData.spotLightNum >= MaxSpotLightNum)
		{
			throw;
		}

		//�󂫂̒��ň�Ԑ擪�ʒu�Ƀf�[�^���i�[����
		m_ligData.spotLightArray[m_ligData.spotLightNum] = *spotLigData;

		//��������C�g�̃^�O�ԍ��ƃf�[�^�̊i�[�ʒu���֘A�t����
		m_spotLigMap.insert(std::make_pair(m_spotLigNum, m_ligData.spotLightNum));

		//���Ƀ��C�g������ʒu�����炷
		SpotLightPlus();

		//��������C�g�̃^�O�ԍ���Ԃ��A1���₵�Ď��̃��C�g�쐬�ɔ�����
		return m_spotLigNum++;
	}

	//�X�|�b�g���C�g���폜����
	void RemoveSpotLight(int spotLightTag)
	{
		//�^�O����폜����ʒu���擾���A�폜���郉�C�g���\�[�g�ň�ԏI�[�Ɏ����Ă���
		for (int i = m_spotLigMap.at(spotLightTag);i < m_ligData.spotLightNum - 1;i++)
		{
			std::swap(m_ligData.spotLightArray[i], m_ligData.spotLightArray[i + 1]);
		}

		//���C�g�̐������炷���Ƃŋ󂢂��ʒu�Ɏ��̃��C�g��������悤�ɂȂ�
		SpotLightMinus();

		//�폜�������C�g�ȍ~�̃��C�g�̈ʒu�ԍ���������ꂽ�̂Ń^�O�ɑΉ������ʒu�̔ԍ���1�����炷
		for (auto i = m_spotLigMap.upper_bound(spotLightTag);i != m_spotLigMap.end();i++)
		{
			i->second--;
		}
	}

	//�X�|�b�g���C�g�̏����X�V����
	void UpdateSpotLight(int spotLightTag, prefab::SpotLigData* spotLigData)
	{
		m_ligData.spotLightArray[m_spotLigMap.at(spotLightTag)] = *spotLigData;
	}

};

