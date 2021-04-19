#include "stdafx.h"
#include "system/system.h"

#include "TitleScene.h"

#include "GameScene.h"
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

	//2��ʕ`�惂�[�h��ON
	//TODO:�ŏ��Ƀ^�C�g���̕\�����K�v�Ȃ̂Ō�XOFF����n�߂�B
	GameObjectManager::GetInstance()->Set2ScreenMode(false);

	//���C�g�}�l�[�W���[�̃C���X�^���X���쐬
	CLightManager::CreateInstance();
	CLightManager::GetInstance()->SetLightCameraPosition({ -1000.0f,1000.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraTarget({ -1000.0f,0.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraUp({ 1.0f,0.0f,0.0f });
	CLightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	CLightManager::GetInstance()->SetLightCameraWidth(5000.0f);
	CLightManager::GetInstance()->SetLightCameraHeight(5000.0f);

	//���\�[�X�}�l�[�W���̃C���X�^���X���쐬
	ResourceBankManager::CreateInstance();

	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();

	PostEffectManager::CreateInstance();
	//�u���[���t���O�A�V���h�E�t���O�̏���
	PostEffectManager::GetInstance()->Init(true,true);
	//////////////////////////////////////////////////

//�^�C�g���V�[�����쐬�B
	TitleScene* title = NewGO<TitleScene>(0, "Title");

	
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

