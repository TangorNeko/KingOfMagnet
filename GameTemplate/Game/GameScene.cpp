#include "stdafx.h"
#include "GameScene.h"

#include "TitleScene.h"

#include "Character_base.h"
#include "ShowModel.h"
#include "ShowSprite.h"
#include "BackGround.h"
#include "CLevel.h"
#include "Mage.h"
#include "Knight.h"
#include "Tank.h"
#include "Ninja.h"
#include "SkyBoard.h"

bool GameScene::Start()
{
	//�X�e�[�W�̃��C�g���쐬
	m_stageLight = NewGO<prefab::CDirectionLight>(0);
	m_stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
	m_stageLight->SetColor({ 0.5f,0.5f,0.5f });

	prefab::CLevelRender2D* m_level2D = NewGO<prefab::CLevelRender2D>(5);
	m_level2D->Init("Assets/Level2D/Level2DTest.casl", nullptr);

	/*
	//�v���C���[1���쐬
	Character_base* player1 = NewGO<Mage>(0, "Player");
	player1->m_position = { 0.0f,0.0f,0.0f };
	player1->m_playerNum = 0;
	player1->m_magPower = 1;

	//�v���C���[2���쐬
	Character_base* player2 = NewGO<Mage>(0, "Player");
	player2->m_position = { 0.0f,0.0f,500.0f };
	player2->m_playerNum = 1;
	player2->m_magPower = -1;
	*/

	//�e�v���C���[�ɓG��n��
	m_player2->m_enemy = m_player1;
	m_player1->m_enemy = m_player2;

	//�A�C�e���������_���ɏo��������
	m_itemgenerator = NewGO<PopRandItem>(0, "popranditem");

	//�X�e�[�W�̕\��
	m_backGround = NewGO<BackGround>(0, "background");

	//����쐬�B�L���[�u�}�b�v����Ȃ��Ă����̔|���B
	m_sky = NewGO<SkyBoard>(0);

	//���ׂč쐬���I���������2��ʂɂ���B
	GameObjectManager::GetInstance()->Set2ScreenMode(true);

	return true;
}

GameScene::~GameScene()
{
	DeleteGO(m_sky);
	DeleteGO(m_backGround);
	DeleteGO(m_itemgenerator);
	DeleteGO(m_player1);
	DeleteGO(m_player2);
	DeleteGO(m_level2D);
	DeleteGO(m_stageLight);

	GameObjectManager::GetInstance()->Set2ScreenMode(false);
}

void GameScene::Update()
{
	if (m_gameEndFlag == true)
	{
		m_gameEndCount++;
	}

	if (m_gameEndCount == 300)
	{
		NewGO<TitleScene>(0, "titlescene");
		DeleteGO(this);
	}
}
