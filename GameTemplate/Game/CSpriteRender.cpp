#include "stdafx.h"
#include "CSpriteRender.h"

namespace prefab
{
	/// <summary>
	/// �X�v���C�g�̏������֐�
	/// </summary>
	/// <param name="spritePath">�X�v���C�g�t�@�C���̃p�X(.dds)</param>
	/// <param name="width">�X�v���C�g�̕�</param>
	/// <param name="height">�X�v���C�g�̍���</param>
	void CSpriteRender::Init(const char* spritePath,UINT width,UINT height)
	{
		SpriteInitData initData;

		initData.m_ddsFilePath[0] = spritePath;

		initData.m_vsEntryPointFunc = "VSMain";

		switch (m_spriteMode)
		{
		case prefab::CSpriteRender::Normal:
			initData.m_psEntryPoinFunc = "PSMain";
			break;
		case prefab::CSpriteRender::Transition:
			initData.m_psEntryPoinFunc = "PS_Transition";
			m_sprite.SetAlpha(0.0f);
			break;
		}

		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		initData.m_width = width;

		initData.m_height = height;

		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		initData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_sprite.Init(initData);

		//�X�v���C�g�T�|�[�^�[�Ɏ����̑��݂�`����
		m_spriteSupporter.SpriteRenderSetting(this);
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	void CSpriteRender::Update()
	{
		//�X�v���C�g�T�|�[�^�[�̍X�V
		m_spriteSupporter.SpriteSupporter_Update();
	}

	//�X�v���C�g�̃h���[�R�[��
	void CSpriteRender::Render(RenderContext& rc, Camera* camera)
	{
		if (rc.GetRenderStep() == m_drawScreen && m_isPostRender == false)
		{
			m_sprite.Draw(rc);
		}
	}

	//�X�v���C�g�̃h���[�R�[��
	void CSpriteRender::PostRender(RenderContext& rc, Camera* camera)
	{
		if (rc.GetRenderStep() == m_drawScreen && m_isPostRender == true)
		{
			m_sprite.Draw(rc);
		}
	}

	/// <summary>
	/// �X�v���C�g�̍��W��ݒ肷��B
	/// </summary>
	/// <param name="pos"></param>
	void CSpriteRender::SetPosition(Vector3 pos)
	{
		m_position = pos;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	/// <summary>
	/// �X�v���C�g�̉�]��ݒ肷��B
	/// </summary>
	/// <param name="qRot"></param>
	void CSpriteRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	//�X�v���C�g�̊g�嗦��ݒ肷��B
	void CSpriteRender::SetScale(Vector3 scale)
	{
		m_scale = scale;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	//�X�v���C�g�̃s�{�b�g��ݒ肷��B
	void CSpriteRender::SetPivot(Vector2 pivot)
	{
		m_pivot = pivot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}
}