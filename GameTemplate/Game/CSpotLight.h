#pragma once

namespace prefab
{
	struct SpotLigData //�X�|�b�g���C�g�̃f�[�^
	{
		Vector3 ligPos;		//���C�g�̍��W
		float pad;			//�p�f�B���O
		Vector3 ligColor;	//���C�g�̐F
		float ligRange;		//���C�g�̉e���͈�
		Vector3 ligDir;		//���C�g�̌���
		float ligAngle;		//���C�g�̉e���p�x(���W�A���P��)
	};

	class CSpotLight : public IGameObject
	{
	private:
		SpotLigData spotLigData;	//�X�|�b�g���C�g�̃f�[�^

		int m_spotLigTag;			//�X�|�b�g���C�g�̃^�O(���Ԗڂɍ��ꂽ���C�g?)
		//TODO:���̃X�|�b�g���C�g�����������ɉ��Ԗڂ����X�V����Ȃ���΂����Ȃ�
	public:
		~CSpotLight();
		bool Start();
		void Update();

		//�X�|�b�g���C�g�̃f�[�^���擾
		SpotLigData* GetLigData() { return &spotLigData; }

		//�X�|�b�g���C�g�̃T�C�Y���擾
		int GetLigDataSize() { return sizeof(SpotLigData); }

		//���W�̐ݒ�Ǝ擾
		void SetPosition(Vector3 pos) { spotLigData.ligPos = pos; }
		Vector3 GetPosition() { return spotLigData.ligPos; }

		//�F�̐ݒ�Ǝ擾
		void SetColor(Vector3 color) { spotLigData.ligColor = color; }
		Vector3 GetColor() { return spotLigData.ligColor; }

		//�e���͈͂̐ݒ�Ǝ擾
		void SetRange(float range) { spotLigData.ligRange = range; }
		float GetRange() { return spotLigData.ligRange; }

		//�����̐ݒ�Ǝ擾
		void SetDirection(Vector3 dir) { spotLigData.ligDir = dir; }
		Vector3 GetDirection() { return spotLigData.ligDir; }

		//�e���p�x�̐ݒ�Ǝ擾(���W�A���P��)
		void SetAngle(float angle) { spotLigData.ligAngle = angle; }
		float GetAngle() { return spotLigData.ligAngle; }

		//�e���p�x�̐ݒ�Ǝ擾(�f�O���[�P��)
		void SetAngleDeg(float angle) { spotLigData.ligAngle = Math::DegToRad(angle); }
		float GetAngleDeg() { return Math::RadToDeg(spotLigData.ligAngle); }
	};
}

