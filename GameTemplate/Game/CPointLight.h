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
	private:
		PointLigData pointligData;

		int m_pointLigNum;
	public:
		~CPointLight();
		bool Start();
		void Update();

		//�|�C���g���C�g�̃f�[�^���擾
		PointLigData* GetLigData() { return &pointligData; }

		//�|�C���g���C�g�̃T�C�Y���擾
		int GetLigDataSize() { return sizeof(PointLigData); }

		//���W�̐ݒ�Ǝ擾
		void SetPosition(Vector3 pos) { pointligData.ligPos = pos; }
		Vector3 GetPosition() { return pointligData.ligPos; }

		//�F�̐ݒ�Ǝ擾
		void SetColor(Vector3 color) { pointligData.ligColor = color; }
		Vector3 GetColor() { return pointligData.ligColor; }

		//�e���͈͂̐ݒ�Ǝ擾
		void SetRange(float range) { pointligData.ligRange = range; }
		float GetRange() { return pointligData.ligRange; }
	};
}
