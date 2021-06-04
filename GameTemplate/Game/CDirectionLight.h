#pragma once

namespace prefab
{
	struct DirLigData
	{
		Vector3 ligDir;		//���C�g�̌���
		float pad0;			//�p�f�B���O
		Vector3 ligColor;	//���C�g�̐F
		float pad1;			//�p�f�B���O(�P�̂��ƕK�v�Ȃ������ꂪ�Ȃ���DirLigData��z��ɂ�����HLSL��4�o�C�g������Ă������ƂɂȂ�?)
	};

	class CDirectionLight : public IGameObject
	{
		~CDirectionLight();
		bool Start();
		void Update();

	public:
		/**
		 * @brief �f�B���N�V�������C�g�̃f�[�^���擾
		 * @return �f�B���N�V�������C�g�̃f�[�^
		*/
		DirLigData* GetLigData() { return &m_dirLigData; }
		
		/**
		 * @brief �f�B���N�V�������C�g�̃f�[�^�̃T�C�Y���擾
		 * @return �f�B���N�V�������C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() { return sizeof(m_dirLigData); }

		/**
		 * @brief �f�B���N�V�������C�g�̌�����ݒ�
		 * @param dir �f�B���N�V�������C�g�̌���
		*/
		void SetDirection(Vector3 dir) { 
			m_dirLigData.ligDir = dir;
			m_dirLigData.ligDir.Normalize();
		}

		/**
		 * @brief �f�B���N�V�������C�g�̌������擾
		 * @return �f�B���N�V�������C�g�̌���
		*/
		Vector3 GetDirecion() { return m_dirLigData.ligDir; }

		/**
		 * @brief �f�B���N�V�������C�g�̐F��ݒ�
		 * @param color �f�B���N�V�������C�g�̐F
		*/
		void SetColor(Vector3 color) { m_dirLigData.ligColor = color; }

		/**
		 * @brief �f�B���N�V�����̃��C�g�̐F���擾
		 * @return �f�B���N�V�������C�g�̐F
		*/
		Vector3 GetColor() { return m_dirLigData.ligColor; }

	private:
		DirLigData m_dirLigData;	//�f�B���N�V�������C�g�̃f�[�^
		int m_dirLigTag;			//�f�B���N�V�������C�g�̔ԍ�(���Ԗڂɍ��ꂽ���C�g?)
	};
}

