/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_instance = this;
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{	
	//死亡フラグがついているゲームオブジェクトを破棄する。
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {	
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;	 
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}


	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UpdateWrapper();
		}
	}
}
void GameObjectManager::ExecuteRender(RenderContext& rc)
{
	
	//レンダラーを変更するならここを改造していくと良い。
	
	//影を先に描いてからモデルに描いた影を描き足すので先にシャドウマップをつくる。
	PostEffectManager::GetInstance()->ShadowRender(rc);

	//shadow
	if (PostEffectManager::GetInstance()->GetShadowFlag())
	{
		rc.SetStep(RenderContext::eStep_RenderShadowMap);
		//ShadowRenderでビューポートを設定しているのでここでビューポート設定しなくてOK(たぶん)
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, CLightManager::GetInstance()->GetLightCamera());
			}
		}
	}
	PostEffectManager::GetInstance()->EndShadowRender(rc);

	//GBufferの作成///

	//ディファードレンダリングの出力スプライトを作成///

	//レンダーターゲットをGBufferに変更///

	//GBufferへの書き込み開始///

	//ポストエフェクト用。Render前の処理
	//(ディファードに合わせてポストエフェクトはディファードレンダリングのスプライト出力前に移動 ? )///
	PostEffectManager::GetInstance()->BeforeRender(rc);

	if (m_2screenMode)//2画面モード
	{
		//2画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P側
		{
			rc.SetStep(RenderContext::eStep_RenderViewport1);
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			rc.SetViewport(viewport);
			//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
			CLightManager::GetInstance()->UpdateEyePos(0);
			g_camera3D[0]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[0]);
				}
			}

			//1画面目エフェクト更新
			EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(),0);
			//1画面目エフェクト描画
			EffectEngine::GetInstance()->Draw(0);
		}

		//2P側
		{
			rc.SetStep(RenderContext::eStep_RenderViewport2);
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.TopLeftY = 0;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			rc.SetViewport(viewport);
			//2P側の画面のカメラは2Pのカメラ(g_camera3D[1])
			CLightManager::GetInstance()->UpdateEyePos(1);
			g_camera3D[1]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[1]);
				}
			}

			//2画面目エフェクト更新
			EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(), 1);
			//2画面目エフェクト描画
			EffectEngine::GetInstance()->Draw(1);
		}
	}
	else //1画面モード
	{
		//1画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderViewport1);
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.SetViewport(viewport);
		//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
		CLightManager::GetInstance()->UpdateEyePos(0);
		g_camera3D[0]->SetAspect(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[0]);
			}
		}

		//1画面オンリーのエフェクト更新
		EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(),0);
		//1画面オンリーのエフェクト描画
		EffectEngine::GetInstance()->Draw(0);
	}

	//ここでG-Bufferへの書き込み終了///

	//ポストエフェクトはここからスタート?///

	//GBufferをもとにディファードレンダリングスプライトを出力///

	//出力されたスプライトをもとにポストエフェクト///
	
	//全画面表示のスプライト用
	{
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderAllScreen);
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.SetViewport(viewport);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[0]);
			}
		}
	}
	
	//ポストエフェクト用。Render後の処理
	//(ディファードレンダリングのため上に移動?///)
	//半透明等のオブジェクトが追加された場合フォワードレンダリングを待って後に実行されることになるかも。///
	PostEffectManager::GetInstance()->AfterRender(rc);

}

void GameObjectManager::ExecutePostRender(RenderContext& rc)
{
	if (m_2screenMode)//2画面モード
	{
		//2画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P側
		{
			rc.SetStep(RenderContext::eStep_RenderViewport1);
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			rc.SetViewport(viewport);
			//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
			CLightManager::GetInstance()->UpdateEyePos(0);
			g_camera3D[0]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->PostRenderWrapper(rc, g_camera3D[0]);
				}
			}
		}

		//2P側
		{
			rc.SetStep(RenderContext::eStep_RenderViewport2);
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.TopLeftY = 0;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			rc.SetViewport(viewport);
			//2P側の画面のカメラは2Pのカメラ(g_camera3D[1])
			CLightManager::GetInstance()->UpdateEyePos(1);
			g_camera3D[1]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->PostRenderWrapper(rc, g_camera3D[1]);
				}
			}
		}
	}
	else //1画面モード
	{
		//1画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderViewport1);
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.SetViewport(viewport);
		//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
		CLightManager::GetInstance()->UpdateEyePos(0);
		g_camera3D[0]->SetAspect(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->PostRenderWrapper(rc, g_camera3D[0]);
			}
		}
	}

	//全画面表示のスプライト用　
	{
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderAllScreen);
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.SetViewport(viewport);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->PostRenderWrapper(rc, g_camera3D[0]);
			}
		}

		//2Dエフェクト更新
		EffectEngine::GetInstance()->Update2D(GameTime::GetInstance().GetFrameDeltaTime());
		//2Dエフェクト描画
		EffectEngine::GetInstance()->Draw2D();
	}
}