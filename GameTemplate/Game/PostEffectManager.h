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
	/// 各種設定をオンにするかどうかを入力、現在はブルームのみ。
	/// </summary>
	/// <param name="bloomMode">ブルームをオンにするか?</param>
	void Init(bool bloomMode,bool shadowMode);

	/// <summary>
	/// レンダリング前の処理
	/// </summary>
	void BeforeRender(RenderContext& rc);

	/// <summary>
	/// レンダリング後の処理
	/// </summary>
	/// <param name="rc"></param>
	void AfterRender(RenderContext& rc);

	void ShadowRender(RenderContext& rc);

	void EndShadowRender(RenderContext& rc);
private:
	bool m_bloomMode = false;
	RenderTarget m_mainRenderTarget;
	RenderTarget m_luminnceRenderTarget;
	Sprite m_luminanceSprite;//輝度抽出したスプライト。
	Sprite m_bokeLuminanceSprite;//輝度抽出したものにブラーをかけたスプライト。
	Sprite m_copyToFrameBufferSprite;//画面に表示する最終結果のスプライト。
	GaussianBlur m_gaussianBlur;

public:
	bool m_shadowMode = false;
	RenderTarget m_shadowMap;
	Camera testLightCamera;
};

