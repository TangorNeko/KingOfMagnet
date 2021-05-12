//�ĈΒe(IncendialyGrenade)�ɂ�鉊��
#include "stdafx.h"
#include "Incendia.h"
#include "Player.h"

Incendia::~Incendia()
{
}
bool Incendia::Start()
{
	prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
	ss->Init(L"Assets/sound/��.wav");
	ss->Play(false);
	return true;
}
void Incendia::Update()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			
			//�_���[�W��H�炤�Ԋu
			if (m_damageCountdown[player->m_playerNum] > 0)
				m_damageCountdown[player->m_playerNum]--;

			//�v���C���[���߂����
			Vector3 diff = m_position - player->m_position;		//diff��difference(��)
			float dis = diff.Length();		//dis��distance(����)
			dis = fabsf(dis);
			if (dis <= 300.0f)
			{
				//�������_���[�W���󂯂�B		
				if (m_damageCountdown[player->m_playerNum] <= 0) 
				{
					player->Damage(10);
					player->m_TakeAttackNum++;//�U�����󂯂���
					//���ɉ���_���[�W���󂯂�܂ł̊Ԋu��ݒ�
					m_damageCountdown[player->m_playerNum] = 60;
				}
			}
			return true;
		}
	);

	//����������܂ł̃J�E���g
	m_deleteCountdown--;
	if (m_deleteCountdown % 100 == 0) {
		prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
		ss->Init(L"Assets/sound/��.wav");
		ss->Play(false);
	}
	if (m_deleteCountdown <= 0)
		DeleteGO(this);
}