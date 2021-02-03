#include "stdafx.h"
#include "system/system.h"

#include "ShowModel.h"
#include "BackGround.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//ライトマネージャーのインスタンスを作成
	CLightManager::CreateInstance();
	
	prefab::CDirectionLight* bluefromup = NewGO<prefab::CDirectionLight>(0);
	bluefromup->SetDirection({ 0.0f,-1.0f,0.0f });
	bluefromup->SetColor({ 0.0f,0.0f,1.0f });

	/*
	prefab::CDirectionLight* greenfromdown = NewGO<prefab::CDirectionLight>(0);
	greenfromdown->SetDirection({ 0.0f,1.0f,0.0f });
	greenfromdown->SetColor({ 0.0f,1.0f,0.0f });
	*/

	NewGO<ShowModel>(0, "model");
	NewGO<BackGround>(0, "background");

	/*
	prefab::CDirectionLight* lig = NewGO<prefab::CDirectionLight>(0,"lig");
	lig->SetColor({ 0.0f,0.0f,0.0f });
	*/

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();

	CLightManager::DeleteInstance();
	return 0;
}

