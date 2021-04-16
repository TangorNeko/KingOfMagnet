#include "stdafx.h"
#include "system/system.h"

#include "ShowModel.h"
#include "ShowSprite.h"
#include "BackGround.h"
#include "CLevel.h"
#include "Mage.h"
#include "Knight.h"
#include "Tank.h"
#include "Ninja.h"
#include "TitleScene.h"

#include "SkyBoard.h"
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

	//TODO:�V���O���g���ł���K�v����?
	PostEffectManager::CreateInstance();

	//�u���[���t���O�A�V���h�E�t���O�̏���
	PostEffectManager::GetInstance()->Init(true,true);
	//////////////////////////////////////////////////

	//TitleScene* title = NewGO<TitleScene>(0, "Title");

	//�X�e�[�W�̃��C�g���쐬
	prefab::CDirectionLight* stageLight = NewGO<prefab::CDirectionLight>(0);
	stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
	stageLight->SetColor({ 0.5f,0.5f,0.5f });

	prefab::CLevelRender2D* level2D = NewGO<prefab::CLevelRender2D>(5);
	level2D->Init("Assets/Level2D/Level2DTest.casl", nullptr);

	//�v���C���[1���쐬
	Character_base* showm = NewGO<Mage>(0, "Player");
	showm->m_position = { 0.0f,0.0f,0.0f };
	showm->m_playerNum = 0;
	showm->m_magPower = 1;

	//�v���C���[2���쐬
	Character_base* showm2 = NewGO<Mage>(0, "Player");
	showm2->m_position = { 0.0f,0.0f,500.0f };
	showm2->m_playerNum = 1;
	showm2->m_magPower = -1;
	showm2->m_toCamera = { 0.0f,100.0f,100.0f };

	//�e�v���C���[�ɓG��n��
	showm2->m_enemy = showm;
	showm->m_enemy = showm2;
	//�A�C�e���������_���ɏo��������
	NewGO<PopRandItem>(0, "popranditem");

	//�X�e�[�W�̕\��
	NewGO<BackGround>(0, "background");

	//NewGO<ShowSprite>(1, "sprite");

	/*
	prefab::CLevel level;
	level.Init("Assets/modelData/yuka2.tkl",nullptr);
	*/

	//����쐬�B�L���[�u�}�b�v����Ȃ��Ă����̔|���B
	SkyBoard* sky = NewGO<SkyBoard>(0);

	
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
		
		//�|�X�g�G�t�F�N�g�p�BRender�O�̏���
		//PostEffectManager::GetInstance()->BeforeRender(renderContext);

		//Render�̓��f�����A�G�t�F�N�g���󂯂���̂�`�悷��
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//�|�X�g�G�t�F�N�g�p�BRender��̏���
		//PostEffectManager::GetInstance()->AfterRender(renderContext);

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

