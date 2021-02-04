#include "stdafx.h"
#include "system/system.h"

#include "ShowModel.h"
#include "BackGround.h"

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

	//���C�g�}�l�[�W���[�̃C���X�^���X���쐬
	CLightManager::CreateInstance();
	
	/*
	prefab::CDirectionLight* bluefromup = NewGO<prefab::CDirectionLight>(0);
	bluefromup->SetDirection({ 0.0f,-1.0f,0.0f });
	bluefromup->SetColor({ 0.0f,0.0f,1.0f });
	*/

	/*
	prefab::CDirectionLight* greenfromdown = NewGO<prefab::CDirectionLight>(0);
	greenfromdown->SetDirection({ 0.0f,1.0f,0.0f });
	greenfromdown->SetColor({ 0.0f,1.0f,0.0f });
	*/

	prefab::CPointLight* bluepointlig = NewGO<prefab::CPointLight>(0);
	bluepointlig->SetPosition({ 100.0f,5.0f,0.0f });
	bluepointlig->SetColor({ 0.0f,0.0f,1.0f });
	bluepointlig->SetRange(100.0f);

	prefab::CPointLight* greenpointlig = NewGO<prefab::CPointLight>(0);
	greenpointlig->SetPosition({ -75.0f,5.0f,80.0f });
	greenpointlig->SetColor({ 0.0f,1.0f,0.0f });
	greenpointlig->SetRange(100.0f);

	NewGO<ShowModel>(0, "model");
	NewGO<BackGround>(0, "background");

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
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();

	CLightManager::DeleteInstance();
	return 0;
}

