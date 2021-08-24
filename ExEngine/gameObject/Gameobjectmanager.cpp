/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
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
	//���S�t���O�����Ă���Q�[���I�u�W�F�N�g��j������B
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
	
	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
	
	//�f�B�t�@�[�h�����_�����O���J�n
	rc.SetMode(RenderContext::enRenderMode_Deferred);

	//shadow
	if (PostEffectManager::GetInstance()->GetShadowFlag())
	{
		CascadeShadow::GetInstance()->DrawShadowMap();
	}


	//GBuffer�ւ̏������݊J�n///
	DeferredRendering::GetInstance()->StartDeferredRendering(rc);

	if (m_2screenMode)//2��ʃ��[�h
	{
		//2��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P��
		{
			rc.SetStep(RenderContext::eStep_RenderViewport1);
			ChangeViewPort(rc, 0);
			//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
			CLightManager::GetInstance()->UpdateEyePos(0);
			g_camera3D[0]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[0]);
				}
			}
		}

		//2P��
		{
			rc.SetStep(RenderContext::eStep_RenderViewport2);
			ChangeViewPort(rc, 1);
			//2P���̉�ʂ̃J������2P�̃J����(g_camera3D[1])
			CLightManager::GetInstance()->UpdateEyePos(1);
			g_camera3D[1]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[1]);
				}
			}
		}
	}
	else //1��ʃ��[�h
	{
		//1��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderViewport1);
		ChangeViewPort(rc, 2);
		//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
		CLightManager::GetInstance()->UpdateEyePos(0);
		g_camera3D[0]->SetAspect(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[0]);
			}
		}
	}

	//������G-Buffer�ւ̏������ݏI��///

	//�|�X�g�G�t�F�N�g�͂�������X�^�[�g?///
	PostEffectManager::GetInstance()->BeforeRender(rc);
	
	//GBuffer�����ƂɃf�B�t�@�[�h�����_�����O�X�v���C�g���o��///
	DeferredRendering::GetInstance()->EndDeferredRendering(rc);

	//�t�H���[�h�����_�����O�J�n

	rc.SetMode(RenderContext::enRenderMode_Forward);

	if (m_2screenMode)//2��ʃ��[�h
	{
		//2��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P��
		{
			rc.SetStep(RenderContext::eStep_RenderViewport1);
			ChangeViewPort(rc, 0);
			//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
			CLightManager::GetInstance()->UpdateEyePos(0);
			g_camera3D[0]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[0]);
				}
			}
		}

		//2P��
		{
			rc.SetStep(RenderContext::eStep_RenderViewport2);
			ChangeViewPort(rc, 1);
			//2P���̉�ʂ̃J������2P�̃J����(g_camera3D[1])
			CLightManager::GetInstance()->UpdateEyePos(1);
			g_camera3D[1]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[1]);
				}
			}
		}
	}
	else //1��ʃ��[�h
	{
		//1��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderViewport1);
		ChangeViewPort(rc, 2);
		//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
		CLightManager::GetInstance()->UpdateEyePos(0);
		g_camera3D[0]->SetAspect(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[0]);
			}
		}
	}
	
	//�f�B�t�@�[�h�X�v���C�g��`��
	ChangeViewPort(rc, 2);
	DeferredRendering::GetInstance()->DrawDeferred(rc);

	//�G�t�F�N�g�̕`��
	ExecuteEffectRender(rc);

	//�o�͂��ꂽ�X�v���C�g�����ƂɃ|�X�g�G�t�F�N�g///
	PostEffectManager::GetInstance()->AfterRender(rc);

	//�S��ʕ\���̃X�v���C�g�p
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
	if (m_2screenMode)//2��ʃ��[�h
	{
		//2��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P��
		{
			rc.SetStep(RenderContext::eStep_RenderViewport1);
			ChangeViewPort(rc, 0);
			//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
			CLightManager::GetInstance()->UpdateEyePos(0);
			g_camera3D[0]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->PostRenderWrapper(rc, g_camera3D[0]);
				}
			}
		}

		//2P��
		{
			rc.SetStep(RenderContext::eStep_RenderViewport2);
			ChangeViewPort(rc, 1);
			//2P���̉�ʂ̃J������2P�̃J����(g_camera3D[1])
			CLightManager::GetInstance()->UpdateEyePos(1);
			g_camera3D[1]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->PostRenderWrapper(rc, g_camera3D[1]);
				}
			}
		}
	}
	else //1��ʃ��[�h
	{
		//1��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderViewport1);
		ChangeViewPort(rc, 2);
		//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
		CLightManager::GetInstance()->UpdateEyePos(0);
		g_camera3D[0]->SetAspect(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->PostRenderWrapper(rc, g_camera3D[0]);
			}
		}
	}

	//�S��ʕ\���̃X�v���C�g�p�@
	{
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		rc.SetStep(RenderContext::eStep_RenderAllScreen);
		ChangeViewPort(rc, 2);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->PostRenderWrapper(rc, g_camera3D[0]);
			}
		}

		//2D�G�t�F�N�g�X�V
		EffectEngine::GetInstance()->Update2D(GameTime::GetInstance().GetFrameDeltaTime());
		//2D�G�t�F�N�g�`��
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
	if (m_2screenMode)//2��ʃ��[�h
	{
		//2��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P��
		{
			ChangeViewPort(rc, 0);

			//1��ʖڃG�t�F�N�g�X�V
			EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(), 0);
			//1��ʖڃG�t�F�N�g�`��
			EffectEngine::GetInstance()->Draw(0);
		}

		//2P��
		{
			ChangeViewPort(rc, 1);

			//2��ʖڃG�t�F�N�g�X�V
			EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(), 1);
			//2��ʖڃG�t�F�N�g�`��
			EffectEngine::GetInstance()->Draw(1);
		}
	}
	else //1��ʃ��[�h
	{
		//1��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()));

		ChangeViewPort(rc, 2);

		//1��ʃI�����[�̃G�t�F�N�g�X�V
		EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(), 0);
		//1��ʃI�����[�̃G�t�F�N�g�`��
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