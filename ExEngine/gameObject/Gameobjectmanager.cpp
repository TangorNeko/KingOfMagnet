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

	if (m_2screenMode)
	{
		//1P��
		{
			rc.SetStep(RenderContext::eStep_RenderViewport1);
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.Height = g_graphicsEngine->GetFrameBufferHeight();
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
		}

		//2P��
		{
			rc.SetStep(RenderContext::eStep_RenderViewport2);
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.TopLeftY = 0;
			viewport.Width = g_graphicsEngine->GetFrameBufferWidth() / 2.0f;
			viewport.Height = g_graphicsEngine->GetFrameBufferHeight();
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
		}
	}
	else
	{
		rc.SetStep(RenderContext::eStep_RenderViewport1);
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = g_graphicsEngine->GetFrameBufferWidth();
		viewport.Height = g_graphicsEngine->GetFrameBufferHeight();
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
	}
	
	
}