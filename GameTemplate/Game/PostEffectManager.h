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

	bool GetShadowFlag()
	{
		return m_shadowMode;
	}

	Texture& GetShadowMap()
	{
		return m_shadowMap.GetRenderTargetTexture();
	}

	Texture& GetBlurShadowMap()
	{
		return m_shadowBlur.GetBokeTexture();
	}

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
	RenderTarget m_mainRenderTarget;
	Sprite m_copyToFrameBufferSprite;//画面に表示する最終結果のスプライト。

	//ブルーム関連。
	bool m_bloomMode = true; //ブルームが有効か
	RenderTarget m_luminnceRenderTarget;
	Sprite m_luminanceSprite;//輝度抽出したスプライト。
	Sprite m_bokeLuminanceSprite;//輝度抽出したものにブラーをかけたスプライト。
	GaussianBlur m_gaussianBlur[4];//ブラー。

	bool m_shadowMode = true;//シャドウが有効か。
	RenderTarget m_shadowMap;
	GaussianBlur m_shadowBlur;
};

