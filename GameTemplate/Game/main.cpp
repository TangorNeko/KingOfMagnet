#include "stdafx.h"
#include "system/system.h"

#include "ShowModel.h"
#include "ShowSprite.h"
#include "BackGround.h"
#include "CLevel.h"
#include "Mage.h"
#include "Knight.h"
#include "Tank.h"
#include "Ninja.h"
#include "TitleScene.h"
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

	//2画面描画モードをON
	//TODO:最初にタイトルの表示が必要なので後々OFFから始める。
	//GameObjectManager::GetInstance()->Set2ScreenMode(true);

	//ライトマネージャーのインスタンスを作成
	CLightManager::CreateInstance();

	//////////////////////////////////////////////////
	//TitleScene* title = NewGO<TitleScene>(0, "Title");

	//ステージのライトを作成
	prefab::CDirectionLight* stageLight = NewGO<prefab::CDirectionLight>(0);
	stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
	stageLight->SetColor({ 0.5f,0.5f,0.5f });


	//プレイヤー1を作成
	Tank* showm = NewGO<Tank>(0, "Player");
	showm->m_position = { 0.0f,0.0f,-500.0f };
	showm->m_playerNum = 0;
	showm->m_magPower = 2;

	//プレイヤー2を作成
	Ninja* showm2 = NewGO<Ninja>(0, "Player");
	showm2->m_position = { 0.0f,0.0f,500.0f };
	showm2->m_playerNum = 1;
	showm2->m_magPower = -2;
	showm2->m_toCamera = { 0.0f,100.0f,100.0f };
	
	//各プレイヤーに敵を渡す
	showm2->m_enemy = showm;
	showm->m_enemy = showm2;

	//ステージの表示
	NewGO<BackGround>(0, "background");
	//NewGO<ShowSprite>(1, "sprite");

	
	/*
	prefab::CLevel level;
	level.Init("Assets/modelData/yuka2.tkl",nullptr);
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

