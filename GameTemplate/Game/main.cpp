#include "stdafx.h"
#include "system/system.h"

#include "TitleScene.h"
#include "SampleScene.h"

#include "GameScene.h"
#include "Player.h"
#include "TransitionGenerator.h"

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//2��ʕ`�惂�[�h��OFF����n�߂�B
	GameObjectManager::GetInstance()->Set2ScreenMode(false);

	//���C�g�}�l�[�W���[�̃C���X�^���X���쐬
	CLightManager::CreateInstance();
	CLightManager::GetInstance()->SetLightCameraPosition({ 0.0f,500.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraTarget({ 0.0f,0.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraUp({ 1.0f,0.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	CLightManager::GetInstance()->SetLightCameraWidth(2000.0f);
	CLightManager::GetInstance()->SetLightCameraHeight(2000.0f);

	//���\�[�X�}�l�[�W���̃C���X�^���X���쐬
	ResourceBankManager::CreateInstance();

	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();

	EffectEngine::CreateInstance();

	PostEffectManager::CreateInstance();
	//�u���[���t���O�A�V���h�E�t���O�̏���
	PostEffectManager::GetInstance()->Init(true,true);
	//////////////////////////////////////////////////

	TransitionGenerator* tg = NewGO<TransitionGenerator>(10, "TransitionGenerator");

	//�^�C�g���V�[�����쐬�B
	TitleScene* title = NewGO<TitleScene>(0, "titlescene");

	//SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
	
	

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////

#if 0
		if (g_pad[0]->IsTrigger(enButtonB)) {
			Player* pl = FindGO<Player>("Player");
			
			prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
			effect->Init(u"Assets/effect/���͒e.efk");
			effect->SetPosition(pl->m_position);
			effect->SetScale({ 10.0f, 10.0f, 10.0f });
			effect->Play();
		}
#endif
		GameObjectManager::GetInstance()->ExecuteUpdate();

		//Render�̓��f�����A�G�t�F�N�g���󂯂���̂�`�悷��
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//PostRender�̓X�v���C�g�A�t�H���g���A�G�t�F�N�g���󂯂Ȃ����̂�`�悷��
		GameObjectManager::GetInstance()->ExecutePostRender(renderContext);

		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();

	//���C�g�}�l�[�W���[���폜
	CLightManager::DeleteInstance();
	
	//tkmFileManager���폜
	ResourceBankManager::DeleteInstance();

	//�T�E���h�G���W�����폜
	CSoundEngine::DeleteInstance();
	
	//�|�X�g�G�t�F�N�g�}�l�[�W���[���폜
	PostEffectManager::DeleteInstance();
	return 0;
}

