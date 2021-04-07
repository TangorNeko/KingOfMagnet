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
#include "PopRandItem.h"
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

	//tkm�t�@�C���}�l�[�W���̃C���X�^���X���쐬
	tkmFileManager::CreateInstance();

	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();

	//////////////////////////////////////////////////

	//TitleScene* title = NewGO<TitleScene>(0, "Title");

	//�X�e�[�W�̃��C�g���쐬
	prefab::CDirectionLight* stageLight = NewGO<prefab::CDirectionLight>(0);
	stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
	stageLight->SetColor({ 0.5f,0.5f,0.5f });

	prefab::CLevelRender2D* level2D = NewGO<prefab::CLevelRender2D>(5);
	level2D->Init("Assets/Level2D/Level2DTest.casl", nullptr);

	//�v���C���[1���쐬
	Character_base* showm = NewGO<Knight>(0, "Player");
	showm->m_position = { 0.0f,0.0f,-500.0f };
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

	//�X�e�[�W�̕\��
	NewGO<BackGround>(0, "background");
	//NewGO<ShowSprite>(1, "sprite");

	/*
	prefab::CLevel level;
	level.Init("Assets/modelData/yuka2.tkl",nullptr);
	*/


	//�A�C�e���������_���ɏo��������
	NewGO<PopRandItem>(0, "popranditem");

	//�������_�����O�^�[�Q�b�g�e�X�g
	
	RenderTarget mainRT;
	mainRT.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	SpriteInitData spriteInitData1;
	spriteInitData1.m_textures[0] = &mainRT.GetRenderTargetTexture();
	spriteInitData1.m_fxFilePath = "Assets/shader/sprite.fx";
	spriteInitData1.m_width = 1280;
	spriteInitData1.m_height = 720;
	spriteInitData1.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	std::unique_ptr<Sprite> sprite1 = std::make_unique<Sprite>();
	sprite1->Init(spriteInitData1);

	SpriteInitData spriteInitData2;
	spriteInitData2.m_textures[0] = &mainRT.GetRenderTargetTexture();
	spriteInitData2.m_fxFilePath = "Assets/shader/postEffect.fx";
	spriteInitData2.m_width = 1280;
	spriteInitData2.m_height = 720;
	spriteInitData2.m_alphaBlendMode = AlphaBlendMode_Add;

	std::unique_ptr<Sprite> sprite2 = std::make_unique<Sprite>();
	sprite2->Init(spriteInitData2);
	
	//�������_�����O�^�[�Q�b�g�e�X�g

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
		
		//�������_�����O�^�[�Q�b�g�e�X�g
		/*
		renderContext.WaitUntilToPossibleSetRenderTarget(mainRT);
		renderContext.SetRenderTarget(mainRT.GetRTVCpuDescriptorHandle(), mainRT.GetDSVCpuDescriptorHandle());
		renderContext.ClearRenderTargetView(mainRT.GetRTVCpuDescriptorHandle(), mainRT.GetRTVClearColor());
		renderContext.ClearDepthStencilView(mainRT.GetDSVCpuDescriptorHandle(), mainRT.GetDSVClearValue());
		*/
		//�������_�����O�^�[�Q�b�g�e�X�g


		GameObjectManager::GetInstance()->ExecuteUpdate();
		//Render�̓��f�����A�G�t�F�N�g���󂯂���̂�`�悷��
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//�������_�����O�^�[�Q�b�g�e�X�g
		/*
		renderContext.WaitUntilFinishDrawingToRenderTarget(mainRT);

		renderContext.SetRenderTarget(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), g_graphicsEngine->GetCurrentFrameBuffuerDSV());
		
		sprite1->Draw(renderContext);
		sprite2->Draw(renderContext);
		*/
		//�������_�����O�^�[�Q�b�g�e�X�g

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
	tkmFileManager::DeleteInstance();

	//�T�E���h�G���W�����폜
	CSoundEngine::DeleteInstance();
	return 0;
}

