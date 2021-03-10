#include "stdafx.h"
#include "system/system.h"

#include "ShowModel.h"
#include "ShowSprite.h"
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

	//2��ʕ`�惂�[�h��ON
	//TODO:�ŏ��Ƀ^�C�g���̕\�����K�v�Ȃ̂Ō�XOFF����n�߂�B
	GameObjectManager::GetInstance()->Set2ScreenMode(true);

	//���C�g�}�l�[�W���[�̃C���X�^���X���쐬
	CLightManager::CreateInstance();

	prefab::CDirectionLight* redfromside = NewGO<prefab::CDirectionLight>(0);
	redfromside->SetDirection({ 0.0f,-1.0f,0.0f });
	redfromside->SetColor({ 0.5f,0.5f,0.5f });

	ShowModel* showm = NewGO<ShowModel>(0, "Player");
	showm->m_position = { 0.0f,0.0f,-500.0f };
	showm->m_playerNum = 0;
	showm->m_magPower = 2;

	ShowModel* showm2 = NewGO<ShowModel>(0, "Player");
	showm2->m_position = { 0.0f,0.0f,0.0f };
	showm2->m_playerNum = 1;
	showm2->m_magPower = -2;
	showm2->m_enemy = showm;
	
	showm->m_enemy = showm2;


	NewGO<BackGround>(0, "background");
	//NewGO<ShowSprite>(1, "sprite");

	
	/*
	prefab::CLevel level;
	level.Init("Assets/modelData/yuka2.tkl",nullptr);
	*/
		

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		
		prefab::CSpotLight* a = nullptr;

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

