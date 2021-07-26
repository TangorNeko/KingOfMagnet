#include "stdafx.h"
#include "Repulsion.h"
#include "Player.h"
#include "GameScene.h"

namespace
{
	const float REPULSION_AREA_LENGTH = 160.0f;				//�˗͏���xz�͈�
	const float REPULSION_RANGE = 200.0f;					//�˗͏���y�͈�
	const Vector3 REPULSION_SPEED = { 0.0f,5.0f,0.0f };		//�˗͏��̏㏸�X�s�[�h
	const float SOUND_SE_REPULSION_ENTER_VOLUME = 1.5f;		//�˗͏��𓥂񂾏u�Ԃ�SE�̃{�����[��
	const float SOUND_SE_REPULSION_STAY_VOLUME = 1.0f;		//�˗͏��ɂƂǂ܂��Ă��鎞��SE�̃{�����[��
	const float SOUND_SE_INCREASE_VALUE = 0.1f;				//�˗͏���SE�̃{�����[���̑�����
	const float SOUND_SE_DECAY_VALUE = 0.01f;				//�˗͏���SE�̃{�����[���̌�����
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
	//�|�[�Y�������U���g�V�[���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause || m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		return;
	}

	if (m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		for (auto soundSource : m_repulsionStaySE)
		{
			soundSource->Stop();
		}
	}

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{			
			//���炭�v���C���[����l���邩���肭�s���Ȃ��B
			//����2�v���C���[���p�ӂ���B

			int playerNum = player->GetPlayerNum();

				//�˗͏��̒��S�ƃv���C���[�Ƃ̋���
				Vector3 diff = player->GetPosition() - m_position;
				//�����͈̂̔͂ɓ���ΐ˗͂�^����
				if (fabsf(diff.x) < REPULSION_AREA_LENGTH && fabsf(diff.z) < REPULSION_AREA_LENGTH)
				{
					if (diff.y < REPULSION_RANGE)//���Ƃ̋������߂���Ώ㏸
					{
						Vector3 repulsionValue = REPULSION_SPEED;
						player->ExecuteCharacon(repulsionValue);

						//�����̉��������Z�b�g
						player->ResetFall();
					}

					//SE

					//�v���C���[���˗͏��ɓ����Ă������̉���炷�B
					if (m_isPlayEnterSE[playerNum] == false)
					{
						SoundOneShotPlay(L"Assets/sound/�}�W�b�N�E�F�[�u.wav", SOUND_SE_REPULSION_ENTER_VOLUME);
						m_isPlayEnterSE[playerNum] = true;
					}

					//�v���C���[���˗͏��ɏ���Ă��鎞�̉���炷�B
					if (m_isPlayStaySE[playerNum] == false) {
						
						m_repulsionStaySE[playerNum]->SetVolume(SOUND_SE_REPULSION_STAY_VOLUME);
						m_repulsionStaySE[playerNum]->Play(true);
						m_isPlayStaySE[playerNum] = true;
					}
					//StaySE���܂����Ă���̂Ȃ�΁A���ʂ�����������������B
					m_staySEVolume[playerNum] += SOUND_SE_INCREASE_VALUE;
					if (m_staySEVolume[playerNum] >= SOUND_SE_REPULSION_STAY_VOLUME) {
						m_staySEVolume[playerNum] = SOUND_SE_REPULSION_STAY_VOLUME;
					}
					m_repulsionStaySE[playerNum]->SetVolume(m_staySEVolume[playerNum]);

				}

				//�͈͂ɓ����Ă��Ȃ��Ƃ�
				else if (fabsf(diff.x) >= REPULSION_AREA_LENGTH || fabsf(diff.z) >= REPULSION_AREA_LENGTH)
				{
					//EnterSE��Flag�����Z�b�g
					if (m_isPlayEnterSE[playerNum] == true)
					{
						m_isPlayEnterSE[playerNum] = false;
					}

					//StaySE���������t�F�[�h�A�E�g������B
					if (m_isPlayStaySE[playerNum] == true)
					{
						m_staySEVolume[playerNum] -= SOUND_SE_DECAY_VALUE;
						m_repulsionStaySE[playerNum]->SetVolume(m_staySEVolume[playerNum]);
						//�������S�ɏ�������AStaySE�̍Đ����~
						if (m_staySEVolume[playerNum] <= 0.0f)
						{
							m_staySEVolume[playerNum] = 0.0f;
							m_repulsionStaySE[playerNum]->Stop();
							m_isPlayStaySE[playerNum] = false;
						}
					}
				}

			return true;
		}
	);
		
}