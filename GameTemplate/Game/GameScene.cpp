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
	m_stageLight->SetDirection({ -1.0f,-1.0f,1.0f });
	m_stageLight->SetColor({ 0.8f,0.8f,0.8f });

	m_player1 = NewGO<Player>(0, "Player");
	m_player1->m_position = { 760.0f,0.0f,400.0f };
	m_player1->m_playerNum = 0;

	std::random_device device;
	std::mt19937_64 rnd(device());

	//�v���C���[1�̎��͂̏�Ԃ������_���Ɍ���
	int mag = rnd() % 2;
	if (mag)
	{
		m_player1->m_magPower = 1;
	}
	else
	{
		m_player1->m_magPower = -1;
	}
	m_player1->m_toCameraDir = { 1.0f,0.0f,0.0f };
	m_player1->m_characterDirection = { -1.0f,0.0f,0.0f };
	m_player1->m_sensitivity = m_P1Sensitivity;

	m_player2 = NewGO<Player>(0, "Player");
	m_player2->m_position = { -760.0f,0.0f,-400.0f };
	m_player2->m_playerNum = 1;

	//�v���C���[2�̎��͂̏�Ԃ������_���Ɍ���
	mag = rnd() % 2;
	if (mag)
	{
		m_player2->m_magPower = 1;
	}
	else
	{
		m_player2->m_magPower = -1;
	}
	m_player2->m_toCameraDir = { -1.0f,0.0f,0.0f };
	m_player2->m_characterDirection = { 1.0f,0.0f,0.0f };
	m_player2->m_sensitivity = m_P2Sensitivity;

	//�e�v���C���[�ɓG��n��
	m_player2->m_enemy = m_player1;
	m_player1->m_enemy = m_player2;
	
	//�X�e�[�W�̕\��
	m_backGround = NewGO<BackGround>(0, "background");

	//�K���L�u���b�N�B
	DebrisBlock* debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -700.0f,0.0f,600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 700.0f,0.0f,-600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 0.0f,160.0f,0.0f };

	Repulsion* rep = NewGO<Repulsion>(0, "repulsion");
	rep->m_position = { 310.0f,1.0f,0.0f };
	rep->m_rot.SetRotationDeg(Vector3::AxisY,-90.0f);
	rep = NewGO<Repulsion>(0, "repulsion");
	rep->m_position = { -310.0f,1.0f,0.0f };
	rep->m_rot.SetRotationDeg(Vector3::AxisY,90.0f);
	//����쐬�B�L���[�u�}�b�v����Ȃ��Ă����̔|���B
	m_sky = NewGO<SkyBoard>(0);

	//���ׂč쐬���I���������2��ʂɂ���B
	GameObjectManager::GetInstance()->Set2ScreenMode(true);
	
	//��ʂ���؂��
	m_delimitLineSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_delimitLineSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_delimitLineSpriteRender->SetPosition({ 0.0f,0.0f,0.0f });
	m_delimitLineSpriteRender->Init("Assets/Image/Waku.dds", 40, 720);

	//HP�̃J�o�[
	m_HPCoverSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_HPCoverSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_HPCoverSpriteRender->SetPosition({ 0.0f,304.0f,0.0f });//y304
	m_HPCoverSpriteRender->Init("Assets/Image/HP_Cover.dds", 1280, 112);

	//�^�C�}�[�̘g
	m_TimerBaseSpriteRender = NewGO<prefab::CSpriteRender>(3);
	m_TimerBaseSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_TimerBaseSpriteRender->SetPosition({ 0.0f,300.0f,0.0f });
	m_TimerBaseSpriteRender->Init("Assets/Image/Timer_Base.dds", 272, 120);

	//�J�E���g3
	m_startCountDown_3_Top = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Top->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_3_Top->SetPosition({ -900.0f,100.0f,0.0f });
	m_startCountDown_3_Top->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_startCountDown_3_Middle = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Middle->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_3_Middle->SetPosition({ 900.0f,0.0f,0.0f });
	m_startCountDown_3_Middle->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_startCountDown_3_Bottom = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Bottom->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_3_Bottom->SetPosition({ -900.0f,-100.0f,0.0f });
	m_startCountDown_3_Bottom->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_startCountDown_3_Right = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_3_Right->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_3_Right->SetPosition({ 50.0f,-600.0f,0.0f });
	m_startCountDown_3_Right->Init("Assets/Image/Count/CountLine.dds", 24, 200);
	//�J�E���g2
	m_startCountDown_2_Top = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Top->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_2_Top->SetPosition({ 900.0f,100.0f,0.0f });
	m_startCountDown_2_Top->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_startCountDown_2_Middle = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Middle->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_2_Middle->SetPosition({ -900.0f,0.0f,0.0f });
	m_startCountDown_2_Middle->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_startCountDown_2_Bottom = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Bottom->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_2_Bottom->SetPosition({ 900.0f,-100.0f,0.0f });
	m_startCountDown_2_Bottom->Init("Assets/Image/Count/CountLine.dds", 100, 24);
	//
	m_startCountDown_2_Right = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Right->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_2_Right->SetPosition({ 50.0f,-550.0f,0.0f });
	m_startCountDown_2_Right->Init("Assets/Image/Count/CountLine.dds", 24, 100);
	//
	m_startCountDown_2_Left = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_2_Left->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_2_Left->SetPosition({ -50.0f,550.0f,0.0f });
	m_startCountDown_2_Left->Init("Assets/Image/Count/CountLine.dds", 24, 100);
	//�J�E���g1
	m_startCountDown_1 = NewGO<prefab::CSpriteRender>(3);
	m_startCountDown_1->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_startCountDown_1->SetPosition({ 0.0f,600.0f,0.0f });
	m_startCountDown_1->Init("Assets/Image/Count/CountLine.dds", 36, 200);
	//�Ō�̃J�E���g
	m_finalCountDown = NewGO<prefab::CSpriteRender>(10);
	m_finalCountDown->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_finalCountDown->SetPosition({ 0.0f,0.0f,0.0f });
	m_finalCountDown->Init("Assets/Image/count3.dds", 300, 300);
	m_finalCountDown->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });

	//�^�C�����~�b�g�̈ꌅ��
	m_onesPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_onesPlaceSpriteRender->SetPosition({ 27.0f, 315.0f, 0.0f });
	m_onesPlaceSpriteRender->SetScale({0.25f,0.25f,1.0f});
	m_onesPlaceSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_onesPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);

	//�^�C�����~�b�g�̓񌅖�
	m_tensPlaceSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_tensPlaceSpriteRender->SetPosition({ -27.0f, 315.0f, 0.0f });
	m_tensPlaceSpriteRender->SetScale({ 0.25f,0.25f,1.0f });
	m_tensPlaceSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_tensPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);

	//�����Đ�
	m_gameBGM = NewGO<prefab::CSoundSource>(0);
	m_gameBGM->Init(L"Assets/sound/�T�C�o�[��BGM.wav", SoundType::enBGM);
	m_gameBGM->SetVolume(0.3f);
	m_gameBGM->Play(true);
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, 60, true);
	return true;
}

void GameScene::Update()
{

	//�X�^�[�g�J�E���g�_�E��
	if (m_gameState == enStartCountDown) {
		StartCountDown();
		return;
	}
	TimeLimitCount();

	//���Ԑ؂�ł����s�����Ă��Ȃ��ꍇ
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
	if (m_gameState == enPlaying && (g_pad[0]->IsTrigger(enButtonStart) || g_pad[1]->IsTrigger(enButtonStart)))
	{
		m_gameState = enPause;

		return;
	}

	//�|�[�Y���A�B
	if (m_gameState == enPause && (g_pad[0]->IsTrigger(enButtonStart) || g_pad[1]->IsTrigger(enButtonStart)))
	{
		m_gameState = enPlaying;

		return;
	}

	if (m_gameState == enResult)
	{	
		if (m_isGameEndFirstFrame == true)
		{
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_onesPlaceSpriteRender);
			if (int(m_timeLimit) >= 10)
			{
				DeleteGO(m_tensPlaceSpriteRender);
			}
			m_isGameEndFirstFrame = false;
			if (m_player1->m_Lose == true)
			{
				m_player1->m_loserNum = 0;
				m_player2->m_loserNum = 0;
			}
			else if (m_player2->m_Lose == true)
			{
				m_player1->m_loserNum = 1;
				m_player2->m_loserNum = 1;
			}
		}

		m_gameEndCount++;

		if (m_gameEndCount == 500)
		{
			TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, 60, false);
		}

		if (m_gameEndCount>550)
		{
			NewGO<ResultScene>(0, "resultscene");
			ResultScene* resultscene = FindGO<ResultScene>("resultscene");
			resultscene->m_loserNum = m_player1->m_loserNum;
			DeleteGO(this);
		}
	}

	//����������
	if (m_gameState == enDraw)
	{
		m_drawFontCount--;

		if (m_drawFontCount == 45)
		{
			//�g�����W�V����
			TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, 60, false);
		}

		if (m_drawFontCount == 0)
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
			DeleteGO(m_tensPlaceSpriteRender);
			DeleteGO(m_gameBGM);
			NewGO<GameScene>(0, "gamescene");
			DeleteGO(this);
		}
	}
}

void GameScene::WinnerJudge()
{
	if (m_player1->m_hp > m_player2->m_hp)
	{
		//1P�̏���
		m_player1->Win();
		m_player2->Lose();

		//���U���g�V�[���Ɉڍs
		m_gameState = enResult;

	}
	else if(m_player1->m_hp < m_player2->m_hp)
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
		m_drawFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
		m_drawFontRender->SetPosition({ -185.0f, 130.0f });
		m_drawFontRender->SetScale({ 2.0f, 2.0f });
		m_drawFontRender->SetText(L"DRAW!");
	}
}

void GameScene::StartCountDown() {

	m_measureSecond += GameTime::GetInstance().GetFrameDeltaTime();		//��b�𑪂�B
	if (m_measureSecond >= 1.0f) {
		m_startCount -= 1;
		m_measureSecond = 0.0f;
		m_playCountSEFlag = true;
	}

	//�J�E���g�_�E��
	if (m_startCount == 3 && m_startCount3_Flag == false) {
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove({ 890.0f,0.0f }, 12, 0, true);
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove({ 10.0f,0.0f }, 48, 12, true);
		m_startCountDown_3_Top->GetSpriteSupporter().SpriteMove({ 890.0f,0.0f }, 12, 60, true);

		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove({ -890.0f,0.0f }, 12, 0, true);
		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove({ -10.0f,0.0f }, 48, 12, true);
		m_startCountDown_3_Middle->GetSpriteSupporter().SpriteMove({ -890.0f,0.0f }, 12, 60, true);

		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove({ 890.0f,0.0f }, 12, 0, true);
		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove({ 10.0f,0.0f }, 48, 12, true);
		m_startCountDown_3_Bottom->GetSpriteSupporter().SpriteMove({ 890.0f,0.0f }, 12, 60, true);

		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove({ 0.0f,590.0f }, 12, 0, true);
		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove({ 0.0f,10.0f }, 48, 12, true);
		m_startCountDown_3_Right->GetSpriteSupporter().SpriteMove({ 0.0f,590.0f }, 12, 60, true);

		m_startCount3_Flag = true;
	}
	if (m_startCount == 2 && m_startCount2_Flag == false) {

		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove({ -890.0f,0.0f }, 12, 0, true);
		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove({ -10.0f,0.0f }, 48, 12, true);
		m_startCountDown_2_Top->GetSpriteSupporter().SpriteMove({ -890.0f,0.0f }, 12, 60, true);

		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove({ 890.0f,0.0f }, 12, 0, true);
		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove({ 10.0f,0.0f }, 48, 12, true);
		m_startCountDown_2_Middle->GetSpriteSupporter().SpriteMove({ 890.0f,0.0f }, 12, 60, true);

		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove({ -890.0f,0.0f }, 12, 0, true);
		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove({ -10.0f,0.0f }, 48, 12, true);
		m_startCountDown_2_Bottom->GetSpriteSupporter().SpriteMove({ -890.0f,0.0f }, 12, 60, true);

		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove({ 0.0f,590.0f }, 12, 0, true);
		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove({ 0.0f,10.0f }, 48, 12, true);
		m_startCountDown_2_Right->GetSpriteSupporter().SpriteMove({ 0.0f,590.0f }, 12, 60, true);

		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove({ 0.0f,-590.0f }, 12, 0, true);
		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove({ 0.0f,-10.0f }, 48, 12, true);
		m_startCountDown_2_Left->GetSpriteSupporter().SpriteMove({ 0.0f,-590.0f }, 12, 60, true);

		m_startCount2_Flag = true;
	}
	if (m_startCount == 1 && m_startCount1_Flag == false) {

		m_startCountDown_1->GetSpriteSupporter().SpriteMove({ 0.0f,-590.0f }, 12, 0, true);
		m_startCountDown_1->GetSpriteSupporter().SpriteMove({ 0.0f,-10.0f }, 48, 12, true);
		m_startCountDown_1->GetSpriteSupporter().SpriteMove({ 0.0f,-590.0f }, 12, 60, true);

		m_startCount1_Flag = true;
	}

	else if (m_startCount < 0) {
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
		m_gameState = enPlaying;

		DeleteGO(m_startSprite);
	}

	if (m_playCountSEFlag == true) {
		prefab::CSoundSource* ssCount = NewGO<prefab::CSoundSource>(0);;
		if (m_startCount >= 1 && m_startCount < 4) 
		{
			ssCount->Init(L"Assets/sound/�J�E���g��2.wav", SoundType::enSE);
			ssCount->SetVolume(0.8f);
		}
		else if (m_startCount == 0)
		{
			ssCount->Init(L"Assets/sound/�G�A�[�z�[��.wav", SoundType::enSE);
			ssCount->SetVolume(0.8f);

			//�Q�[���X�^�[�g�J�E���g
			m_startSprite = NewGO<prefab::CSpriteRender>(5);
			m_startSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
			m_startSprite->Init("Assets/Image/Start.DDS" ,324, 64);
			m_startSprite->SetPosition({ 0.0f, 0.0f, 0.0f });
			m_startSprite->SetScale({ 1.5f, 1.5f, 1.5f });
		}
		ssCount->Play(false);
		m_playCountSEFlag = false;
	}
}
void GameScene::TimeLimitCount()
{
	//�������Ԃ̕\��
	if (m_timeLimit > 0 && m_gameState == enPlaying) {
		m_timeLimit -= GameTime::GetInstance().GetFrameDeltaTime();
		//�^�C�����~�b�g���񌅂̎�
		if (m_timeLimit >= 10)
		{
			//��̈�
			int Number = (int)m_timeLimit % 10;
			TimeLimitChangesSprits(Number,1);
			//�\�̈�
			Number = (int)m_timeLimit / 10;
			TimeLimitChangesSprits(Number,10);
		}
		//�^�C�����~�b�g���ꌅ�̎�
		else if ((int)m_timeLimit < 10)
		{
			int Number = (int)m_timeLimit;
			TimeLimitChangesSprits(Number, 0);
		}
	}
	m_oldTimeLimit = m_timeLimit;
}

void GameScene::TimeLimitChangesSprits(int num, int numPlace)
{
	//�^�C�����~�b�g���O�t���[���ƈقȂ�Ƃ�(��b�o�����Ƃ�)
	if ((int)m_timeLimit != m_oldTimeLimit)
	{
		//num��0�`9�̂ǂ̐������ɂ���āAInit����X�v���C�g��ς���B
		switch (num)
		{
		case 0:
			if (numPlace == 1)	//num����̈ʂ̎�
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/0.dds", 500, 500);
			}
			else if (numPlace == 10)	//num���\�̈ʂ̎�
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/0.dds", 500, 500);
			}
			else if (numPlace == 0)		//num���ꌅ�̎�(�^�C�����~�b�g��10�ȉ��̎�)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/0.dds", 500, 500);
			}
			break;
		case 1:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/1.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/1.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/1.dds", 500, 500);
			}
			break;
		case 2:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/2.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/2.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/2.dds", 500, 500);
			}
			break;
		case 3:
			if (numPlace == 1)
			{			
				m_onesPlaceSpriteRender->Init("Assets/Image/3.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/3.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/3.dds", 500, 500);
			}
			break;
		case 4:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/4.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/4.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/4.dds", 500, 500);
			}
			break;
		case 5:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/5.dds", 500, 500);
			}
			else if (numPlace == 10)
			{			
				m_tensPlaceSpriteRender->Init("Assets/Image/5.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/5.dds", 500, 500);
			}
			break;
		case 6:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/6.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/6.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/6.dds", 500, 500);
			}
			break;
		case 7:
			if (numPlace == 1)
			{			
				m_onesPlaceSpriteRender->Init("Assets/Image/7.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/7.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/7.dds", 500, 500);
			}
			break;
		case 8:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/8.dds", 500, 500);
			}
			else if (numPlace == 10)
			{			
				m_tensPlaceSpriteRender->Init("Assets/Image/8.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/8.dds", 500, 500);
			}
			break;
		case 9:
			if (numPlace == 1)
			{
				m_onesPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);
			}
			else if (numPlace == 10)
			{
				m_tensPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);
			}
			else if (numPlace == 0)
			{
				//�ꌅ�ɂȂ�����A��̈ʕ\���ʒu��^�񒆂ɁB
				DeleteGO(m_tensPlaceSpriteRender);
				m_onesPlaceSpriteRender->SetPosition({ 0.0f, 315.0f, 0.0f });
				m_onesPlaceSpriteRender->Init("Assets/Image/9.dds", 500, 500);
			}
			break;
		default:
			break;
		}
	}
}

void GameScene::FinalCount() {

	if (m_timeLimit > 3.0f && m_timeLimit <= 4.0f && m_finalCount3_Flag == false) {
		m_finalCountDown->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		m_finalCountDown->SetScale({ 3.0f,3.0f,3.0f });
		m_finalCountDown->GetSpriteSupporter().SpriteScale(1.0f, 6, 0);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(0.8f, 24, 6);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(0.0f, 6, 30);
		m_finalCountDown->GetSpriteSupporter().SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 6, 30);
		m_finalCount3_Flag = true;
	}
	if (m_timeLimit > 2.0f && m_timeLimit <= 3.0f && m_finalCount2_Flag == false) {
		m_finalCountDown->Init("Assets/Image/count2.dds", 300, 300);
		m_finalCountDown->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		m_finalCountDown->SetScale({ 3.0f,3.0f,3.0f });
		m_finalCountDown->GetSpriteSupporter().SpriteScale(1.0f, 6, 0);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(0.8f, 24, 6);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(0.0f, 6, 30);
		m_finalCountDown->GetSpriteSupporter().SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 6, 30);
		m_finalCount2_Flag = true;
	}
	if (m_timeLimit > 1.0f && m_timeLimit <= 2.0f && m_finalCount1_Flag == false) {
		m_finalCountDown->Init("Assets/Image/count1.dds", 300, 300);
		m_finalCountDown->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		m_finalCountDown->SetScale({ 3.0f,3.0f,3.0f });
		m_finalCountDown->GetSpriteSupporter().SpriteScale(1.0f, 6, 0);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(0.8f, 24, 6);
		m_finalCountDown->GetSpriteSupporter().SpriteScale(0.0f, 6, 30);
		m_finalCountDown->GetSpriteSupporter().SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 6, 30);
		m_finalCount1_Flag = true;
	}

}