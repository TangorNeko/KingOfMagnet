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
	static void CreateInstence()
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

private:
	enum GBuffers						//G-Buffer�̎��
	{
		enAlbedo,						//�A���x�h
		enNormal,						//�@��
		enWorldPos,						//���[���h���W
		enShadowColor,					//�V���h�E�J���[
		enGBufferNum,					//G-Buffer�̐�
	};
	RenderTarget m_rts[enGBufferNum];	//G-Buffer�̃����_�[�^�[�Q�b�g�̔z��

	Sprite m_deferredSprite;			//�f�B�t�@�[�h�����_�����O�̏o�̓X�v���C�g
};

