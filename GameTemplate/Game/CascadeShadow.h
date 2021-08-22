#pragma once
class CascadeShadow
{
private:
	CascadeShadow() = default;
	~CascadeShadow() = default;

	static CascadeShadow* m_instance;

public:
	/**
	 * @brief カスケードシャドウクラスのインスタンスを作成
	*/
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new CascadeShadow;
		}
	}

	/**
	 * @brief カスケードシャドウクラスのインスタンスを削除
	*/
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/**
	 * @brief カスケードシャドウクラスのインスタンスを取得
	 * @return インスタンスのポインタ
	*/
	static CascadeShadow* GetInstance() { return m_instance; }

	/**
	 * @brief カスケードシャドウに使用するレンダーターゲットを初期化する
	*/
	void Init();

	/**
	 * @brief シャドウマップを描画する
	*/
	void DrawShadowMap();
	
	/**
	 * @brief シャドウマップのテクスチャを取得
	 * @param screenNo スクリーン番号
	 * @param areaNo 影のエリア番号
	 * @return シャドウマップのテクスチャ
	*/
	Texture& GetShaowMapTexture(int screenNo,int areaNo)
	{
		return m_shadowMaps[screenNo][areaNo].GetRenderTargetTexture();
	}

	Matrix* GetLVPCMatrix(int screenNo)
	{
		return m_lvpcMatrix[screenNo];
	}

private:
	enum shadowMapArea
	{
		enShort = 0,					//近距離
		enMedium = 1,					//中距離
		enLong = 2,						//遠距離
		enShadowMapAreaNum = 3			//シャドウマップのエリアの数
	};
	Camera m_lightCamera[2];			//ライトのカメラ
	RenderTarget m_shadowMaps[2][3];	//シャドウマップのレンダーターゲット
	Matrix m_lvpcMatrix[2][3] = { g_matIdentity };			//ライトビュープロジェクションクロップ行列
	float m_areaRangeTable[3] = { 0.0f,0.0f,0.0f };
};

