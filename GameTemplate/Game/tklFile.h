#pragma once
class tklFile
{
public:
	struct SObject
	{
		std::unique_ptr<char[]> boneName;	//�{�[���̖��O�B
		int parentBoneNo;					//�e�̃{�[���ԍ�
		int boneNo;							//�{�[���ԍ�
		Vector3 bindPose[4];				//�o�C���h�|�[�Y�s��
		Vector3 invBindPose[4];				//�o�C���h�|�[�Y�t�s��
		bool ShadowCasterFlag;				//�V���h�E�L���X�^�[�t���O(���g�p)
		bool ShadowReceiverFlag;			//�V���h�E���V�[�o�[�t���O(���g�p)
		std::vector<int> intData;			//int�̒ǉ��f�[�^
		std::vector<float> floatData;		//float�̒ǉ��f�[�^
		std::vector<char*> stringData;		//������̒ǉ��f�[�^
		std::vector<Vector3> vector3Data;	//Vector3�̒ǉ��f�[�^
	};

	//SObject�̃��j�[�N�|�C���^
	typedef std::unique_ptr<SObject> CObject;

	//SObject�ɑ΂��ċ��ʂ̏������s��
	void QueryObject(std::function<void(SObject& obj)>query)
	{
		for (auto& obj : m_objects)
		{
			query(obj);
		}
	}

	//.tkl�t�@�C����ǂݍ���œ��e��SObject�̉ϒ��z��m_objects�ɏ�������
	bool Load(const char* levelFilePath);
//private:
	//tkl�t�@�C���̃o�[�W����
	int m_tklVersion = 0;
	//�{�[���̐�
	int m_numBone = 0;
	//SObject�̉ϒ��z��
	std::vector<SObject> m_objects;
public:
	//�{�[���̐����擾
	int GetNumBone() { return m_numBone; }
};

