#pragma once
#include "GaussianBlur.h"

class PostEffectManager
{
private:
	PostEffectManager() = default;
	~PostEffectManager() = default;

	static PostEffectManager* m_instance;
public:
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new PostEffectManager;
		}
	}

	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	static PostEffectManager* GetInstance() { return m_instance; }

	/// <summary>
	/// �e��ݒ���I���ɂ��邩�ǂ�������́A���݂̓u���[���̂݁B
	/// </summary>
	/// <param name="bloomMode">�u���[�����I���ɂ��邩?</param>
	void Init(bool bloomMode,bool shadowMode);

	/// <summary>
	/// �����_�����O�O�̏���
	/// </summary>
	void BeforeRender(RenderContext& rc);

	/// <summary>
	/// �����_�����O��̏���
	/// </summary>
	/// <param name="rc"></param>
	void AfterRender(RenderContext& rc);

	void ShadowRender(RenderContext& rc);

	void EndShadowRender(RenderContext& rc);
private:
	bool m_bloomMode = false;
	RenderTarget m_mainRenderTarget;
	RenderTarget m_luminnceRenderTarget;
	Sprite m_luminanceSprite;//�P�x���o�����X�v���C�g�B
	Sprite m_bokeLuminanceSprite;//�P�x���o�������̂Ƀu���[���������X�v���C�g�B
	Sprite m_copyToFrameBufferSprite;//��ʂɕ\������ŏI���ʂ̃X�v���C�g�B
	GaussianBlur m_gaussianBlur;

public:
	bool m_shadowMode = false;
	RenderTarget m_shadowMap;
	Camera testLightCamera;
};

