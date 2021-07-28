#include "stdafx.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOption.h"
#include "RoundCounter.h"

namespace
{
	const Vector3 MODEL_TITLEBACKGROUND_POSITION = { 60.0f,-300.0f,-75.0f };				//�w�i�̓����f���̍��W
	const Vector3 MODEL_TITLEBACKGROUND_SCALE = { 1.2f,1.2f ,1.2f };						//�w�i�̓����f���̊g�嗦
	const float MODEL_TITLEBACKGROUND_ROTATIONRATE = -0.2f;									//�w�i�̓����f���̉�]��

	const float POINTLIGHT_TITLEBACKGROUND_RANGE = 1500.0f;									//�w�i�̓����f�����Ƃ炷�|�C���g���C�g�̉e���͈�
	const Vector3 POINTLIGHT_COLOR_WHITE = { 1.0f,1.0f,1.0f };								//�w�i�̓����f�����Ƃ炷�|�C���g���C�g�̐F

	const Vector3 SPRITE_MONITOR_POSITION = { -265.0f,-20.0f,0.0f };						//���j�^�[�X�v���C�g�̍��W
	const int SPRITE_MONITOR_WIDTH = 752;													//���j�^�[�X�v���C�g�̕�
	const int SPRITE_MONITOR_HEIGHT = 620;													//���j�^�[�X�v���C�g�̍���

	const Vector3 SPRITE_MONITORLINE_POSITION = { -290.0f,-21.0f,0.0f };					//���j�^�[���C���X�v���C�g�̍��W
	const int SPRITE_MONITORLINE_WIDTH = 704;												//���j�^�[���C���X�v���C�g�̕�
	const int SPRITE_MONITORLINE_HEIGHT = 560;												//���j�^�[���C���X�v���C�g�̍���

	const Vector3 SPRITE_TITLELOGO_POSITION = { -320.0f,10.0f,0.0f };						//�^�C�g�����S�̍��W
	const int SPRITE_TITLELOGO_WIDTH = 660;													//�^�C�g�����S�̕�
	const int SPRITE_TITLELOGO_HEIGHT = 495;												//�^�C�g�����S�̍���

	const int SPRITE_MENUBAR_WIDTH = 500;													//���j���[�o�[�X�v���C�g�̕�
	const int SPRITE_MENUBAR_HEIGHT = 188;													//���j���[�o�[�X�v���C�g�̍���
	const Vector3 SPRITE_MENUBAR_SCALE_MOVINGTOBACK = { 0.5f,0.5f,1.0f };					//���j���[�o�[�����Ɉړ����鎞�̊g�嗦
	const Vector3 SPRITE_MENUBAR_SCALE_NOTSELECTED = { 0.7f,0.7f,1.0f };					//���j���[�o�[���I������Ă��Ȃ����̊g�嗦
	const Vector3 SPRITE_MENUBAR_SCALE_SELECTED = { 1.0f,1.0f,1.0f };						//���j���[�o�[���I������Ă��鎞�̊g�嗦
	const Vector3 SPRITE_MENUBAR_POSITION_TOP = { 440.0f,200.0f,0.0f };						//���j���[�o�[����ɂ��鎞�̍��W
	const Vector3 SPRITE_MENUBAR_POSITION_CENTER = { 320.0f,0.0f,0.0f };					//���j���[�o�[���^�񒆂ɂ��鎞�̍��W
	const Vector3 SPRITE_MENUBAR_POSITION_BOTTOM = { 440.0f,-200.0f,0.0f };					//���j���[�o�[�����ɂ��鎞�̍��W
	const Vector3 SPRITE_MENUBAR_POSITION_CENTER_BACK = { 500.0f,0.0f,0.0f };				//���j���[�o�[������ʂ鎞�̐^�񒆂̍��W

	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT1 = { 50.0f,0.0f,1.0f };			//���ڑI�����̃A�j���[�V����1�̈ړ���
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT2 = { -125.0f,0.0f,1.0f };			//���ڑI�����̃A�j���[�V����2�̈ړ���
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT3 = { 20.0f,0.0f,1.0f };			//���ڑI�����̃A�j���[�V����3�̈ړ���
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT1 = { -50.0f,0.0f,1.0f };	//���ڃL�����Z�����̃A�j���[�V����1�̈ړ���
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT2 = { 125.0f,0.0f,1.0f };	//���ڃL�����Z�����̃A�j���[�V����2�̈ړ���
	const Vector3 SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT3 = { -20.0f,0.0f,1.0f };	//���ڃL�����Z�����̃A�j���[�V����3�̈ړ���
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1 = 12;								//���ڑI�����̃A�j���[�V����1�̈ړ�����
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2 = 12;								//���ڑI�����̃A�j���[�V����2�̈ړ�����
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3 = 6;									//���ڑI�����̃A�j���[�V����3�̈ړ�����
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1 = 0;								//���ڑI�����̃A�j���[�V����1�̈ړ��f�B���C
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2 = 6;								//���ڑI�����̃A�j���[�V����2�̈ړ��f�B���C
	const int SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3 = 8;								//���ڑI�����̃A�j���[�V����3�̈ړ��f�B���C
	const int SPRITE_MENUBAR_MOVINGFRONT_MOVETIME = 24;										//���j���[�o�[���O�ʂ��ړ����鎞�̈ړ�����
	const int SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME = 12;									//���j���[�o�[������ʂ鎞�̈ړ�����
	const int SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME = 12;									//���j���[�o�[����납��o�Ă��鎞�̈ړ�����
	const int SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY = 1;									//���j���[�o�[����낪�o�Ă��鎞�̈ړ��f�B���C
	const int SPRITE_MENUBAR_MOVING_DELAY = 0;												//���j���[�o�[���ړ����鎞�̈ړ��f�B���C

	const Vector4 SPRITE_MENUBAR_COLOR_WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };					//���j���[�o�[�̏�Z�J���[�@��
	const Vector4 SPRITE_MENUBAR_COLOR_SEMITRANSPARENT = { 1.0f, 1.0f, 1.0f, 0.7f };		//���j���[�o�[�̏�Z�J���[�@������
	const Vector4 SPRITE_MENUBAR_COLOR_TRANSPARENT = { 1.0f, 1.0f, 1.0f, 0.0f };			//���j���[�o�[�̏�Z�J���[�@����

	const Vector3 SPRITE_ARROW_POSITION = { 600.0f,0.0f,0.0f };								//�I����X�v���C�g�̍��W
	const int SPRITE_ARROW_WIDTH = 48;														//�I����X�v���C�g�̕�
	const int SPRITE_ARROW_HEIGHT = 88;														//�I����X�v���C�g�̍���

	const Vector3 CAMERA_TITLE_POSITION = { 0.0f,50.0f,200.0f };							//�^�C�g���̃J�����̍��W
	const Vector3 CAMERA_TITLE_TARGETPOSITION = { 0.0f,50.0f,0.0f };						//�^�C�g���̃J�����̒����_

	const float SOUND_BGM_TITLE_VOLUME = 0.2f;												//�^�C�g��BGM�̃{�����[��
	const float SOUND_SE_SCROLL_VOLUME = 0.8f;												//���ڈړ�����SE�̃{�����[��
	const float SOUND_SE_SELECT_VOLUME = 1.2f;												//���ڑI������SE�̃{�����[��

	const int COMMANDTIMER_COMMANDSTART = 0;												//�R�}���h�^�C�}�[�@�X�^�[�g
	const int COMMANDTIMER_SPRITEMOVE_BACKTOFRONT = 12;										//�R�}���h�^�C�}�[�@����ʂ����X�v���C�g���o�Ă���
	const int COMMANDTIMER_PLAY_SELECT_SE = 20;												//�R�}���h�^�C�}�[�@�I��SE�𗬂�
	const int COMMANDTIME_SPRITEMOVE_FINISH = 30;											//�R�}���h�^�C�}�[�@�X�v���C�g�̈ړ�����
	const int COMMANDTIMER_START_TRANSITION = 40;											//�R�}���h�^�C�}�[�@�g�����W�V�����J�n
	const int COMMANDTIMER_EXECUTE_COMMAND = 60;											//�R�}���h�^�C�}�[�@�I�����ڂɉ��������������s
}

TitleScene::~TitleScene() 
{
	DeleteGO(m_BG_ModelRender);
	DeleteGO(m_BGLight);
	DeleteGO(m_Monitor_SpriteRender);
	DeleteGO(m_MonitorLine_SpriteRender);
	DeleteGO(m_Rogo_SpriteRender);
	DeleteGO(m_Option_SpriteRender);
	DeleteGO(m_Start_SpriteRender);
	DeleteGO(m_Exit_SpriteRender);
	DeleteGO(m_Arrow_SpriteRender);

	DeleteGO(m_option);
}
bool TitleScene::Start()
{	
	//�g�����W�V����
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::Circle, TRANSITION_TIME_NORMAL,true);
	//�w�i�̃��f��
	m_BG_ModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_BG_ModelRender->Init("Assets/modelData/TitleCylinder.tkm");
	m_BG_ModelRender->SetPosition(MODEL_TITLEBACKGROUND_POSITION);
	m_BG_ModelRender->SetScale(MODEL_TITLEBACKGROUND_SCALE);

	//�w�i�̃��f�����Ƃ炷�|�C���g���C�g
	m_BGLight = NewGO<prefab::CPointLight>(0);
	m_BGLight->SetRange(POINTLIGHT_TITLEBACKGROUND_RANGE);
	m_BGLight->SetPosition(Vector3::Zero);
	m_BGLight->SetColor(POINTLIGHT_COLOR_WHITE);

	//���j�^�[
	m_Monitor_SpriteRender =NewGO<prefab::CSpriteRender>(0);
	m_Monitor_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Monitor_SpriteRender->SetPosition(SPRITE_MONITOR_POSITION);
	m_Monitor_SpriteRender->Init("Assets/Image/Title_Monitor.dds", SPRITE_MONITOR_WIDTH, SPRITE_MONITOR_HEIGHT);

	//���j�^�[���C��
	m_MonitorLine_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_MonitorLine_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_MonitorLine_SpriteRender->SetPosition(SPRITE_MONITORLINE_POSITION);
	m_MonitorLine_SpriteRender->Init("Assets/Image/Title_MonitorLine.dds", SPRITE_MONITORLINE_WIDTH, SPRITE_MONITORLINE_HEIGHT);

	//�^�C�g�����S
	m_Rogo_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Rogo_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Rogo_SpriteRender->SetPosition(SPRITE_TITLELOGO_POSITION);
	m_Rogo_SpriteRender->Init("Assets/Image/Zikai_Rogo.dds", SPRITE_TITLELOGO_WIDTH, SPRITE_TITLELOGO_HEIGHT);

	//�I�v�V����
	m_Option_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Option_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Option_SpriteRender->SetPosition(SPRITE_MENUBAR_POSITION_TOP);
	m_Option_SpriteRender->Init("Assets/Image/Title_Option.dds", SPRITE_MENUBAR_WIDTH, SPRITE_MENUBAR_HEIGHT);
	m_Option_SpriteRender->SetMulColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT);
	m_Option_SpriteRender->SetScale(SPRITE_MENUBAR_SCALE_NOTSELECTED);

	//�n�߂�
	m_Start_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Start_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Start_SpriteRender->SetPosition(SPRITE_MENUBAR_POSITION_CENTER);
	m_Start_SpriteRender->Init("Assets/Image/Title_Start.dds", SPRITE_MENUBAR_WIDTH, SPRITE_MENUBAR_HEIGHT);
	
	//��߂�
	m_Exit_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Exit_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Exit_SpriteRender->SetPosition(SPRITE_MENUBAR_POSITION_BOTTOM);
	m_Exit_SpriteRender->Init("Assets/Image/Title_Exit.dds", SPRITE_MENUBAR_WIDTH, SPRITE_MENUBAR_HEIGHT);
	m_Exit_SpriteRender->SetMulColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT);
	m_Exit_SpriteRender->SetScale(SPRITE_MENUBAR_SCALE_NOTSELECTED);

	//���
	m_Arrow_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Arrow_SpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_Arrow_SpriteRender->SetPosition(SPRITE_ARROW_POSITION);
	m_Arrow_SpriteRender->Init("Assets/Image/Title_Arrow.dds", SPRITE_ARROW_WIDTH, SPRITE_ARROW_HEIGHT);

	//�J�����ʒu�̏�����
	g_camera3D[0]->SetPosition(CAMERA_TITLE_POSITION);
	g_camera3D[0]->SetTarget(CAMERA_TITLE_TARGETPOSITION);

	//BGM���Đ�
	m_titleBGM = NewGO<prefab::CSoundSource>(0);
	m_titleBGM->Init(L"Assets/sound/TitleBGM.wav", SoundType::enBGM);
	m_titleBGM->SetVolume(SOUND_BGM_TITLE_VOLUME);
	m_titleBGM->Play(true);	

	//�I�v�V�����͍ŏ��������Ă���
	//�I�v�V�����̗D��x�͏��Ȃ��Ƃ�TitleScene����ɂ��Ȃ���΁A���ڂ��甲�����ۂɓ����ɃI�v�V��������������Ă��܂�
	m_option = NewGO<GameOption>(10, "gameoption");
	return true;
}
void TitleScene::Update()
{
	//������]
	Quaternion titleBackGroundRot = m_BG_ModelRender->GetRotation();
	Vector3 AxisY = Vector3::AxisY;
	titleBackGroundRot.Apply(AxisY);
	titleBackGroundRot.SetRotationDeg(Vector3::AxisY, MODEL_TITLEBACKGROUND_ROTATIONRATE);
	m_BG_ModelRender->SetRotation(m_BG_ModelRender->GetRotation() * titleBackGroundRot);

	if (m_selectFlag == false) {

		//����
		if ((g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA)) && m_selectMoveFlag == false) {
			m_commandTimer = COMMANDTIMER_COMMANDSTART;
			m_selectFlag = true;
		}

		//�R�}���h�ړ� ��
		if ((g_pad[PAD_PLAYER1]->IsPress(enButtonUp) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp)) && m_selectMoveFlag == false) {
			m_selectMoveFlag = true;
			m_comandUpDown = false;
			m_commandTimer = COMMANDTIMER_COMMANDSTART;

			//SE
			SoundOneShotPlay(L"Assets/sound/CommandMove.wav", SOUND_SE_SCROLL_VOLUME);
		}
		//�R�}���h�ړ� ��
		if ((g_pad[PAD_PLAYER1]->IsPress(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonDown)) && m_selectMoveFlag == false) {
			m_selectMoveFlag = true;
			m_comandUpDown = true;
			m_commandTimer = COMMANDTIMER_COMMANDSTART;

			//SE
			SoundOneShotPlay(L"Assets/sound/CommandMove.wav", SOUND_SE_SCROLL_VOLUME);
		}

		if (m_selectMoveFlag == true) {
			//�R�}���h���o
			CommandMove();
		}

	}
	else {
		//���肵���̂ŃR�}���h���o
		CommandSelectMove();

		//�I�v�V�����̎��͖߂��悤�ɁB
		if ((g_pad[PAD_PLAYER1]->IsTrigger(enButtonB) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonB)) && m_titleCommand == TitleScene::TC_Option && m_option->GetSelectingState() == GameOption::enItem && m_option->isQueuing() == false) {
			m_commandTimer = COMMANDTIMER_COMMANDSTART;
			m_selectFlag = false;

			m_option->Close();

			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT1, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1, true);
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT2, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2, true);
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_REVERSEMOVEMENT3, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3, true);
		}
	}
}

void TitleScene::CommandMove() {

	if (m_commandTimer == COMMANDTIMER_COMMANDSTART) {
		if (m_comandUpDown == false) {
			//��
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			case TitleScene::TC_Option:
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			case TitleScene::TC_Exit:
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			}
		}
		else {
			//��
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			case TitleScene::TC_Option:
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			case TitleScene::TC_Exit:
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER_BACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_MOVINGTOBACK, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_TRANSPARENT, SPRITE_MENUBAR_MOVINGTOBACK_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				//
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_CENTER, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_SELECTED, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_WHITE, SPRITE_MENUBAR_MOVINGFRONT_MOVETIME, SPRITE_MENUBAR_MOVING_DELAY);
				break;
			}
		}
	}

	//�������������ڂ�������o�Ă��鋓��
	if (m_commandTimer == COMMANDTIMER_SPRITEMOVE_BACKTOFRONT) {
		//��
		if (m_comandUpDown == false) {
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Exit;
				break;
			case TitleScene::TC_Option:
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Start;
				break;
			case TitleScene::TC_Exit:
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_BOTTOM, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Option;
				break;
			}
		}
		else {
			//��
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Exit_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Option;
				break;
			case TitleScene::TC_Option:
				m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Start_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Exit;
				break;
			case TitleScene::TC_Exit:
				m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_POSITION_TOP, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteScale(SPRITE_MENUBAR_SCALE_NOTSELECTED, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_Option_SpriteRender->GetSpriteSupporter().SpriteColor(SPRITE_MENUBAR_COLOR_SEMITRANSPARENT, SPRITE_MENUBAR_MOVINGTOFRONT_MOVETIME, SPRITE_MENUBAR_MOVINGTOFRONT_MOVEDELAY);
				m_titleCommand = TC_Start;
				break;
			}
		}
	}

	m_commandTimer++;

	if (m_commandTimer >= COMMANDTIME_SPRITEMOVE_FINISH) {
		//�R�}���h���o�I��
		m_selectMoveFlag = false;
	}
}

void TitleScene::CommandSelectMove() {

	if (m_commandTimer == 0) {
		switch (m_titleCommand)
		{
		case TitleScene::TC_Start:
			//�J�n
			m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT1, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1, true);
			m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT2, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2, true);
			m_Start_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT3, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3, true);
			break;
		case TitleScene::TC_Option:
			//�ݒ�
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT1, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1, true);
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT2, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2, true);
			m_Option_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT3, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3, true);
			break;
		case TitleScene::TC_Exit:
			//�I��
			m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT1, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME1, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY1, true);
			m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT2, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME2, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY2, true);
			m_Exit_SpriteRender->GetSpriteSupporter().SpriteMove(SPRITE_MENUBAR_SELECTANIMATION_MOVEMENT3, SPRITE_MENUBAR_SELECTANIMATION_MOVETIME3, SPRITE_MENUBAR_SELECTANIMATION_MOVEDELAY3, true);
			break;
		}

	}

	m_commandTimer++;

	if (m_commandTimer == COMMANDTIMER_PLAY_SELECT_SE)
	{
		//SE
		SoundOneShotPlay(L"Assets/sound/TitleSelect.wav", SOUND_SE_SELECT_VOLUME);
	}
	
	if (m_commandTimer == COMMANDTIMER_START_TRANSITION && m_titleCommand == TitleScene::TC_Start)
	{
		TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, false);
	}

	if (m_commandTimer == COMMANDTIMER_EXECUTE_COMMAND) {
		switch (m_titleCommand)
		{
		case TitleScene::TC_Start:
			//�J�n
			//BGM������
			DeleteGO(m_titleBGM);
			//switch���̒��Ő錾���邽�߂̃X�R�[�v
			{
				//�����̃��E���h�̌v�����J�n
				RoundCounter* roundCounter = NewGO<RoundCounter>(0, "roundcounter");
				roundCounter->SetMaxRound(m_option->GetRoundToWin());

				GameScene* gameScene = NewGO<GameScene>(0, "gamescene");
				gameScene->SetP1Sensitivity(m_option->GetP1Sensitivity());
				gameScene->SetP2Sensitivity(m_option->GetP2Sensitivity());
				gameScene->SetTimeLimit(m_option->GetTimeLimit());
			}
			DeleteGO(this);
			break;
		case TitleScene::TC_Option:
			//�ݒ�
			m_option->Open();
			break;
		case TitleScene::TC_Exit:
			//�I��
			exit(EXIT_SUCCESS);
			break;
		}
	}
}