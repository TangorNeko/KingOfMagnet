#include "stdafx.h"
#include "CDirectionLight.h"

namespace prefab
{
	CDirectionLight::~CDirectionLight()
	{
		CLightManager::GetInstance()->DirectionLightMinus();
	}

	bool CDirectionLight::Start()
	{
		/*
		//ライトのデータの構造体のサイズを確保する
		cb.Init(sizeof(LigData),nullptr);
		//定数バッファを登録
		//TODO:複数ライトがあってもずっとレジスタb1で行けるのか?要詳細
		ds.RegistConstantBuffer(1, cb);
		//登録を確定
		ds.Commit();

		ligData.ligDir = { 1.0f,0.0f,0.0f };
		ligData.ligColor = { 1.0f,1.0f,1.0f };
		*/

		m_lightNum = CLightManager::GetInstance()->AddDirectionLight(GetLigData());

		return true;
	}

	void CDirectionLight::Update()
	{
		/*
		//定数バッファを作成
		ConstantBuffer cb;

		//ライトのデータの構造体のサイズを確保する
		cb.Init(sizeof(LigData));

		//ディスクリプタヒープを作成
		DescriptorHeap ds;
		//定数バッファを登録
		//TODO:複数ライトがあってもずっとレジスタb1で行けるのか?要詳細
		ds.RegistConstantBuffer(1, cb);
		//登録を確定
		ds.Commit();
		*/


		/*
		//実際のライトのデータを転送
		cb.CopyToVRAM(ligData);

		auto& RenderContext = g_graphicsEngine->GetRenderContext();
		RenderContext.SetDescriptorHeap(ds);
		*/

		dirLigData.eyePos = g_camera3D->GetPosition();
		
		CLightManager::GetInstance()->UpdateDirectionLight(m_lightNum,GetLigData());
	}
}