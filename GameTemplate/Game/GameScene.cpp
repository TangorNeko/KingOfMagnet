#include "stdafx.h"
#include "GameScene.h"
#include "Player.h"
#include "BackGround.h"
#include "SkyBoard.h"
#include "Debris.h"
#include "DebrisBlock.h"
#include "Bomb.h"
#include "Repulsion.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include <random>

#include "RoundCounter.h"
#include "OpeningCamera.h"
#include "FinalHit.h"

namespace
{
	const Vector3 STAGELIGHT_DIRECTION = { -1.0f,-1.0f,1.0f };				//�X�e�[�W�ɂ���X�|�b�g���C�g�̌���
	const Vector3 STAGELIGHT_COLOR = { 0.8f,0.8f,0.8f };					//�X�|�b�g���C�g�̐F
	const Vector3 PLAYER1_STARTPOSITION = { 760.0f,0.0f,400.0f };			//�v���C���[1�̃X�^�[�g���W
	const Vector3 PLAYER2_STARTPOSITION = { -760.0f,0.0f,-400.0f };			//�v���C���[2�̃X�^�[�g���W
	const Vector3 STARTDIRECTION_PLAYER1_TOCAMERA = { 1.0f,0.0f,0.0f };		//�v���C���[1�̃X�^�[�g���̃J�����ւ̌���
	const Vector3 STARTDIRECTION_PLAYER2_TOCAMERA = { -1.0f,0.0f,0.0f };	//�v���C���[2�̃X�^�[�g���̃J�����ւ̌���
	const Vector3 STARTDIRECTION_PLAYER1_MODEL = { -1.0f,0.0f,0.0f };		//�v���C���[1�̃X�^�[�g���̃��f���̌���
	const Vector3 STARTDIRECTION_PLAYER2_MODEL = { 1.0f,0.0f,0.0f };		//�v���C���[2�̃X�^�[�g���̃��f���̌���
	const Vector3 SPRITE_DELIMITLINE_POSITION = { 0.0f,0.0f,0.0f };			//��ʕ������X�v���C�g�̍��W
	const int SPRITE_DELIMITELINE_WIDTH = 40;								//��ʕ������X�v���C�g�̕�
	const int SPRITE_DELIMITELINE_HEIGHT = 720;								//��ʕ������X�v���C�g�̍���
	const Vector3 SPRITE_HPCOVER_POSITION = { 0.0f,304.0f,0.0f };			//HP�o�[�̃J�o�[�̍��W
	const int SPRITE_HPCOVER_WIDTH = 1280;									//HP�o�[�̃J�o�[�̕�
	const int SPRITE_HPCOVER_HEIGHT = 112;									//HP�o�[�̃J�o�[�̍���
	const Vector3 SPRITE_TIMERBASE_POSITION = { 0.0f,300.0f,0.0f };			//�^�C�}�[�X�v���C�g�̍��W
	const int SPRITE_TIMERBASE_WIDTH = 272;									//�^�C�}�[�X�v���C�g�̕�
	const int SPRITE_TIMERBASE_HEIGHT = 120;								//�^�C�}�[�X�v���C�g�̍���
	const Vector3 SPRITE_STARTCOUNTDOWN_3_TOP_STARTPOSITION
				= { -900.0f,100.0f,0.0f };									//�J�E���g�_�E��3�̏�̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_3_MIDDLE_STARTPOSITION
				= { 900.0f,0.0f,0.0f };										//�^�񒆂̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_3_BOTTOM_STARTPOSITION
				= { -900.0f,-100.0f,0.0f };									//���̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_3_RIGHT_STARTPOSITION
				= { 50.0f,-600.0f,0.0f };									//�E�̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_2_TOP_STARTPOSITION
				= { 900.0f,100.0f,0.0f };									//�J�E���g�_�E��2�̏�̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_2_MIDDLE_STARTPOSITION
				= { -900.0f,0.0f,0.0f };									//�^�񒆂̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_2_BOTTOM_STARTPOSITION
				= { 900.0f,-100.0f,0.0f };									//���̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_2_RIGHT_STARTPOSITION
				= { 50.0f,-550.0f,0.0f };									//�E�̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_2_LEFT_STARTPOSITION
				= { -50.0f,550.0f,0.0f };									//���̖_�̏������W
	const Vector3 SPRITE_STARTCOUNTDOWN_1_STARTPOSITION
				= { 0.0f,600.0f,0.0f };										//�J�E���g�_�E��1�̖_�̏������W
	const int SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH = 100;				//�J�E���g�_�E���̉��_�̕�
	const int SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT = 24;				//�J�E���g�_�E���̉��_�̍���
	const int SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_WIDTH = 24;				//�J�E���g�_�E��3�̏c�_�̕�
	const int SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_HEIGHT = 200;				//�J�E���g�_�E��3�̏c�_�̍���
	const int SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_WIDTH = 24;				//�J�E���g�_�E��2�̏c�_�̕�
	const int SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_HEIGHT = 100;				//�J�E���g�_�E��2�̏c�_�̍���
	const int SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_WIDTH = 36;				//�J�E���g�_�E��1�̏c�_�̕�
	const int SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_HEIGHT = 200;				//�J�E���g�_�E��1�̏c�_�̍���
	const Vector3 SPRITE_FINALCOUNTDOWN_POSITION = { 0.0f,0.0f,0.0f };		//�I���J�E���g�_�E���̃X�v���C�g�̍��W
	const int SPRITE_FINALCOUNTDOWN_WIDTH = 300;							//�I���J�E���g�_�E���̃X�v���C�g�̕�
	const int SPRITE_FINALCOUNTDOWN_HEGIHT = 300;							//�I���J�E���g�_�E���̃X�v���C�g�̍���
	const Vector3 SPRITE_FINALCOUNTDOWN_SCALE_START = { 3.0f,3.0f,3.0f };	//�I���J�E���g�_�E���̃X�v���C�g�̏����g�嗦
	const float SPRITE_FINALCOUNTDOWN_SCALE_SHRINK = 1.0f;					//�I���J�E���g�_�E���̎��k�������̊g�嗦
	const float SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY = 0.8f;					//�I���J�E���g�_�E���̕\�����̊g�嗦
	const float SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR = 0.0f;				//�I���J�E���g�_�E���̏��Ŏ��̊g�嗦
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK = 6;					//�I���J�E���g�_�E���̎��k�̎���
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY = 24;					//�I���J�E���g�_�E���̕\���̎���
	const int SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR = 6;				//�I���J�E���g�_�E���̏��ł̎���
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK = 0;					//�I���J�E���g�_�E���̎��k�̃f�B���C
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY = 6;					//�I���J�E���g�_�E���̕\���̃f�B���C
	const int SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR = 30;				//�I���J�E���g�_�E���̏��ł̃f�B���C
	const Vector4 SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT
				= { 1.0f,1.0f,1.0f,0.0f };									//�I���J�E���g�_�E���X�v���C�g�̏�Z�J���[�@����
	const Vector4 SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT
				= { 1.0f,1.0f,1.0f,0.5f };									//�I���J�E���g�_�E���X�v���C�g�̏�Z�J���[�@������
	const int SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME = 6;					//�I���J�E���g�_�E���X�v���C�g�𓧖��ɂ��鎞��
	const int SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY = 30;					//�I���J�E���g�_�E���X�v���C�g�𓧖��ɂ���f�B���C
	const Vector3 SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_SINGLEDIGIT
				= { 0.0f, 315.0f, 0.0f };									//�������ԃX�v���C�g1������1�̈ʂ̍��W
	const Vector3 SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_DOUBLEDIGIT
				= { 27.0f, 315.0f, 0.0f };									//�������ԃX�v���C�g2������1�̈ʂ̍��W
	const Vector3 SPRITE_TIMELIMIT_POSITION_TENTHPLACE_OF_DOUBLEDIGIT
				= { -27.0f, 315.0f, 0.0f };									//�������ԃX�v���C�g2������10�̈ʂ̍��W
	const Vector3 SPRITE_TIMELIMIT_SCALE = { 0.25f,0.25f,1.0f };			//�������ԃX�v���C�g�̊g�嗦
	const int SPRITE_TIMELIMIT_WIDTH = 500;									//�������ԃX�v���C�g�̕�
	const int SPRITE_TIMELIMIT_HEIGHT = 500;								//�������ԃX�v���C�g�̍���
	const float SOUND_BGM_GAME_VOLUME = 0.3f;								//�Q�[��BGM�̉���
	const float SOUND_SE_STARTCOUNTDOWN_VOLUME = 0.8f;						//�J�E���g�_�E��SE�̃{�����[��
	const float SOUND_SE_STARTHORN_VOLUME = 0.8f;							//�X�^�[�g�̃z�[��SE�̃{�����[��
	const int GAMEENDTIMER_ROUNDCOUNTER_SHOW = 450;							//�Q�[���I�����@���E���h�擾�\���̃^�C�}�[
	const int GAMEENDTIMER_START_TRANSITION = 650;							//�Q�[���I�����@�g�����W�V�����X�^�[�g�̃^�C�}�[
	const int GAMEENDTIMER_GOTO_RESULT = 700;								//�Q�[���I�����@���U���g�V�[���Ɉڍs����^�C�}�[
	const int DRAWTIMER_START_TRANSITION = 45;								//�����������@�@�g�����W�V�����X�^�[�g�̃^�C�}�[
	const int DRAWTIMER_GOTO_REMATCH = 0;									//�����������@�@���̎����ֈڍs����^�C�}�[
	const Vector2 FONT_SKIP_POSITION = { 270.0f,-320.0f };					//�I�[�v�j���O�J�����̃X�L�b�v�t�H���g�̍��W
	const Vector2 FONT_SKIP_SCALE = { 0.6f,0.6f };							//�I�[�v�j���O�J�����̃X�L�b�v�t�H���g�̊g�嗦
	const Vector2 FONT_DRAW_POSITION = { -185.0f, 130.0f };					//���������t�H���g�̍��W
	const Vector2 FONT_DRAW_SCALE = { 2.0f, 2.0f };							//���������t�H���g�̊g�嗦
	const Vector4 FONT_SHADOWCOLOR = { 0,0,0,1 };							//�t�H���g�̉e�̐F
	const float FONT_SHADOWOFFSET = 2.0f;									//�t�H���g�̉e�̑���
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT
				= { 890.0f,0.0f };											//�X�^�[�g�J�E���g�_�E���@�E�ړ��̈ړ���
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT
				= { 10.0f,0.0f };											//�X�^�[�g�J�E���g�_�E���@�E�ړ�(��)�̈ړ���
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT
				= { -890.0f,0.0f };											//�X�^�[�g�J�E���g�_�E���@���ړ��̈ړ���
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT
				= { -10.0f,0.0f };											//�X�^�[�g�J�E���g�_�E���@���ړ�(��)�̈ړ���
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP
				= { 0.0f,590.0f };											//�X�^�[�g�J�E���g�_�E���@��ړ��̈ړ���
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP_SHORT
				= { 0.0f,10.0f };											//�X�^�[�g�J�E���g�_�E���@��ړ�(��)�̈ړ���
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM
				= { 0.0f,-590.0f };											//�X�^�[�g�J�E���g�_�E���@���ړ��̈ړ���
	const Vector2 SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM_SHORT
				= { 0.0f,-10.0f };											//�X�^�[�g�J�E���g�_�E���@���ړ�(��)�̈ړ���
	const int SPRITE_STARTCOUNTDOWN_MOVETIME_FAST = 12;						//�X�^�[�g�J�E���g�_�E���@�����ړ��̈ړ�����
	const int SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW = 48;						//�X�^�[�g�J�E���g�_�E���@�x���ړ��̈ړ�����
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE = 0;					//�X�^�[�g�J�E���g�_�E���@�g�ݗ��ĊJ�n�̃f�B���C	
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY = 12;					//�X�^�[�g�J�E���g�_�E���@�\���̃f�B���C
	const int SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE = 60;				//�X�^�[�g�J�E���g�_�E���@�����̃f�B���C
	const Vector3 SPRITE_START_POSITION = { 0.0f, 0.0f, 0.0f };				//�X�^�[�g�X�v���C�g�̍��W
	const Vector3 SPRITE_START_SCALE = { 1.5f, 1.5f, 1.5f };				//�X�^�[�g�X�v���C�g�̊g�嗦
	const int SPRITE_START_WIDTH = 324;										//�X�^�[�g�X�v���C�g�̕�
	const int SPRITE_START_HEIGHT = 64;										//�X�^�[�g�X�v���C�g�̍���
	const int NUMBER_TIMELIMIT_SPRITE_0 = 0;								//�������ԃX�v���C�g�̐��� 0
	const int NUMBER_TIMELIMIT_SPRITE_1 = 1;								//�������ԃX�v���C�g�̐��� 1
	const int NUMBER_TIMELIMIT_SPRITE_2 = 2;								//�������ԃX�v���C�g�̐��� 2
	const int NUMBER_TIMELIMIT_SPRITE_3 = 3;								//�������ԃX�v���C�g�̐��� 3
	const int NUMBER_TIMELIMIT_SPRITE_4 = 4;								//�������ԃX�v���C�g�̐��� 4
	const int NUMBER_TIMELIMIT_SPRITE_5 = 5;								//�������ԃX�v���C�g�̐��� 5
	const int NUMBER_TIMELIMIT_SPRITE_6 = 6;								//�������ԃX�v���C�g�̐��� 6
	const int NUMBER_TIMELIMIT_SPRITE_7 = 7;								//�������ԃX�v���C�g�̐��� 7
	const int NUMBER_TIMELIMIT_SPRITE_8 = 8;								//�������ԃX�v���C�g�̐��� 8
	const int NUMBER_TIMELIMIT_SPRITE_9 = 9;								//�������ԃX�v���C�g�̐��� 9


	//���ł��g�������Ȃ̂łǂ����ɕ���?
	const int NUMBER_MAGNET_STATUS = 2;
}

float GameScene::m_maxTimeLimit = 0;
float GameScene::m_P1Sensitivity;
float GameScene::m_P2Sensitivity;

GameScene::~GameScene()
{	
	DeleteGO(m_stageLight);
	DeleteGO(m_backGround);
	DeleteGO(m_sky);
	DeleteGO(m_player1);
	DeleteGO(m_player2);
	QueryGOs<DebrisBlock>("debrisblock",[](DebrisBlock* debrisblock)->bool
		{
			DeleteGO(debrisblock);
			return true;
		});

	QueryGOs<Bomb>("bomb", [](Bomb* bomb)->bool
		{
			DeleteGO(bomb);
			return true;
		});

	QueryGOs<Debris>("debris", [](Debris* debris)->bool
		{
			DeleteGO(debris);
			return true;
		});

	QueryGOs<Repulsion>("repulsion", [](Repulsion* repulsion)->bool
		{
			DeleteGO(repulsion);
			return true;
		});
}

bool GameScene::Start()
{
	//�X�e�[�W�̃��C�g���쐬
	m_stageLight = NewGO<prefab::CDirectionLight>(0);
	m_stageLight->SetDirection(STAGELIGHT_DIRECTION);
	//���C�g�J�����ɕ��s�����̌�����`����
	CascadeShadow::GetInstance()->SetLightCameraDirection(STAGELIGHT_DIRECTION);
	m_stageLight->SetColor(STAGELIGHT_COLOR);

	m_player1 = NewGO<Player>(0, "Player");
	m_player1->SetPosition(PLAYER1_STARTPOSITION);
	m_player1->SetPlayerNum(NUMBER_PLAYER1);

	std::random_device device;
	std::mt19937_64 rnd(device());

	//�v���C���[1�̎��͂̏�Ԃ������_���Ɍ���
	int mag = rnd() % NUMBER_MAGNET_STATUS;
	if (mag)
	{
		m_player1->SetMagnetState(MAGNETSTATE_REPULSION);
	}
	else
	{
		m_player1->SetMagnetState(MAGNETSTATE_GRAVITY);
	}
	m_player1->SetToCameraDirection(STARTDIRECTION_PLAYER1_TOCAMERA);
	m_player1->SetCameraFront(STARTDIRECTION_PLAYER1_MODEL);
	m_player1->SetSensitivity(m_P1Sensitivity);

	m_player2 = NewGO<Player>(0, "Player");
	m_player2->SetPosition(PLAYER2_STARTPOSITION);
	m_player2->SetPlayerNum(NUMBER_PLAYER2);

	//�v���C���[2�̎��͂̏�Ԃ������_���Ɍ���
	mag = rnd() % NUMBER_MAGNET_STATUS;
	if (mag)
	{
		m_player2->SetMagnetState(MAGNETSTATE_REPULSION);
	}
	else
	{
		m_player2->SetMagnetState(MAGNETSTATE_GRAVITY);
	}
	m_player2->SetToCameraDirection(STARTDIRECTION_PLAYER2_TOCAMERA);
	m_player2->SetCameraFront(STARTDIRECTION_PLAYER2_MODEL);
	m_player2->SetSensitivity(m_P2Sensitivity);

	//�e�v���C���[�ɓG��n��
	m_player2->SetEnemy(m_player1);
	m_player1->SetEnemy(m_player2);
	
	//�X�e�[�W�̍쐬
	m_backGround = NewGO<BackGround>(0, "background");

	//����쐬�B�L���[�u�}�b�v����Ȃ��Ă����̔|���B
	m_sky = NewGO<SkyBoard>(0);

	//���ׂč쐬���I���������2��ʂɂ���B
	GameObjectManager::GetInstance()->Set2ScreenMode(true);
	
	//��ʂ���؂��
	m_delimitLineSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_delimitLineSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_delimitLineSpriteRender->SetPosition(SPRITE_DELIMITLINE_POSITION);
	m_delimitLineSpriteRender->Init("Assets/Image/Waku.dds", SPRITE_DELIMITELINE_WIDTH, SPRITE_DELIMITELINE_HEIGHT);

	//HP�̃J�o�[
	m_HPCoverSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_HPCoverSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_HPCoverSpriteRender->SetPosition(SPRITE_HPCOVER_POSITION);
	m_HPCoverSpriteRender->Init("Assets/Image/HP_Cover.dds", SPRITE_HPCOVER_WIDTH, SPRITE_HPCOVER_HEIGHT);

	//�^�C�}�[�̘g
	m_TimerBaseSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_TimerBaseSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_TimerBaseSpriteRender->SetPosition(SPRITE_TIMERBASE_POSITION);
	m_TimerBaseSpriteRender->Init("Assets/Image/Timer_Base.dds", SPRITE_TIMERBASE_WIDTH, SPRITE_TIMERBASE_HEIGHT);

	//�X�L�b�v�t�H���g
	m_skipFontRender = NewGO<prefab::CFontRender>(4);
	m_skipFontRender->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);
	m_skipFontRender->SetPosition(FONT_SKIP_POSITION);
	m_skipFontRender->SetScale(FONT_SKIP_SCALE);
	m_skipFontRender->SetShadowFlag(true);
	m_skipFontRender->SetShadowColor(FONT_SHADOWCOLOR);
	m_skipFontRender->SetShadowOffset(FONT_SHADOWOFFSET);
	m_skipFontRender->SetText(L"PRESS A TO SKIP");

	//�J�E���g3
	m_startCountDown_3_Top = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Top->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_3_Top->SetPosition(SPRITE_STARTCOUNTDOWN_3_TOP_STARTPOSITION);
	m_startCountDown_3_Top->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_3_Middle = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Middle->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_3_Middle->SetPosition(SPRITE_STARTCOUNTDOWN_3_MIDDLE_STARTPOSITION);
	m_startCountDown_3_Middle->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_3_Bottom = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Bottom->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_3_Bottom->SetPosition(SPRITE_STARTCOUNTDOWN_3_BOTTOM_STARTPOSITION);
	m_startCountDown_3_Bottom->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_3_Right = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Right->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_3_Right->SetPosition(SPRITE_STARTCOUNTDOWN_3_RIGHT_STARTPOSITION);
	m_startCountDown_3_Right->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_3_VERTICALBAR_HEIGHT);
	//�J�E���g2
	m_startCountDown_2_Top = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Top->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Top->SetPosition(SPRITE_STARTCOUNTDOWN_2_TOP_STARTPOSITION);
	m_startCountDown_2_Top->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_2_Middle = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Middle->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Middle->SetPosition(SPRITE_STARTCOUNTDOWN_2_MIDDLE_STARTPOSITION);
	m_startCountDown_2_Middle->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_2_Bottom = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Bottom->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Bottom->SetPosition(SPRITE_STARTCOUNTDOWN_2_BOTTOM_STARTPOSITION);
	m_startCountDown_2_Bottom->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_HORIZONTALBAR_HEIGHT);
	//
	m_startCountDown_2_Right = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Right->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Right->SetPosition(SPRITE_STARTCOUNTDOWN_2_RIGHT_STARTPOSITION);
	m_startCountDown_2_Right->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_HEIGHT);
	//
	m_startCountDown_2_Left = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Left->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_2_Left->SetPosition(SPRITE_STARTCOUNTDOWN_2_LEFT_STARTPOSITION);
	m_startCountDown_2_Left->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_2_VERTICALBAR_HEIGHT);
	//�J�E���g1
	m_startCountDown_1 = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_1->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_startCountDown_1->SetPosition(SPRITE_STARTCOUNTDOWN_1_STARTPOSITION);
	m_startCountDown_1->Init("Assets/Image/Count/CountLine.dds", SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_WIDTH, SPRITE_STARTCOUNTDOWN_1_VERTICALBAR_HEIGHT);
	//�Ō�̃J�E���g
	m_finalCountDown = NewGO<prefab::CSpriteRender>(10);
	m_finalCountDown->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_finalCountDown->SetPosition(SPRITE_FINALCOUNTDOWN_POSITION);
	m_finalCountDown->Init("Assets/Image/count3.dds", SPRITE_FINALCOUNTDOWN_WIDTH, SPRITE_FINALCOUNTDOWN_HEGIHT);
	m_finalCountDown->SetMulColor(SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT);

	//�������Ԃ��Z�b�g
	m_timeLimit = m_maxTimeLimit;

	//�^�C�����~�b�g�̈ꌅ��
	int onesPlace = (int)m_timeLimit % 10;
	char onesPlacePathName[256];
	sprintf_s(onesPlacePathName, "Assets/Image/%d.dds", onesPlace);
	m_onesPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_onesPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_DOUBLEDIGIT);
	m_onesPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
	m_onesPlaceSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_onesPlaceSpriteRender->SetUseSpriteSupporterFlag(false);
	m_onesPlaceSpriteRender->Init(onesPlacePathName, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);

	//�^�C�����~�b�g�̓񌅖�
	int tensPlace = (int)m_timeLimit / 10;
	char tensPlacePathName[256];
	sprintf_s(tensPlacePathName, "Assets/Image/%d.dds", tensPlace);
	m_tensPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_tensPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_TENTHPLACE_OF_DOUBLEDIGIT);
	m_tensPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
	m_tensPlaceSpriteRender->SetDrawScreen(prefab::CSpriteRender::DrawScreen::AllScreen);
	m_tensPlaceSpriteRender->SetUseSpriteSupporterFlag(false);
	m_tensPlaceSpriteRender->Init(tensPlacePathName, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);

	//�����Đ�
	m_gameBGM = NewGO<prefab::CSoundSource>(0);
	m_gameBGM->Init(L"Assets/sound/GameBGM.wav", SoundType::enBGM);
	m_gameBGM->SetVolume(SOUND_BGM_GAME_VOLUME);
	m_gameBGM->Play(true);
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, true);

	m_roundCounter = FindGO<RoundCounter>("roundcounter");
	m_roundCounter->MoveToGamePosition();
	return true;
}

void GameScene::Update()
{
	if (m_gameState == enBirdseye)
	{
		if (m_openingCameraPlayer1 == nullptr) 
		{
			m_openingCameraPlayer1 = NewGO<OpeningCamera>(0);
			m_openingCameraPlayer2 = NewGO<OpeningCamera>(0);
			m_openingCameraPlayer1->SetPlayerNum(0);
			m_openingCameraPlayer2->SetPlayerNum(1);
			m_openingCameraPlayer1->SetCameraFront(g_camera3D[0]->GetForward());
			m_openingCameraPlayer2->SetCameraFront(g_camera3D[1]->GetForward());
			m_openingCameraPlayer1->SetPlayerPos(m_player1->GetPosition());
			m_openingCameraPlayer2->SetPlayerPos(m_player2->GetPosition());
		}
		m_roundCounter->RoundAnnounce();
	}

	//�X�^�[�g�J�E���g�_�E��
	if (m_gameState == enStartCountDown) {
		if (m_isOpeningCameraOn == true)
		{
			DeleteGO(m_openingCameraPlayer1);
			DeleteGO(m_openingCameraPlayer2);
			m_isOpeningCameraOn = false;
		}
		StartCountDown();
		return;
	}
	TimeLimitCount();

	//���Ԑ؂�3�b�O����J�E���g�_�E����\������
	if (m_timeLimit <= 4 && m_timeLimit >= 0 && m_gameState != enResult && m_gameState != enDraw)
	{
		FinalCount();
	}

	//���Ԑ؂�ł����s�����Ă��Ȃ��ꍇ
	if(m_timeLimit <= 0 && m_gameState != enResult && m_gameState != enDraw)
	{
		//���Ҕ���
		WinnerJudge();
	}

	//�|�[�Y�֘A�B
	//�|�[�Y�B
	if (m_gameState == enPlaying && (g_pad[PAD_PLAYER1]->IsTrigger(enButtonStart) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonStart)))
	{
		m_gameState = enPause;

		return;
	}

	//�|�[�Y���A�B
	if (m_gameState == enPause && (g_pad[PAD_PLAYER1]->IsTrigger(enButtonStart) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonStart)))
	{
		m_gameState = enPlaying;

		return;
	}

	if (m_gameState == enResult)
	{
		//�Q�[�������������ŏ��̃t���[��
		if (m_isGameEndFirstFrame == true)
		{
			m_finalHit = NewGO<FinalHit>(0);
			//�e������
			QueryGOs<Bomb>("bomb", [](Bomb* bomb)->bool
				{
					DeleteGO(bomb);
					return true;
				});
			QueryGOs<Debris>("debris", [](Debris* debris)->bool
				{
					DeleteGO(debris);
					return true;
				});
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_onesPlaceSpriteRender);
			if (int(m_timeLimit) >= 10)
			{
				DeleteGO(m_tensPlaceSpriteRender);
			}

			m_isGameEndFirstFrame = false;

			if (m_player1->IsLose() == true)
			{
				m_finalHit->SetLoserNum(NUMBER_PLAYER1);
				m_roundCounter->SubmitRoundWinner(NUMBER_PLAYER2);
			}
			else if (m_player2->IsLose() == true)
			{
				m_finalHit->SetLoserNum(NUMBER_PLAYER2);
				m_roundCounter->SubmitRoundWinner(NUMBER_PLAYER1);
			}

			m_roundCounter->MoveToResultPosition();
		}

		m_gameEndCount++;

		if (m_gameEndCount == GAMEENDTIMER_ROUNDCOUNTER_SHOW)
		{
			m_roundCounter->StartResultMove();
		}

		if (m_gameEndCount == GAMEENDTIMER_START_TRANSITION)
		{
			TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, false);
		}

		if (m_gameEndCount > GAMEENDTIMER_GOTO_RESULT)
		{
			

			if (m_roundCounter->GetOverAllWinner() == -1)
			{
				NewGO<GameScene>(0, "gamescene");
				DeleteGO(m_finalHit);
				DeleteGO(this);
			}
			else
			{
				NewGO<ResultScene>(0, "resultscene");
				ResultScene* resultscene = FindGO<ResultScene>("resultscene");
				resultscene->SetLoserNum(m_finalHit->GetLoserNum());
				DeleteGO(m_roundCounter);
				DeleteGO(m_finalHit);
				DeleteGO(this);
			}
		}
	}

	//����������
	if (m_gameState == enDraw)
	{
		m_drawFontCount--;

		if (m_drawFontCount == DRAWTIMER_START_TRANSITION)
		{
			//�g�����W�V����
			TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, TRANSITION_TIME_NORMAL, false);
		}

		if (m_drawFontCount == DRAWTIMER_GOTO_REMATCH)
		{
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_onesPlaceSpriteRender);
			if (int(m_timeLimit) >= 10)
			{
				DeleteGO(m_tensPlaceSpriteRender);
			}
			DeleteGO(m_drawFontRender);
			DeleteGO(m_onesPlaceSpriteRender);
			DeleteGO(m_gameBGM);
			NewGO<GameScene>(0, "gamescene");
			DeleteGO(this);
		}
	}
}

void GameScene::WinnerJudge()
{
	if (m_player1->GetHP() > m_player2->GetHP())
	{
		//1P�̏���
		m_player1->Win();
		m_player2->Lose();

		//���U���g�V�[���Ɉڍs
		m_gameState = enResult;

	}
	else if(m_player1->GetHP() < m_player2->GetHP())
	{
		//2P�̏���
		m_player1->Lose();
		m_player2->Win();

		//���U���g�V�[���Ɉڍs
		m_gameState = enResult;
	}
	else
	{
		//��������
		m_gameState = enDraw;

		m_drawFontRender = NewGO<prefab::CFontRender>(4);
		m_drawFontRender->SetDrawScreen(prefab::CFontRender::DrawScreen::AllScreen);
		m_drawFontRender->SetPosition(FONT_DRAW_POSITION);
		m_drawFontRender->SetScale(FONT_DRAW_SCALE);
		m_drawFontRender->SetShadowFlag(true);
		m_drawFontRender->SetShadowColor(FONT_SHADOWCOLOR);
		m_drawFontRender->SetShadowOffset(FONT_SHADOWOFFSET);
		m_drawFontRender->SetText(L"DRAW!");

		//�{�C�X�Đ�
		SoundOneShotPlay(L"Assets/sound/Rakutan.wav");
		SoundOneShotPlay(L"Assets/sound/Draw.wav", SOUND_SE_VOICE_VOLUME);
	}
}

void GameScene::StartCountDown() {

	m_measureSecond += GameTime::GetInstance().GetFrameDeltaTime();		//��b�𑪂�B
	if (m_measureSecond >= 1.0f) {
		m_startCount--;
		m_measureSecond = 0.0f;
		m_playCountSEFlag = true;
	}

	//�J�E���g�_�E��
	if (m_startCount == 3 && m_startCount3_Flag == false) {
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		SoundOneShotPlay(L"Assets/sound/CountDown3.wav", SOUND_SE_VOICE_VOLUME);

		m_startCount3_Flag = true;
	}
	if (m_startCount == 2 && m_startCount2_Flag == false) {

		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_RIGHT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_LEFT, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_TOP, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		SoundOneShotPlay(L"Assets/sound/CountDown2.wav", SOUND_SE_VOICE_VOLUME);

		m_startCount2_Flag = true;
	}
	if (m_startCount == 1 && m_startCount1_Flag == false) {

		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_ASSEMBLE, true);
		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM_SHORT, SPRITE_STARTCOUNTDOWN_MOVETIME_SLOW, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISPLAY, true);
		m_startCountDown_1->GetSpriteSupporter().SpriteMove(SPRITE_STARTCOUNTDOWN_MOVEMENT_TO_BOTTOM, SPRITE_STARTCOUNTDOWN_MOVETIME_FAST, SPRITE_STARTCOUNTDOWN_MOVEDELAY_DISASSEMBLE, true);

		SoundOneShotPlay(L"Assets/sound/CountDown1.wav", SOUND_SE_VOICE_VOLUME);

		m_startCount1_Flag = true;
	}
	else if (m_startCount < 0) {
		//�J�E���g�_�E���Ɏg�p�����X�v���C�g���폜
		DeleteGO(m_startCountDown_3_Top);
		DeleteGO(m_startCountDown_3_Middle);
		DeleteGO(m_startCountDown_3_Bottom);
		DeleteGO(m_startCountDown_3_Right);
		DeleteGO(m_startCountDown_2_Top);
		DeleteGO(m_startCountDown_2_Middle);
		DeleteGO(m_startCountDown_2_Bottom);
		DeleteGO(m_startCountDown_2_Right);
		DeleteGO(m_startCountDown_2_Left);
		DeleteGO(m_startCountDown_1);
		DeleteGO(m_startSprite);
		m_gameState = enPlaying;
	}

	if (m_playCountSEFlag == true) {
		prefab::CSoundSource* ssCount = NewGO<prefab::CSoundSource>(0);;
		if (1 <= m_startCount && m_startCount < 4)
		{
			ssCount->Init(L"Assets/sound/CountSE.wav", SoundType::enSE);
			ssCount->SetVolume(SOUND_SE_STARTCOUNTDOWN_VOLUME);
		}
		else if (m_startCount == 0)
		{
			ssCount->Init(L"Assets/sound/AirHorn.wav", SoundType::enSE);
			ssCount->SetVolume(SOUND_SE_STARTHORN_VOLUME);
			SoundOneShotPlay(L"Assets/sound/CountDown0.wav", SOUND_SE_VOICE_VOLUME);

			//�Q�[���X�^�[�g�J�E���g
			m_startSprite = NewGO<prefab::CSpriteRender>(5);
			m_startSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
			m_startSprite->Init("Assets/Image/Start.DDS" , SPRITE_START_WIDTH, SPRITE_START_HEIGHT);
			m_startSprite->SetPosition(SPRITE_START_POSITION);
			m_startSprite->SetScale(SPRITE_START_SCALE);
		}
		ssCount->Play(false);
		m_playCountSEFlag = false;
	}
}
void GameScene::TimeLimitCount()
{
	//�������Ԃ̕\��
	if (0 < m_timeLimit && m_gameState == enPlaying) {
		m_timeLimit -= GameTime::GetInstance().GetFrameDeltaTime();
		//�^�C�����~�b�g���񌅂̎�
		if (m_timeLimit >= 10)
		{
			//��̈�
			int Number = static_cast<int>(m_timeLimit) % 10;
			TimeLimitChangesSprits(Number, enOnesPlaceOfDoubleDigit);
			//�\�̈�
			Number = static_cast<int>(m_timeLimit) / 10;
			TimeLimitChangesSprits(Number, enTenthPlaceOfDoubleDigit);
		}
		//�^�C�����~�b�g���ꌅ�̎�
		else if (static_cast<int>(m_timeLimit) < 10)
		{
			int Number = static_cast<int>(m_timeLimit);
			TimeLimitChangesSprits(Number, enOnesPlaceOfSingleDigit);
		}
	}
	m_oldTimeLimit = static_cast<int>(m_timeLimit);
}

void GameScene::TimeLimitChangesSprits(int num, TimeLimitSpriteDigit numPlace)
{
	//�O�t���[���̐������Ԃ̕b���Ɠ����ꍇreturn
	if (static_cast<int>(m_timeLimit) == m_oldTimeLimit)
	{
		return;
	}

	//�^�C�����~�b�g���O�t���[���ƈقȂ�Ƃ�(��b�o�����Ƃ�)
	
	//num��0�`9�̂ǂ̐������ɂ���āAInit����dds�t�@�C���̃p�X��ς���B
	char ddsPath[256];
	sprintf(ddsPath, "Assets/Image/%d.dds", num);

	//�����̈ʒu�ɂ���ĕ���
	//Init�Œ��ڕύX����ƃf�B�X�N���v�^�q�[�v����������̂ō폜���Ă���쐬����
	switch (numPlace)
	{
	case enOnesPlaceOfDoubleDigit://�񌅎��̈�̈�
		DeleteGO(m_onesPlaceSpriteRender);
		m_onesPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
		m_onesPlaceSpriteRender->Init(ddsPath, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
		m_onesPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_DOUBLEDIGIT);
		m_onesPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
		m_onesPlaceSpriteRender->SetUseSpriteSupporterFlag(false);
		break;
	case enTenthPlaceOfDoubleDigit://�񌅎��̏\�̈�
		DeleteGO(m_tensPlaceSpriteRender);
		m_tensPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
		m_tensPlaceSpriteRender->Init(ddsPath, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
		m_tensPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_TENTHPLACE_OF_DOUBLEDIGIT);
		m_tensPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
		m_tensPlaceSpriteRender->SetUseSpriteSupporterFlag(false);
		break;
	case enOnesPlaceOfSingleDigit://�ꌅ���̈�̈�
		DeleteGO(m_onesPlaceSpriteRender);
		m_onesPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
		m_onesPlaceSpriteRender->Init(ddsPath, SPRITE_TIMELIMIT_WIDTH, SPRITE_TIMELIMIT_HEIGHT);
		m_onesPlaceSpriteRender->SetPosition(SPRITE_TIMELIMIT_POSITION_ONESPLACE_OF_SINGLEDIGIT);
		m_onesPlaceSpriteRender->SetScale(SPRITE_TIMELIMIT_SCALE);
		m_onesPlaceSpriteRender->SetUseSpriteSupporterFlag(false);

		//�c��9�b�ɂȂ������͏\�̈ʂ̃X�v���C�g���폜
		if (num == NUMBER_TIMELIMIT_SPRITE_9)
		{
			DeleteGO(m_tensPlaceSpriteRender);
		}
		break;
	}
}

void GameScene::FinalCount() {

	if (m_timeLimit > 3.0f && m_timeLimit <= 4.0f && m_finalCount3_Flag == false) {
		m_finalCountDown->SetMulColor(SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT);
		m_finalCountDown->SetScale(SPRITE_FINALCOUNTDOWN_SCALE_START);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_SHRINK, SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK, SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR);
		m_finalCountDown->GetSpriteSupporter().SpriteColor(SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT, SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME, SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY);
		m_finalCount3_Flag = true;
	}
	if (m_timeLimit > 2.0f && m_timeLimit <= 3.0f && m_finalCount2_Flag == false) {
		m_finalCountDown->Init("Assets/Image/count2.dds", SPRITE_FINALCOUNTDOWN_WIDTH, SPRITE_FINALCOUNTDOWN_HEGIHT);
		m_finalCountDown->SetMulColor(SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT);
		m_finalCountDown->SetScale(SPRITE_FINALCOUNTDOWN_SCALE_START);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_SHRINK, SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK, SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR);
		m_finalCountDown->GetSpriteSupporter().SpriteColor(SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT, SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME, SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY);
		m_finalCount2_Flag = true;
	}
	if (m_timeLimit > 1.0f && m_timeLimit <= 2.0f && m_finalCount1_Flag == false) {
		m_finalCountDown->Init("Assets/Image/count1.dds", SPRITE_FINALCOUNTDOWN_WIDTH, SPRITE_FINALCOUNTDOWN_HEGIHT);
		m_finalCountDown->SetMulColor(SPRITE_FINALCOUNTDOWN_COLOR_SEMITRANSPARENT);
		m_finalCountDown->SetScale(SPRITE_FINALCOUNTDOWN_SCALE_START);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_SHRINK, SPRITE_FINALCOUNTDOWN_SCALETIME_SHRINK, SPRITE_FINALCOUNTDOWN_SCALEDELAY_SHRINK);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALETIME_DISPLAY, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISPLAY);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(SPRITE_FINALCOUNTDOWN_SCALE_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALETIME_DISAPPEAR, SPRITE_FINALCOUNTDOWN_SCALEDELAY_DISAPPEAR);
		m_finalCountDown->GetSpriteSupporter().SpriteColor(SPRITE_FINALCOUNTDOWN_COLOR_TRANSPARENT, SPRITE_FINALCOUNTDOWN_TRANSPARENT_TIME, SPRITE_FINALCOUNTDOWN_TRANSPARENT_DELAY);
		m_finalCount1_Flag = true;
	}
}