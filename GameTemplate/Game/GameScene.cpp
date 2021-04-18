#include "stdafx.h"
#include "GameScene.h"

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
	prefab::CDirectionLight* stageLight = NewGO<prefab::CDirectionLight>(0);
	stageLight->SetDirection({ 0.0f,-1.0f,0.0f });
	stageLight->SetColor({ 0.5f,0.5f,0.5f });

	prefab::CLevelRender2D* level2D = NewGO<prefab::CLevelRender2D>(5);
	level2D->Init("Assets/Level2D/Level2DTest.casl", nullptr);

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
	NewGO<PopRandItem>(0, "popranditem");

	//�X�e�[�W�̕\��
	NewGO<BackGround>(0, "background");

	//����쐬�B�L���[�u�}�b�v����Ȃ��Ă����̔|���B
	SkyBoard* sky = NewGO<SkyBoard>(0);

	//���ׂč쐬���I���������2��ʂɂ���B
	GameObjectManager::GetInstance()->Set2ScreenMode(true);

	return true;
}

GameScene::~GameScene()
{

}

void GameScene::Update()
{

}
