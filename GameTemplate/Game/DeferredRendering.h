#pragma once
class DeferredRendering
{
private:
	DeferredRendering() = default;
	~DeferredRendering() = default;

	static DeferredRendering* m_instance;
public:
	/**
	 * @brief �f�B�t�@�[�h�����_�����O�N���X�̃C���X�^���X���쐬
	*/
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new DeferredRendering;
		}
	}

	/**
	 * @brief �f�B�t�@�[�h�����_�����O�N���X�̃C���X�^���X���폜
	*/
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/**
	 * @brief �f�B�t�@�[�h�����_�����O�N���X�̃C���X�^���X���擾
	 * @return �C���X�^���X�̃|�C���^
	*/
	static DeferredRendering* GetInstance() { return m_instance; }


	/**
	 * @brief G-Buffer�Əo�̓X�v���C�g���쐬
	*/
	void Init();

	/**
	 * @brief �f�B�t�@�[�h�����_�����O�̊J�n����
	*/
	void StartDeferredRendering(RenderContext& rc);

	/**
	 * @brief �f�B�t�@�[�h�����_�����O�̏I������
	*/
	void EndDeferredRendering(RenderContext& rc);

	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescriptorHandle()
	{
		return m_rts[0].GetDSVCpuDescriptorHandle();
	}

private:
	enum GBuffers						//�ǉ������e�N�X�`���̎��
	{
		enAlbedo,						//�A���x�h
		enNormal,						//�@��
		enWorldPos,						//���[���h���W
		enGBufferNum,					//G-Buffer�̐�
	};

	enum ShadowTextures
	{
		enShadowmap_Near = 3,			//�V���h�E�}�b�v(�ߋ���)
		enShadowmap_Middle = 4,			//�V���h�E�}�b�v(������)
		enShadowmap_Far = 5,			//�V���h�E�}�b�v(������)
	};
	RenderTarget m_rts[enGBufferNum];	//G-Buffer�̃����_�[�^�[�Q�b�g�̔z��

	Sprite m_deferredSprite;			//�f�B�t�@�[�h�����_�����O�̏o�̓X�v���C�g
};

