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
	void Init(bool bloomMode);

	/// <summary>
	/// �����_�����O�O�̏���
	/// </summary>
	void BeforeRender(RenderContext& rc);

	/// <summary>
	/// �����_�����O��̏���
	/// </summary>
	/// <param name="rc"></param>
	void AfterRender(RenderContext& rc);

private:
	bool m_bloomMode = false;
	RenderTarget m_mainRenderTarget;
	RenderTarget m_luminnceRenderTarget;
	Sprite m_testSprite;
	Sprite m_luminanceSprite;
	Sprite m_finalSprite;
	GaussianBlur m_gaussianBlur;
};

