#pragma once
#include "tklFile.h"
#include "CMapChipRender.h"

namespace prefab
{
	//���x���̃I�u�W�F�N�g���̃f�[�^
	struct LevelObjectData
	{
		Vector3 position = Vector3::Zero;
		Quaternion rotation = Quaternion::Identity;
		Vector3 scale = Vector3::One;
		char* name = nullptr;
	};

	class CLevel
	{
	private:
		//�ǂݍ���tkl�t�@�C���̃f�[�^���i�[�����B
		tklFile m_tklFile;
		
		//�{�[���̃��j�[�N�|�C���^�B
		typedef std::unique_ptr<Bone> CBone;

		//�{�[���̃��j�[�N�|�C���^�̉ϒ��z��
		std::vector<CBone> m_bone;

		//�s��̃��j�[�N�|�C���^�B
		std::unique_ptr<Matrix[]> m_boneMatrixs;

		//������������������?
		bool m_isInited = false;

		//�{�[���������m_bone�ɓn��
		void BuildBone();

		std::map<unsigned int, CMapChipRender*> m_mapChipRenderPtrs;
	public:
		~CLevel();

		//���x���̏�����
		bool Init(const char* levelFilePath,std::function<bool(LevelObjectData& objectData)> hookFunc);

		//
		CMapChipRender* CreateMapChipRenderOrAddRenderObject(const LevelObjectData& objData);
	};
}

