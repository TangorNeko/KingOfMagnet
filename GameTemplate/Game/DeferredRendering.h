#pragma once
class DeferredRendering
{
private:
	DeferredRendering() = default;
	~DeferredRendering() = default;

	static DeferredRendering* m_instance;
public:
	/**
	 * @brief ディファードレンダリングクラスのインスタンスを作成
	*/
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new DeferredRendering;
		}
	}

	/**
	 * @brief ディファードレンダリングクラスのインスタンスを削除
	*/
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/**
	 * @brief ディファードレンダリングクラスのインスタンスを取得
	 * @return インスタンスのポインタ
	*/
	static DeferredRendering* GetInstance() { return m_instance; }


	/**
	 * @brief G-Bufferと出力スプライトを作成
	*/
	void Init();

	/**
	 * @brief ディファードレンダリングの開始処理
	*/
	void StartDeferredRendering(RenderContext& rc);

	/**
	 * @brief ディファードレンダリングの終了処理
	*/
	void EndDeferredRendering(RenderContext& rc);

	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescriptorHandle()
	{
		return m_rts[0].GetDSVCpuDescriptorHandle();
	}

private:
	enum GBuffers						//追加されるテクスチャの種類
	{
		enAlbedo,						//アルベド
		enNormal,						//法線
		enWorldPos,						//ワールド座標
		enGBufferNum,					//G-Bufferの数
	};

	enum ShadowTextures
	{
		enShadowmap_Near = 3,			//シャドウマップ(近距離)
		enShadowmap_Middle = 4,			//シャドウマップ(中距離)
		enShadowmap_Far = 5,			//シャドウマップ(遠距離)
	};
	RenderTarget m_rts[enGBufferNum];	//G-Bufferのレンダーターゲットの配列

	Sprite m_deferredSprite;			//ディファードレンダリングの出力スプライト
};

