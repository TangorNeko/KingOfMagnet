#include "stdafx.h"
#include "system/system.h"

#include "TitleScene.h"

#include "GameScene.h"
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
	GameObjectManager::GetInstance()->Set2ScreenMode(false);

	//ライトマネージャーのインスタンスを作成
	CLightManager::CreateInstance();
	CLightManager::GetInstance()->SetLightCameraPosition({ -1000.0f,1000.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraTarget({ -1000.0f,0.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraUp({ 1.0f,0.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	CLightManager::GetInstance()->SetLightCameraWidth(5000.0f);
	CLightManager::GetInstance()->SetLightCameraHeight(5000.0f);

	//リソースマネージャのインスタンスを作成
	ResourceBankManager::CreateInstance();

	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();

	PostEffectManager::CreateInstance();
	//ブルームフラグ、シャドウフラグの順番
	PostEffectManager::GetInstance()->Init(true,true);
	//////////////////////////////////////////////////

//タイトルシーンを作成。
	TitleScene* title = NewGO<TitleScene>(0, "Title");

	
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

		//Renderはモデル等、エフェクトを受けるものを描画する
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

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

