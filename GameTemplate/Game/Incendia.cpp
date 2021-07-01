//�ĈΒe(IncendialyGrenade)�ɂ�鉊��
#include "stdafx.h"
#include "Incendia.h"
#include "Player.h"
#include "GameScene.h"

namespace
{
	const float SOUND_SE_BURN_VOLUME = 1.0f;
	const Vector3 EFFECT_BURN_SCALE = { 25.0f, 15.0f, 25.0f };
	const float BURN_RANGE = 250.0f;
	const int BURN_DAMAGE = 30;
	const int DAMAGE_COUNT_DAMAGE = 0;
	const int DAMAGE_COUNT_RESET = 30;
}

Incendia::~Incendia()
{
	m_effect->Stop();
	DeleteGO(m_effect);
}
bool Incendia::Start()
{
	//�����Đ�
	prefab::CSoundSource* burnSound = NewGO<prefab::CSoundSource>(0);
	burnSound->Init(L"Assets/sound/��.wav", SoundType::enSE);
	burnSound->SetVolume(SOUND_SE_BURN_VOLUME);
	burnSound->Play(false);

	//�G�t�F�N�g���Đ�
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/�Ĉ�.efk");
	m_effect->SetPosition(m_position);
	m_effect->SetScale(EFFECT_BURN_SCALE);
	m_effect->Play();

	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}
void Incendia::Update()
{
	//�|�[�Y���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause)
	{
		return;
	}


	if (m_deleteFlag == false)
	{
		QueryGOs<Player>("Player", [this](Player* player)->bool
			{
				//�_���[�W��H�炤�Ԋu���J�E���g
				if (m_damageCountdown[player->GetPlayerNum()] > DAMAGE_COUNT_DAMAGE)
				{
					m_damageCountdown[player->GetPlayerNum()]--;
				}

				//�v���C���[���߂����
				Vector3 diff = m_position - player->m_position;		//diff��difference(��)
				float dis = diff.Length();		//dis��distance(����)
				dis = fabsf(dis);
				if (dis <= BURN_RANGE)
				{
					//�������_���[�W���󂯂�B		
					if (m_damageCountdown[player->GetPlayerNum()] <= DAMAGE_COUNT_DAMAGE)
					{
						player->Damage(BURN_DAMAGE);
						//���ɉ���_���[�W���󂯂�܂ł̊Ԋu��ݒ�
						m_damageCountdown[player->GetPlayerNum()] = DAMAGE_COUNT_RESET;
					}
				}
				return true;
			}
		);

		//����������܂ł̃J�E���g
		m_deleteCountdown--;
		if (m_deleteCountdown <= 0)
			m_deleteFlag = true;
	}

	else if (m_deleteFlag == true && m_effect->IsPlay() == false)
	{
		DeleteGO(this);
	}
}