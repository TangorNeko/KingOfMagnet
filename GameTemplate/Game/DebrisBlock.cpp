#include "stdafx.h"
#include "DebrisBlock.h"

#include <random>
#include "Player.h"
#include "SampleScene.h"
#include "Debris.h"
#include "Bomb.h"

DebrisBlock::~DebrisBlock()
{
	DeleteGO(m_skinModelRender);
}

bool DebrisBlock::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/yuka.tkm");

	m_gameScene = FindGO<SampleScene>("gamescene");

	return true;
}

void DebrisBlock::Update()
{
	//�����^�C�}�[�����炵�Ă����B
	m_factoryTimer--;
	if (m_factoryTimer < 0)
	{
		m_factoryTimer = 0;
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			Vector3 toPlayer = player->m_position - m_position;

			//���̓o�[�X�g���Ă���v���C���[������500�ȓ��ɋ߂Â��A�^�C�}�[���オ���Ă��āA�Q�[���ɑ��݂ł���ő吔�����ł���ΐV�����K���L�𐶐�
			if (player->m_magPower == -1 && player->m_isBurst == true && m_factoryTimer <= 0 && toPlayer.Length() < 500.0f && m_gameScene->GetBulletNum() < m_gameScene->MAXBULLETNUM)
			{
				//�Q�[���V�[���̒e�̐��𑝂₷�B
				//�e��Start�֐��ő��₷�Ɛ����t���[���̌�ɑ��₳���̂Ő������𒴂��Ă��܂��B
				m_gameScene->BulletIncrease();

				std::random_device device;
				std::mt19937_64 rnd(device());

				//��ꒊ�I�A�K���L���A���e��?
				int bulletLottery = rnd() % 100;//0~99

				if (bulletLottery <= 90)
				{
					//�K���L
					Debris* debris = NewGO<Debris>(0, "debris");
					debris->m_debrisState = Debris::enDrop;
					debris->m_position = m_position;

					//��񒊑I�A�K���L�̌`���?
					int shapeLottery = rnd() % 100;//0~99
					if (shapeLottery <= 75)
					{
						//��
						debris->m_debrisShape = Debris::enStone;
					}
					else if (shapeLottery <= 90)
					{
						//��
						debris->m_debrisShape = Debris::enSword;
					}
					else
					{
						//�K�E�Z�`���[�W
						debris->m_debrisShape = Debris::enSpecialCharger;
					}
				}
				else
				{
					//���e
					Bomb* bomb = NewGO<Bomb>(0, "bomb");
					bomb->m_bombState = Bomb::enDrop;
					bomb->m_position = m_position;

					//��񒊑I�A���e�̌`���?
					int shapeLottery = rnd() % 100;//0~99
					if (shapeLottery <= 33)
					{
						//��֒e
						bomb->m_bombShape = Bomb::enGrenade;
					}
					else if (shapeLottery <= 66)
					{
						//�t���b�V���O���l�[�h
						bomb->m_bombShape = Bomb::enFlashGrenade;
					}
					else
					{
						//�ĈΒe
						bomb->m_bombShape = Bomb::enIncendiaryGrenade;
					}
				}

				//�����^�C�}�[���Z�b�g
				m_factoryTimer = 180;
				
			}
			return true;
		});

	m_skinModelRender->SetPosition(m_position);
}