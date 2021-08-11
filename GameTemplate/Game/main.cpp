#include "stdafx.h"
#include "system/system.h"

#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "Player.h"
#include "TransitionGenerator.h"
#include "GameOption.h"

namespace
{
	const Vector3 LIGHTCAMERA_POSITION = { 500.0f,500.0f,-500.0f };	//���C�g�J�����̍��W
	const Vector3 LIGHTCAMERA_TARGET = { 0.0f,0.0f,0.0f };			//���C�g�J�����̒����_
	const Vector3 LIGHTCAMERA_UP = { 1.0f,0.0f,0.0f };				//���C�g�J�����̏����
	const float LIGHTCAMERA_WIDTH = 2000.0f;						//���C�g�J�������ʂ���(���s�����Ȃ̂ŃJ�����͕��s���e)
	const float LIGHTCAMERA_HEIGHT = 2000.0f;						//���C�g�J�������ʂ�����
}

/**
 * @brief ���f���f�[�^���̎��O���[�h
*/
void PreLoad();

/**
 * @brief �e��}�l�[�W���[�̏�����
*/
void InitManager();

/**
 * @brief �Q�[�����[�v
 * @param renderContext �����_�[�R���e�L�X�g 
 * @param stopwatch FPS�Œ�Ɏg�p����X�g�b�v�E�H�b�`
*/
void GameLoop(RenderContext& renderContext,Stopwatch& stopwatch);

/**
 * @brief �e��}�l�[�W���[�̔j��
*/
void ReleaseManager();

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//�}�l�[�W���[�̏�����
	InitManager();

	//���f���f�[�^�A�G�t�F�N�g�f�[�^�̎��O���[�h�B
	PreLoad();

	//�Q�[���ݒ�̓ǂݍ���
	GameOption::ReadOption();

	//�g�����W�V�����W�F�l���[�^�[���쐬
	NewGO<TransitionGenerator>(5, "TransitionGenerator");

	//�g�����W�V����
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::Circle, TRANSITION_TIME_NORMAL, false);

	//�^�C�g���V�[�����쐬�B
	NewGO<TitleScene>(6, "titlescene");

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//FPS�Œ�p�X�g�b�v�E�H�b�`
	Stopwatch stopwatch;

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		GameLoop(renderContext, stopwatch);
	}

	//�}�l�[�W���[�̔j��
	ReleaseManager();

	return 0;
}

//�g�p���郂�f���A�G�t�F�N�g�̎��O���[�h�A
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
	prefab::CEffect::PreLoadEffect(u"Assets/effect/Flash.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/Explosion.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/GravityBullet1.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/GravityBullet2.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/Blackhole3.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/Burning.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/Repulsion.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/Gravity.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/Damage.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/SPGaugeMax.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/ReplusionCharge.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/GravityCharge.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/GravityShot.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/RepulsionShot.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/GravityBurst.efk");
	prefab::CEffect::PreLoadEffect(u"Assets/effect/RepulsionBurst.efk");
}

void InitManager()
{
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//2��ʕ`�惂�[�h��OFF����n�߂�B
	GameObjectManager::GetInstance()->Set2ScreenMode(false);

	//���C�g�}�l�[�W���[�̃C���X�^���X���쐬
	CLightManager::CreateInstance();
	CLightManager::GetInstance()->SetLightCameraPosition(LIGHTCAMERA_POSITION);
	CLightManager::GetInstance()->SetLightCameraTarget(LIGHTCAMERA_TARGET);
	CLightManager::GetInstance()->SetLightCameraUp(LIGHTCAMERA_UP);
	CLightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	CLightManager::GetInstance()->SetLightCameraWidth(LIGHTCAMERA_WIDTH);
	CLightManager::GetInstance()->SetLightCameraHeight(LIGHTCAMERA_HEIGHT);

	//���\�[�X�}�l�[�W���̃C���X�^���X���쐬
	ResourceBankManager::CreateInstance();

	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();

	EffectEngine::CreateInstance();

	DeferredRendering::CreateInstance();
	DeferredRendering::GetInstance()->Init();

	PostEffectManager::CreateInstance();
	//�u���[���t���O�A�V���h�E�t���O�̏���
	PostEffectManager::GetInstance()->Init(true, true);
}

void GameLoop(RenderContext& renderContext, Stopwatch& stopwatch)
{
	//FPS�v���p�X�g�b�v�E�H�b�`�̌v���J�n
	stopwatch.Start();

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

	//�X�s�����b�N���s���B
	int restTime = 0;
	do {
		stopwatch.Stop();
		restTime = 16 - (int)stopwatch.GetElapsedMillisecond();
	} while (restTime > 0);


	//�X�g�b�v�E�H�b�`�̌v���I��
	stopwatch.Stop();
	//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���A�v�Z����
	GameTime::GetInstance().PushFrameDeltaTime((float)stopwatch.GetElapsed());
}

void ReleaseManager()
{
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
}
