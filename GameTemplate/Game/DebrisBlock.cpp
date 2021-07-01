#include "stdafx.h"
#include "DebrisBlock.h"

#include <random>
#include "Player.h"
#include "GameScene.h"
#include "Debris.h"
#include "Bomb.h"

namespace
{
	const int FACTORY_TIMER_CAN_CREATE = 0;
	const int FACTORY_TIMER_AFTER_CREATE = 90;
	const float FACTORY_MAX_CREATE_DISTANCE = 500.0f;
	const int FACTORY_BULLET_CREATE_NUM = 5;
	const int BULLET_LOTTERY_MOD_VALUE = 100;
	const int BULLET_LOTTERY_DEBRIS = 85;
	const int DEBRIS_SHAPE_LOTTRY_MOD_VALUE = 100;
	const int DEBRIS_SHAPE_LOTTRY_SCRAP = 75;
	const int DEBRIS_SHAPE_LOTTRY_SWORD = 90;
	const int BOMG_SHAPE_LOTTERY_MOD_VALUE = 100;
	const int BOMG_SHAPE_LOTTERY_GRENADE = 33;
	const int BOMG_SHAPE_LOTTERY_FLASHGRENADE = 66;
}
DebrisBlock::~DebrisBlock()
{
	DeleteGO(m_skinModelRender);
}

bool DebrisBlock::Start()
{
	//���f�����쐬
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/scrap.tkm");

	//�Q�[���V�[��������
	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}

void DebrisBlock::Update()
{
	//�Q�[�����|�[�Y���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause)
	{
		return;
	}

	//�����^�C�}�[�����炵�Ă����B
	m_factoryTimer--;
	if (m_factoryTimer < FACTORY_TIMER_CAN_CREATE)
	{
		m_factoryTimer = FACTORY_TIMER_CAN_CREATE;
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			Vector3 toPlayer = player->GetPosition() - m_position;

			//���̓o�[�X�g���Ă���v���C���[������500�ȓ��ɋ߂Â��A�^�C�}�[���オ���Ă��āA�Q�[���ɑ��݂ł���ő吔�����ł���ΐV�����K���L�𐶐�
			if (player->GetMagnetState() == MAGNETSTATE_GRAVITY && player->IsBurst() == true && m_factoryTimer == FACTORY_TIMER_CAN_CREATE && toPlayer.Length() < FACTORY_MAX_CREATE_DISTANCE)
			{
				for (int i = 0; i < FACTORY_BULLET_CREATE_NUM; i++)//�e�𐶐����鐔
				{
					if (m_gameScene->GetBulletNum() < m_gameScene->MAXBULLETNUM)
					{
						//�Q�[���V�[���̒e�̐��𑝂₷�B
						//�e��Start�֐��ő��₷�Ɛ����t���[���̌�ɑ��₳���̂Ő������𒴂��Ă��܂��B
						m_gameScene->BulletIncrease();

						std::random_device device;
						std::mt19937_64 rnd(device());

						//��ꒊ�I�A�K���L���A���e��?
						int bulletLottery = rnd() % BULLET_LOTTERY_MOD_VALUE;//0~99

						if (bulletLottery <= BULLET_LOTTERY_DEBRIS)
						{
							//�K���L
							Debris* debris = NewGO<Debris>(0, "debris");
							debris->SetDebrisState(Debris::enDrop);
							debris->SetPosition(m_position);

							//��񒊑I�A�K���L�̌`���?
							int shapeLottery = rnd() % DEBRIS_SHAPE_LOTTRY_MOD_VALUE;//0~99
							if (shapeLottery <= DEBRIS_SHAPE_LOTTRY_SCRAP)
							{
								//�X�N���b�v
								debris->SetDebrisShape(Debris::enScrap);
							}
							else if (shapeLottery <= DEBRIS_SHAPE_LOTTRY_SWORD)
							{
								//��
								debris->SetDebrisShape(Debris::enSword);
							}
							else
							{
								//�K�E�Z�`���[�W
								debris->SetDebrisShape(Debris::enSpecialCharger);
							}
						}
						else
						{
							//���e
							Bomb* bomb = NewGO<Bomb>(0, "bomb");
							bomb->SetBombState(Bomb::enDrop);
							bomb->SetPosition(m_position);

							//��񒊑I�A���e�̌`���?
							int shapeLottery = rnd() % BOMG_SHAPE_LOTTERY_MOD_VALUE;//0~99
							if (shapeLottery <= BOMG_SHAPE_LOTTERY_GRENADE)
							{
								//��֒e
								bomb->SetBombShape(Bomb::enGrenade);
							}
							else if (shapeLottery <= BOMG_SHAPE_LOTTERY_FLASHGRENADE)
							{
								//�t���b�V���O���l�[�h
								bomb->SetBombShape(Bomb::enFlashGrenade);
							}
							else
							{
								//�ĈΒe
								bomb->SetBombShape(Bomb::enIncendiaryGrenade);
							}
						}
					}
					else
					{
						break;
					}
				}
				//�����^�C�}�[���Z�b�g
				m_factoryTimer = FACTORY_TIMER_AFTER_CREATE;
				
			}
			return true;
		});

	m_skinModelRender->SetPosition(m_position);
}