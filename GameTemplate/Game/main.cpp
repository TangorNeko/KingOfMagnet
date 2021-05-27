#include "stdafx.h"
#include "system/system.h"

#include "TitleScene.h"
#include "SampleScene.h"
#include "ResultScene.h"
#include "GameScene.h"
#include "Player.h"
#include "TransitionGenerator.h"

void PreLoad();
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
	CLightManager::GetInstance()->SetLightCameraPosition({ 500.0f,500.0f,-500.0f });
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
	
	//���f���f�[�^�A�G�t�F�N�g�f�[�^�̎��O���[�h�B
	PreLoad();

	TransitionGenerator* tg = NewGO<TransitionGenerator>(5, "TransitionGenerator");

	//�g�����W�V����
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::Circle, 60, false);

	//�^�C�g���V�[�����쐬�B
	TitleScene* title = NewGO<TitleScene>(6, "titlescene");

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

//�g�p���郂�f���A�G�t�F�N�g�̎��O���[�h�A
//TODO:�ォ��csv�t�@�C�����ɂ܂Ƃ߂ĘA�z�z��ȂǂŎg�p�ł���悤�ɂ�������...
void PreLoad()
{
	//���f���̎��O���[�h
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/Player1.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/Player2.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/TitleCylinder.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/pipe.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/gear.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/neji.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/KnightBullet.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/Gravity.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/repulsion.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/stage00.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/SkyBoard.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/Grenade.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/FlashGrenade.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/SmokeGrenade.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/scrap.tkm");
	prefab::CSkinModelRender::PreLoadModel("Assets/modelData/Gravity2.tkm");

	//�G�t�F�N�g�̎��O���[�h
	prefab::CEffect::PreLoadEffect(u"Assets/effect/�M��.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/����.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/���͒e.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/���͒e2.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/Blackhole3.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/�Ĉ�.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/�˗�.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/����.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/�_���[�W.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/�L���s�[��.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/�˗̓`���[�W.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/���̓`���[�W.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/���͒e����.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/�˗͒e����.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/���̓o�[�X�g.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/�˗̓o�[�X�g.efk");
}
