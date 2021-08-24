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
	
	//ディファードレンダリングを開始
	rc.SetMode(RenderContext::enRenderMode_Deferred);

	//shadow
	if (PostEffectManager::GetInstance()->GetShadowFlag())
	{
		CascadeShadow::GetInstance()->DrawShadowMap();
	}


	//GBufferへの書き込み開始///
	DeferredRendering::GetInstance()->StartDeferredRendering(rc);

	if (m_2screenMode)//2画面モード
	{
		//2画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P側
		{
			rc.SetStep(RenderContext::eStep_RenderViewport1);
			ChangeViewPort(rc, 0);
			//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
			CLightManager::GetInstance()->UpdateEyePos(0);
			g_camera3D[0]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[0]);
				}
			}
		}

		//2P側
		{
			rc.SetStep(RenderContext::eStep_RenderViewport2);
			ChangeViewPort(rc, 1);
			//2P側の画面のカメラは2Pのカメラ(g_camera3D[1])
			CLightManager::GetInstance()->UpdateEyePos(1);
			g_camera3D[1]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[1]);
				}
			}
		}
	}
	else //1画面モード
	{
		//1画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderViewport1);
		ChangeViewPort(rc, 2);
		//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
		CLightManager::GetInstance()->UpdateEyePos(0);
		g_camera3D[0]->SetAspect(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[0]);
			}
		}
	}

	//ここでG-Bufferへの書き込み終了///

	//ポストエフェクトはここからスタート?///
	PostEffectManager::GetInstance()->BeforeRender(rc);
	
	//GBufferをもとにディファードレンダリングスプライトを出力///
	DeferredRendering::GetInstance()->EndDeferredRendering(rc);

	//フォワードレンダリング開始

	rc.SetMode(RenderContext::enRenderMode_Forward);

	if (m_2screenMode)//2画面モード
	{
		//2画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P側
		{
			rc.SetStep(RenderContext::eStep_RenderViewport1);
			ChangeViewPort(rc, 0);
			//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
			CLightManager::GetInstance()->UpdateEyePos(0);
			g_camera3D[0]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[0]);
				}
			}
		}

		//2P側
		{
			rc.SetStep(RenderContext::eStep_RenderViewport2);
			ChangeViewPort(rc, 1);
			//2P側の画面のカメラは2Pのカメラ(g_camera3D[1])
			CLightManager::GetInstance()->UpdateEyePos(1);
			g_camera3D[1]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[1]);
				}
			}
		}
	}
	else //1画面モード
	{
		//1画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderViewport1);
		ChangeViewPort(rc, 2);
		//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
		CLightManager::GetInstance()->UpdateEyePos(0);
		g_camera3D[0]->SetAspect(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[0]);
			}
		}
	}
	
	//ディファードスプライトを描画
	ChangeViewPort(rc, 2);
	DeferredRendering::GetInstance()->DrawDeferred(rc);

	//エフェクトの描画
	ExecuteEffectRender(rc);

	//出力されたスプライトをもとにポストエフェクト///
	PostEffectManager::GetInstance()->AfterRender(rc);

	//全画面表示のスプライト用
	{
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderAllScreen);
		ChangeViewPort(rc, 2);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[0]);
			}
		}
	}
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
			ChangeViewPort(rc, 0);
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
			ChangeViewPort(rc, 1);
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
		ChangeViewPort(rc, 2);
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
		ChangeViewPort(rc, 2);
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

void GameObjectManager::ExecuteShadowRender(RenderContext& rc, const Matrix& viewMatrix, const Matrix& projMatrix,int screenNo,int areaNo)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ShadowRenderWrapper(rc, viewMatrix,projMatrix,screenNo,areaNo);
		}
	}
}

void GameObjectManager::ExecuteEffectRender(RenderContext& rc)
{
	if (m_2screenMode)//2画面モード
	{
		//2画面のスプライトのアスペクト比に合わせる。
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P側
		{
			ChangeViewPort(rc, 0);

			//1画面目エフェクト更新
			EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(), 0);
			//1画面目エフェクト描画
			EffectEngine::GetInstance()->Draw(0);
		}

		//2P側
		{
			ChangeViewPort(rc, 1);

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

		ChangeViewPort(rc, 2);

		//1画面オンリーのエフェクト更新
		EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(), 0);
		//1画面オンリーのエフェクト描画
		EffectEngine::GetInstance()->Draw(0);
	}
}

void GameObjectManager:: ChangeViewPort(RenderContext& rc,int screenNo)
{
	D3D12_VIEWPORT viewport;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	switch(screenNo)
	{
	case 0:
		viewport.TopLeftX = 0;
		viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		break;
	case 1:
		viewport.TopLeftX = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
		viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		break;
	case 2:
		viewport.TopLeftX = 0;
		viewport.Width = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
	}



	rc.SetViewport(viewport);
}