#include "stdafx.h"
#include "Repulsion.h"
#include "Player.h"
#include "GameScene.h"

namespace
{
	const float REPULSION_AREA_LENGTH = 160.0f;
	const float REPULSION_RANGE = 200.0f;
	const Vector3 REPULSION_SPEED = { 0.0f,5.0f,0.0f };
	const float SOUND_SE_REPULSION_ENTER_VOLUME = 1.5f;
	const float SOUND_SE_REPULSION_STAY_VOLUME = 1.5f;
	const float SOUND_SE_INCREASE_VALUE = 0.1f;
	const float SOUND_SE_DECAY_VALUE = 0.01f;
}

Repulsion::~Repulsion()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_repulsionStaySE[NUMBER_PLAYER1]);
	DeleteGO(m_repulsionStaySE[NUMBER_PLAYER2]);
}
bool Repulsion::Start()
{
	//�˗͏��̃��f�����쐬
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/repulsion.tkm");
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_qRot);

	//�˗͏��ɏ�葱���Ă��鎞��SE���쐬
	m_repulsionStaySE[NUMBER_PLAYER1] = NewGO<prefab::CSoundSource>(0);
	m_repulsionStaySE[NUMBER_PLAYER1]->Init(L"Assets/sound/UFO.wav", SoundType::enSE);
	m_repulsionStaySE[NUMBER_PLAYER2] = NewGO<prefab::CSoundSource>(0);
	m_repulsionStaySE[NUMBER_PLAYER2]->Init(L"Assets/sound/UFO.wav", SoundType::enSE);

	//�Q�[���V�[�����擾
	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}
void Repulsion::Update()
{	
	//�|�[�Y���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause)
	{
		return;
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{			
			//���炭�v���C���[����l���邩���肭�s���Ȃ��B
			//����2�v���C���[���p�ӂ���B

				//�˗͏��̒��S�ƃv���C���[�Ƃ̋���
				Vector3 diff = player->m_position - m_position;
				//�����͈̂̔͂ɓ���ΐ˗͂�^����
				if (fabsf(diff.x) < REPULSION_AREA_LENGTH && fabsf(diff.z) < REPULSION_AREA_LENGTH)
				{
					if (diff.y < REPULSION_RANGE)//���Ƃ̋������߂���Ώ㏸
					{
						Vector3 repulsionValue = REPULSION_SPEED;
						player->m_charaCon.Execute(repulsionValue, 1.0f);
						player->m_fallLoop = 0;
					}

					//SE

					//�v���C���[���˗͏��ɓ����Ă������̉���炷�B
					if (m_isPlayEnterSE[player->m_playerNum] == false)
					{
						prefab::CSoundSource* ss1 = NewGO<prefab::CSoundSource>(0);
						ss1->Init(L"Assets/sound/�}�W�b�N�E�F�[�u.wav", SoundType::enSE);
						ss1->SetVolume(SOUND_SE_REPULSION_ENTER_VOLUME);
						ss1->Play(false);
						m_isPlayEnterSE[player->m_playerNum] = true;
					}

					//�v���C���[���˗͏��ɏ���Ă��鎞�̉���炷�B
					if (m_isPlayStaySE[player->m_playerNum] == false) {												
						
						m_repulsionStaySE[player->m_playerNum]->SetVolume(SOUND_SE_REPULSION_STAY_VOLUME);
						m_repulsionStaySE[player->m_playerNum]->Play(true);	
						m_isPlayStaySE[player->m_playerNum] = true;
					}
					//StaySE���܂����Ă���̂Ȃ�΁A���ʂ�1.5f�ɂȂ�܂ŏ���������������B
					m_staySEVolume[player->m_playerNum] += SOUND_SE_INCREASE_VALUE;
					if (m_staySEVolume[player->m_playerNum] >= SOUND_SE_REPULSION_STAY_VOLUME) {
						m_staySEVolume[player->m_playerNum] = SOUND_SE_REPULSION_STAY_VOLUME;
					}
					m_repulsionStaySE[player->m_playerNum]->SetVolume(m_staySEVolume[player->m_playerNum]);

				}

				//�͈͂ɓ����Ă��Ȃ��Ƃ�
				else if (fabsf(diff.x) >= REPULSION_AREA_LENGTH || fabsf(diff.z) >= REPULSION_AREA_LENGTH)
				{
					//EnterSE��Flag�����Z�b�g
					if (m_isPlayEnterSE[player->m_playerNum] == true)
					{
						m_isPlayEnterSE[player->m_playerNum] = false;
					}

					//StaySE���������t�F�[�h�A�E�g������B
					if (m_isPlayStaySE[player->m_playerNum] == true)
					{
						m_staySEVolume[player->m_playerNum] -= SOUND_SE_DECAY_VALUE;
						m_repulsionStaySE[player->m_playerNum]->SetVolume(m_staySEVolume[player->m_playerNum]);
						//�������S�ɏ�������AStaySE�̍Đ����~
						if (m_staySEVolume[player->m_playerNum] <= 0.0f)
						{
							m_staySEVolume[player->m_playerNum] = 0.0f;
							m_repulsionStaySE[player->m_playerNum]->Stop();
							m_isPlayStaySE[player->m_playerNum] = false;
						}
					}
				}

			return true;
		}
	);
		
}