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

	/**
	 * @brief �f�B�t�@�[�h�����_�����O�̌��ʂ��烉�C�e�B���O�����X�v���C�g��`��
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void DrawDeferred(RenderContext& rc)
	{
		m_deferredSprite.Draw(rc);
	}

	/**
	 * @brief �f�B�t�@�[�h�����_�����O�Ɏg�p�����f�v�X�X�e���V���r���[���擾
	 * @return �f�v�X�X�e���V���r���[
	*/
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
		enShadowmap_Screen1_Near = 3,			//�V���h�E�}�b�v(�X�N���[��1�A�ߋ���)
		enShadowmap_Screen1_Middle,			//�V���h�E�}�b�v(�X�N���[��1�A������)
		enShadowmap_Screen1_Far,			//�V���h�E�}�b�v(�X�N���[��1�A������)
		enShadowmap_Screen2_Near,			//�V���h�E�}�b�v(�X�N���[��2�A�ߋ���)
		enShadowmap_Screen2_Middle,			//�V���h�E�}�b�v(�X�N���[��2�A������)
		enShadowmap_Screen2_Far,			//�V���h�E�}�b�v(�X�N���[��2�A������)
	};
	RenderTarget m_rts[enGBufferNum];	//G-Buffer�̃����_�[�^�[�Q�b�g�̔z��

	Sprite m_deferredSprite;			//�f�B�t�@�[�h�����_�����O�̏o�̓X�v���C�g
};

