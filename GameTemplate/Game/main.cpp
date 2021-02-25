#include "stdafx.h"
#include "system/system.h"

#include "ShowModel.h"
#include "BackGround.h"
#include "CLevel.h"

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

	prefab::CDirectionLight* redfromside = NewGO<prefab::CDirectionLight>(0);
	redfromside->SetDirection({ 1.0f,0.0f,0.0f });
	redfromside->SetColor({ 1.0f,0.0f,0.0f });

	ShowModel* showm = NewGO<ShowModel>(0, "model");
	//NewGO<BackGround>(0, "background");

	prefab::CLevel level;
	level.Init("Assets/modelData/Test.tkl",nullptr);

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

