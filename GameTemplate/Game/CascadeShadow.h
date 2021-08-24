#pragma once
class CascadeShadow
{
private:
	CascadeShadow() = default;
	~CascadeShadow() = default;

	static CascadeShadow* m_instance;

public:
	/**
	 * @brief �J�X�P�[�h�V���h�E�N���X�̃C���X�^���X���쐬
	*/
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new CascadeShadow;
		}
	}

	/**
	 * @brief �J�X�P�[�h�V���h�E�N���X�̃C���X�^���X���폜
	*/
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/**
	 * @brief �J�X�P�[�h�V���h�E�N���X�̃C���X�^���X���擾
	 * @return �C���X�^���X�̃|�C���^
	*/
	static CascadeShadow* GetInstance() { return m_instance; }

	/**
	 * @brief �J�X�P�[�h�V���h�E�Ɏg�p���郌���_�[�^�[�Q�b�g������������
	*/
	void Init();

	/**
	 * @brief �V���h�E�}�b�v��`�悷��
	*/
	void DrawShadowMap();
	
	/**
	 * @brief �V���h�E�}�b�v�̃e�N�X�`�����擾
	 * @param screenNo �X�N���[���ԍ�
	 * @param areaNo �e�̃G���A�ԍ�
	 * @return �V���h�E�}�b�v�̃e�N�X�`��
	*/
	Texture& GetShaowMapTexture(int screenNo,int areaNo)
	{
		return m_shadowMaps[screenNo][areaNo].GetRenderTargetTexture();
	}

	/**
	 * @brief �N���b�v���ꂽ���C�g�r���[�v���W�F�N�V�����s����擾
	 * @param screenNo �X�N���[���ԍ�
	 * @return �N���b�v���ꂽ���C�g�r���[�v���W�F�N�V�����s��
	*/
	Matrix* GetLVPCMatrix(int screenNo)
	{
		return m_lvpcMatrix[0];
	}

	/**
	 * @brief ���C�g�J�����̒����_��ݒ�
	 * @param screenNo �X�N���[���ԍ�
	 * @param targetPos �����_
	*/
	void SetLightCameraTarget(int screenNo, const Vector3& targetPos);

	/**
	 * @brief ���C�g�J�����̌�����ݒ�
	 * @param direction ����
	*/
	void SetLightCameraDirection(const Vector3& direction)
	{
		m_lightCameraDirection = direction;
		m_lightCameraDirection.Normalize();
	}

private:
	enum shadowMapArea
	{
		enNear = 0,					//�ߋ���
		enMedium = 1,				//������
		enLong = 2,					//������
		enShadowMapAreaNum = 3		//�V���h�E�}�b�v�̃G���A�̐�
	};
	Camera m_lightCamera[2];								//���C�g�̃J����
	Vector3 m_lightCameraDirection = { -1.0f,-1.0f,1.0f };	//�e�Ɏg�p���镽�s�����̌���
	RenderTarget m_shadowMaps[2][3];						//�V���h�E�}�b�v�̃����_�[�^�[�Q�b�g
	Matrix m_lvpcMatrix[2][3] = { g_matIdentity };			//���C�g�r���[�v���W�F�N�V�����N���b�v�s��
	float m_areaRangeTable[3] = { 0.0f,0.0f,0.0f };
};

