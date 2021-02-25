#include "stdafx.h"
#include "system/system.h"

#include "ShowModel.h"
#include "BackGround.h"
#include "CLevel.h"

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

	prefab::CDirectionLight* redfromside = NewGO<prefab::CDirectionLight>(0);
	redfromside->SetDirection({ 1.0f,0.0f,0.0f });
	redfromside->SetColor({ 1.0f,0.0f,0.0f });

	ShowModel* showm = NewGO<ShowModel>(0, "model");
	//NewGO<BackGround>(0, "background");

	prefab::CLevel level;
	level.Init("Assets/modelData/Test.tkl",nullptr);

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

