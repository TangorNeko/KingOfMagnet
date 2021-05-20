//�ĈΒe(IncendialyGrenade)�ɂ�鉊��
#include "stdafx.h"
#include "Incendia.h"
#include "Player.h"
#include "SampleScene.h"

Incendia::~Incendia()
{
	DeleteGO(m_effect);
}
bool Incendia::Start()
{
	//�����Đ�
	prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
	ss->Init(L"Assets/sound/��.wav");
	ss->Play(false);

	//�G�t�F�N�g���Đ�
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/�Ĉ�.efk");
	m_effect->SetPosition(m_position);
	m_effect->SetScale({ 25.0f, 15.0f, 25.0f });
	m_effect->Play();

	m_gameScene = FindGO<SampleScene>("gamescene");

	return true;
}
void Incendia::Update()
{
	//�|�[�Y���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPause)
	{
		return;
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			
			//�_���[�W��H�炤�Ԋu
			if (m_damageCountdown[player->m_playerNum] > 0)
				m_damageCountdown[player->m_playerNum]--;

			//�v���C���[���߂����
			Vector3 diff = m_position - player->m_position;		//diff��difference(��)
			float dis = diff.Length();		//dis��distance(����)
			dis = fabsf(dis);
			if (dis <= 250.0f)
			{
				//�������_���[�W���󂯂�B		
				if (m_damageCountdown[player->m_playerNum] <= 0) 
				{
					player->Damage(30);
					player->m_TakeAttackNum++;//�U�����󂯂���
					//���ɉ���_���[�W���󂯂�܂ł̊Ԋu��ݒ�
					m_damageCountdown[player->m_playerNum] = 30;
				}
			}
			return true;
		}
	);

	//����������܂ł̃J�E���g
	m_deleteCountdown--;
	if (m_deleteCountdown <= 0)
		DeleteGO(this);
}