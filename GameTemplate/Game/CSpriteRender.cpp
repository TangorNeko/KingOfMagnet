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

		initData.m_psEntryPoinFunc = "PSMain";

		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		initData.m_width = width;

		initData.m_height = height;

		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		initData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_sprite.Init(initData);
	}

	//�X�v���C�g�̃h���[�R�[��
	void CSpriteRender::PostRender(RenderContext& rc, Camera* camera)
	{
		if (m_drawScreen == AllScreen || rc.GetRenderStep() == m_drawScreen)
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