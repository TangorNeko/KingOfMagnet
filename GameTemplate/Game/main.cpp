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
#include "PopRandItem.h"
#include "PostEffectManager.h"
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
	GameObjectManager::GetInstance()->Set2ScreenMode(true);

	//ライトマネージャーのインスタンスを作成
	CLightManager::CreateInstance();

	//リソースマネージャのインスタンスを作成
	ResourceBankManager::CreateInstance();

	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();

	//TODO:シングルトンである必要ある?
	PostEffectManager::CreateInstance();

	//ブルームフラグ、シャドウフラグの順番
	PostEffectManager::GetInstance()->Init(true,true);
	//////////////////////////////////////////////////

	//TitleScene* title = NewGO<TitleScene>(0, "Title");

	//ステージのライトを作成
	prefab::CDirectionLight* stageLight = NewGO<prefab::CDirectionLight>(0);
	stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
	stageLight->SetColor({ 0.5f,0.5f,0.5f });

	prefab::CLevelRender2D* level2D = NewGO<prefab::CLevelRender2D>(5);
	level2D->Init("Assets/Level2D/Level2DTest.casl", nullptr);

	//プレイヤー1を作成
	Character_base* showm = NewGO<Knight>(0, "Player");
	showm->m_position = { 0.0f,0.0f,0.0f };
	showm->m_playerNum = 0;
	showm->m_magPower = 1;

	//プレイヤー2を作成
	Character_base* showm2 = NewGO<Mage>(0, "Player");
	showm2->m_position = { 0.0f,0.0f,500.0f };
	showm2->m_playerNum = 1;
	showm2->m_magPower = -1;
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


	//アイテムをランダムに出現させる
	NewGO<PopRandItem>(0, "popranditem");

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
		
		//ポストエフェクト用。Render前の処理
		PostEffectManager::GetInstance()->BeforeRender(renderContext);

		//Renderはモデル等、エフェクトを受けるものを描画する
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//ポストエフェクト用。Render後の処理
		PostEffectManager::GetInstance()->AfterRender(renderContext);

		//PostRenderはスプライト、フォント等、エフェクトを受けないものを描画する
		GameObjectManager::GetInstance()->ExecutePostRender(renderContext);

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();

	//ライトマネージャーを削除
	CLightManager::DeleteInstance();
	
	//tkmFileManagerを削除
	ResourceBankManager::DeleteInstance();

	//サウンドエンジンを削除
	CSoundEngine::DeleteInstance();
	
	//ポストエフェクトマネージャーを削除
	PostEffectManager::DeleteInstance();
	return 0;
}

