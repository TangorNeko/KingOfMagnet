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
	
	//�e���ɕ`���Ă��烂�f���ɕ`�����e��`�������̂Ő�ɃV���h�E�}�b�v������B
	PostEffectManager::GetInstance()->ShadowRender(rc);

	//shadow
	if (PostEffectManager::GetInstance()->GetShadowFlag())
	{
		rc.SetStep(RenderContext::eStep_RenderShadowMap);
		//ShadowRender�Ńr���[�|�[�g��ݒ肵�Ă���̂ł����Ńr���[�|�[�g�ݒ肵�Ȃ���OK(���Ԃ�)
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, CLightManager::GetInstance()->GetLightCamera());
			}
		}
	}
	PostEffectManager::GetInstance()->EndShadowRender(rc);

	//GBuffer�̍쐬///

	//�f�B�t�@�[�h�����_�����O�̏o�̓X�v���C�g���쐬///

	//�����_�[�^�[�Q�b�g��GBuffer�ɕύX///

	//GBuffer�ւ̏������݊J�n///

	//�|�X�g�G�t�F�N�g�p�BRender�O�̏���
	//(�f�B�t�@�[�h�ɍ��킹�ă|�X�g�G�t�F�N�g�̓f�B�t�@�[�h�����_�����O�̃X�v���C�g�o�͑O�Ɉړ� ? )///
	PostEffectManager::GetInstance()->BeforeRender(rc);

	if (m_2screenMode)//2��ʃ��[�h
	{
		//2��ʂ̃X�v���C�g�̃A�X�y�N�g��ɍ��킹��B
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth() / 2.0f);

		//1P��
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
			//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
			CLightManager::GetInstance()->UpdateEyePos(0);
			g_camera3D[0]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[0]);
				}
			}

			//1��ʖڃG�t�F�N�g�X�V
			EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(),0);
			//1��ʖڃG�t�F�N�g�`��
			EffectEngine::GetInstance()->Draw(0);
		}

		//2P��
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
			//2P���̉�ʂ̃J������2P�̃J����(g_camera3D[1])
			CLightManager::GetInstance()->UpdateEyePos(1);
			g_camera3D[1]->SetAspect(2);
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					go->RenderWrapper(rc, g_camera3D[1]);
				}
			}

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

		rc.SetStep(RenderContext::eStep_RenderViewport1);
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.SetViewport(viewport);
		//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
		CLightManager::GetInstance()->UpdateEyePos(0);
		g_camera3D[0]->SetAspect(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[0]);
			}
		}

		//1��ʃI�����[�̃G�t�F�N�g�X�V
		EffectEngine::GetInstance()->Update(GameTime::GetInstance().GetFrameDeltaTime(),0);
		//1��ʃI�����[�̃G�t�F�N�g�`��
		EffectEngine::GetInstance()->Draw(0);
	}

	//������G-Buffer�ւ̏������ݏI��///

	//�|�X�g�G�t�F�N�g�͂�������X�^�[�g?///

	//GBuffer�����ƂɃf�B�t�@�[�h�����_�����O�X�v���C�g���o��///

	//�o�͂��ꂽ�X�v���C�g�����ƂɃ|�X�g�G�t�F�N�g///
	
	//�S��ʕ\���̃X�v���C�g�p
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
	
	//�|�X�g�G�t�F�N�g�p�BRender��̏���
	//(�f�B�t�@�[�h�����_�����O�̂��ߏ�Ɉړ�?///)
	//���������̃I�u�W�F�N�g���ǉ����ꂽ�ꍇ�t�H���[�h�����_�����O��҂��Č�Ɏ��s����邱�ƂɂȂ邩���B///
	PostEffectManager::GetInstance()->AfterRender(rc);

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
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			rc.SetViewport(viewport);
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
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.TopLeftY = 0;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			rc.SetViewport(viewport);
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
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.SetViewport(viewport);
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

		//2D�G�t�F�N�g�X�V
		EffectEngine::GetInstance()->Update2D(GameTime::GetInstance().GetFrameDeltaTime());
		//2D�G�t�F�N�g�`��
		EffectEngine::GetInstance()->Draw2D();
	}
}