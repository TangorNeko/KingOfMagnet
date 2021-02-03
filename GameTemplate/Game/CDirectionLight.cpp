#include "stdafx.h"
#include "CDirectionLight.h"

namespace prefab
{
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
		ligData[0].eyePos = g_camera3D->GetPosition();
		ligData[1].eyePos = g_camera3D->GetPosition();
	}
}