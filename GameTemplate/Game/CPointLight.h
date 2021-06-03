#pragma once

namespace prefab
{
	struct PointLigData
	{
		Vector3 ligPos;		//���C�g�̍��W
		float pad;			//�p�f�B���O
		Vector3 ligColor;	//���C�g�̐F
		float ligRange;		//���C�g�̉e���͈�
	};

	class CPointLight : public IGameObject
	{
		~CPointLight();
		bool Start();
		void Update();

	public:
		/**
		 * @brief �|�C���g���C�g�̃f�[�^���擾
		 * @return �|�C���g���C�g�̃f�[�^
		*/
		PointLigData* GetLigData() { return &m_pointLigData; }

		/**
		 * @brief �|�C���g���C�g�̃f�[�^�̃T�C�Y���擾
		 * @return �|�C���g���C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() { return sizeof(PointLigData); }

		/**
		 * @brief �|�C���g���C�g�̍��W��ݒ�
		 * @param pos �|�C���g���C�g�̍��W
		*/
		void SetPosition(Vector3 pos) { m_pointLigData.ligPos = pos; }

		/**
		 * @brief �|�C���g���C�g�̍��W���擾
		 * @return �|�C���g���C�g�̍��W 
		*/
		Vector3 GetPosition() { return m_pointLigData.ligPos; }

		/**
		 * @brief �|�C���g���C�g�̐F��ݒ�
		 * @param color �|�C���g���C�g�̐F
		*/
		void SetColor(Vector3 color) { m_pointLigData.ligColor = color; }

		/**
		 * @brief �|�C���g���C�g�̐F���擾
		 * @return �|�C���g���C�g�̐F
		*/
		Vector3 GetColor() { return m_pointLigData.ligColor; }

		/**
		 * @brief �|�C���g���C�g�̉e���͈͂�ݒ�
		 * @param range �|�C���g���C�g�̉e���͈�
		*/
		void SetRange(float range) { m_pointLigData.ligRange = range; }

		/**
		 * @brief �|�C���g���C�g�̉e���͈͂��擾
		 * @return �|�C���g���C�g�̉e���͈�
		*/
		float GetRange() { return m_pointLigData.ligRange; }

	private:
		PointLigData m_pointLigData;	//�|�C���g���C�g�̃f�[�^
		int m_pointLigTag;			//�|�C���g���C�g�̃^�O(���Ԗڂɍ��ꂽ���C�g?)
	};
}
