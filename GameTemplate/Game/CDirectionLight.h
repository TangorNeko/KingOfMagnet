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
	private:
		DirLigData dirLigData;	//�f�B���N�V�������C�g�̃f�[�^

		int m_dirLigTag;		//�f�B���N�V�������C�g�̔ԍ�(���Ԗڂɍ��ꂽ���C�g?)
	public:
		~CDirectionLight();
		bool Start();
		void Update();

		//�f�B���N�V�������C�g�̃f�[�^���擾
		DirLigData* GetLigData() { return &dirLigData; }
		
		//�f�B���N�V�������C�g�̃T�C�Y���擾
		int GetLigDataSize() { return sizeof(dirLigData); }

		//�����̐ݒ�Ǝ擾
		void SetDirection(Vector3 dir) { 
			dirLigData.ligDir = dir;
			dirLigData.ligDir.Normalize();
		}
		Vector3 GetDirecion() { return dirLigData.ligDir; }

		//�F�̐ݒ�Ǝ擾
		void SetColor(Vector3 color) { dirLigData.ligColor = color; }
		Vector3 GetColor() { return dirLigData.ligColor; }
	};
}

