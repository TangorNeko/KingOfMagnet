#include "stdafx.h"
#include "SampleScene.h"
#include "Player.h"
#include "BackGround.h"
#include "SkyBoard.h"
#include "Debris.h"
#include "DebrisBlock.h"
#include "Bomb.h"
#include "Repulsion.h"
#include "TitleScene.h"

SampleScene::~SampleScene()
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

	
	/*QueryGOs<Bomb>("bomb", [](Bomb* bomb)->bool
		{
			DeleteGO(bomb);
			return true;
		});
		

	QueryGOs<Debris>("debris", [](Debris* debris)->bool
		{
			DeleteGO(debris);
			return true;
		});*/

	QueryGOs<Repulsion>("repulsion", [](Repulsion* repulsion)->bool
		{
			DeleteGO(repulsion);
			return true;
		});
	/*DeleteGO(m_delimitLineSpriteRender);
	DeleteGO(m_HPCoverSpriteRender);
	DeleteGO(m_TimerBaseSpriteRender);*/

	DeleteGO(ssBGM);
}

bool SampleScene::Start()
{
	//�X�e�[�W�̃��C�g���쐬
	m_stageLight = NewGO<prefab::CDirectionLight>(0);
	m_stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
	m_stageLight->SetColor({ 0.5f,0.5f,0.5f });

	m_player1 = NewGO<Player>(0, "Player");
	m_player1->m_position = { 860.0f,0.0f,400.0f };
	//m_player1->m_position = { 250.0f,0.0f,250.0f };//�~�j�X�e�[�W
	m_player1->m_playerNum = 0;
	m_player1->m_magPower = 1;
	m_player1->m_toCameraDir = { 1.0f,0.0f,0.0f };

	m_player2 = NewGO<Player>(0, "Player");
	m_player2->m_position = { -860.0f,0.0f,-400.0f };
	//m_player2->m_position = { -250.0f,0.0f,-250.0f };//�~�j�X�e�[�W

	m_player2->m_playerNum = 1;
	m_player2->m_magPower = -1;
	m_player2->m_toCameraDir = { -1.0f,0.0f,0.0f };

	//�e�v���C���[�ɓG��n��
	m_player2->m_enemy = m_player1;
	m_player1->m_enemy = m_player2;
	
	//�X�e�[�W�̕\��
	m_backGround = NewGO<BackGround>(0, "background");

	//�K���L�u���b�N�B
	DebrisBlock* debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -700.0f,0.0f,600.0f };

	/*debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -750.0f,0.0f,600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -700.0f,0.0f,550.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -750.0f,0.0f,550.0f };*/



	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 700.0f,0.0f,-600.0f };

	/*debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 750.0f,0.0f,-600.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 700.0f,0.0f,-550.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 750.0f,0.0f,-550.0f };*/


	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 0.0f,160.0f,0.0f };

	/*debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 50.0f,160.0f,50.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { 50.0f,160.0f,-50.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -50.0f,160.0f,50.0f };

	debrisblock = NewGO<DebrisBlock>(0, "debrisblock");
	debrisblock->m_position = { -50.0f,160.0f,-50.0f };*/

	//�K���L�B
	/*Debris* debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_position = { 0.0f,0.0f,0.0f };

	debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_debrisShape = Debris::enSword;
	debris->m_position = { 200.0f,0.0f,0.0f };

	debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_position = { 300.0f,0.0f,0.0f };

	debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_position = { 300.0f,0.0f,100.0f };

	//��
	debris = NewGO<Debris>(0, "debris");
	debris->m_debrisState = Debris::enDrop;
	debris->m_debrisShape = Debris::enSpecialCharger;
	debris->m_position = { 100.0f,500.0f,100.0f };*/


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
	
	//�^�C�����~�b�g�\��
	m_timeFontRender = NewGO<prefab::CFontRender>(4);
	m_timeFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_timeFontRender->SetPosition({ -60.0f, 380.0f });
	m_timeFontRender->SetScale({ 2.0f, 2.0f });
	
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

	//�Q�[���X�^�[�g�J�E���g
	m_startCountFontRender = NewGO<prefab::CFontRender>(4);
	m_startCountFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_startCountFontRender->SetPosition({ -185.0f, 130.0f });
	m_startCountFontRender->SetScale({ 3.0f, 3.0f });

	//�����Đ�
	ssBGM = NewGO<prefab::CSoundSource>(0);;
	ssBGM->Init(L"Assets/sound/�T�C�o�[��BGM.wav");
	ssBGM->SetVolume(0.3f);
	ssBGM->Play(true);

	return true;
}

void SampleScene::Update()
{

	//�f�o�b�O�p�B�Q�[���V�[���ɑ��݂���e+�O���l�[�h�̐����o��
	char buff[256];
	sprintf_s(buff, "���݂̃Q�[���V�[���ɑ��݂���e�� = %d\n", m_bulletNum);
	OutputDebugStringA(buff);

	if (m_isGameStart == false) {
		StartCountDown();
		return;
	}

	//�������Ԃ̕\��
	if (m_timeLimit > 0 && m_gameEndFlag == false) {
		m_timeLimit -= GameTime::GetInstance().GetFrameDeltaTime();
		if ((int)m_timeLimit == 9) //�ꌅ�ɂȂ�����\���ʒu��^�񒆂ɁB
		{
			m_timeFontRender->SetPosition({ -35.0f, 380.0f });
		}
		m_timeFontRender->SetText(std::to_wstring((int)m_timeLimit));

		if (m_timeLimit >= 10)
		{
			m_timeFontRender->SetPosition({ -60.0f, 380.0f });
		}
		else
		{
			m_timeFontRender->SetPosition({ -35.0f, 380.0f });
		}

	}
	else if(m_gameEndFlag == false)
	{
		WinnerJudge();
		m_gameEndFlag = true;
	}

	if (m_gameEndFlag == true)
	{	
		if (m_GEfirstLoop == true)
		{
			DeleteGO(m_delimitLineSpriteRender);
			DeleteGO(m_HPCoverSpriteRender);
			DeleteGO(m_TimerBaseSpriteRender);
			DeleteGO(m_timeFontRender);
			m_player1->m_displayOff = true;
			m_player2->m_displayOff = true;
			m_GEfirstLoop = false;
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
		if (g_pad[0]->IsTrigger(enButtonA)|| g_pad[1]->IsTrigger(enButtonA))
		{
			m_gameEndCount++;
		}
	}

	if (m_gameEndCount == 2)
	{
		NewGO<TitleScene>(0, "titlescene");
		DeleteGO(this);
	}

	//�G�t�F�N�g�����p
	if (g_pad[0]->IsTrigger(enButtonB)) {
		Player* pl = FindGO<Player>("Player");

		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->Init(u"Assets/effect/���͒e.efk");
		effect->SetPosition(pl->m_position);
		effect->SetScale({ 30.0f, 30.0f, 30.0f });
		effect->Play();
		//�K�E�Z�����p
		pl->ChargeSpecialAttackGauge(100);

	}
}

void SampleScene::WinnerJudge()
{
	if (m_player1->m_hp > m_player2->m_hp)
	{
		//1P�̏���
		m_player1->Win();
		m_player2->Lose();

	}
	else if(m_player1->m_hp < m_player2->m_hp)
	{
		//2P�̏���
		m_player1->Lose();
		m_player2->Win();
	}
	else
	{
		//��������
	}
}

void SampleScene::StartCountDown() {

	m_measureSecond += GameTime::GetInstance().GetFrameDeltaTime();		//��b�𑪂�B
	if (m_measureSecond >= 1.0f) {
		m_startCount -= 1;
		m_measureSecond = 0.0f;
		m_playCountSEFlag = true;
	}
	if (m_startCount >= 1 && m_startCount < 4) {	
		//3�E2�E1�̃J�E���g�\��
		m_startCountFontRender->SetText(L"  " + std::to_wstring(m_startCount));
	}

	else if ( m_startCount == 0) {
		m_startCountFontRender->SetText(L"Start!");
	}

	else if (m_startCount < 0) {
		DeleteGO(m_startCountFontRender);
		m_isGameStart = true;

		QueryGOs<Player>("Player", [this](Player* player)->bool {
			player->m_canMove = true;
			return true;
			});
	}

	if (m_playCountSEFlag == true) {
		prefab::CSoundSource* ssCount = NewGO<prefab::CSoundSource>(0);;
		if (m_startCount >= 1 && m_startCount < 4) 
		{
			ssCount->Init(L"Assets/sound/�J�E���g��.wav");
			ssCount->SetVolume(0.8f);
		}
		else if (m_startCount == 0)
		{
			ssCount->Init(L"Assets/sound/�G�A�[�z�[��.wav");
			ssCount->SetVolume(0.8f);
		}
		ssCount->Play(false);
		m_playCountSEFlag = false;
	}
}