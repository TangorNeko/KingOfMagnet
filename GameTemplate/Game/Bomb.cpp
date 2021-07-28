#include "stdafx.h"
#include "Bomb.h"

#include "Player.h"
#include "BackGround.h"
#include "GameScene.h"

#include "Explosion.h"
#include "Flash.h"
#include "Incendia.h"

namespace
{
	const float BOMB_SPEED = 20.0f;										//���e�̑��x
	const Vector3 BOMB_GRENADE_SCALE = { 0.5f,0.5f,0.5f };				//�O���l�[�h�̃��f���̊g�嗦
	const Vector3 BOMB_FLASHGRENADE_SCALE = { 0.35f, 0.35f, 0.35f };	//�M���e�̃��f���̊g�嗦
	const Vector3 BOMB_INCENDIARY_SCALE = { 0.25f, 0.25f, 0.25f };		//�ĈΒe�̃��f���̊g�嗦
	const float BOMB_DESTROY_HEIGHT = -1000.0f;							//���e�����ł��鍂��
	const float BOMB_BIRST_MOVE_SPEED_X = 30.0f;						//�o�[�X�g���̈ړ����x_X
	const float BOMB_BIRST_MOVE_SPEED_Y = 10.0f;						//�o�[�X�g���̈ړ����x_Y
	const float BOMB_BIRST_MOVE_SPEED_Z = 30.0f;						//�o�[�X�g���̈ړ����x_Z
	const float BOMB_OBTAINABLE_RANGE = 100.0f;							//�v���C���[�ɏE���鋗��
	const float BOMB_GROUND_GRAVITY_SPEED = 10.0f;						//�����Ă��鎞�̏d��
	const float BOMB_COLLISION_RADIUS = 60.0f;							//���e�̓����蔻��̔��a
	const float BOMB_FALLING_DIRECTION_MINUS_VALUE = 0.035f;			//��������`������Y�̑��x�Ƀ}�C�i�X���Ă����l
	const float BOMB_POP_POSITION_SPACE = 30.0f;						//�X�e�[�W�ɓ����������ǂɓ�����Ȃ��悤�ɂ��邽�߂��炷����
	const int BOMB_EXPLOSION_COUNT_EXPLOSION = 20;						//���e����������J�E���g


	//���̃N���X�ł��g�������Ȓ萔
	const int PLAYER_HOLD_BOMB_SIZE_MAX = 3;							//�v���C���[�����Ă锚�e�̍ő吔
}
Bomb::~Bomb()
{
	DeleteGO(m_skinModelRender);

	//�Q�[���V�[���̒e�̐������炷�B
	if (m_gameScene != nullptr)
	{
		m_gameScene->BulletDecrease();
	}
}

bool Bomb::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->SetShadowCasterFlag(true);

	//���e�̌`��œǂݍ��ރ��f���𕪊�
	switch (m_bombShape)
	{
	case enGrenade:
		m_skinModelRender->Init("Assets/modelData/Grenade.tkm");
		m_skinModelRender->SetScale(BOMB_GRENADE_SCALE);
		m_moveDirection.Normalize();
		break;
	case enFlashGrenade:
		m_skinModelRender->Init("Assets/modelData/FlashGrenade.tkm");
		m_skinModelRender->SetScale(BOMB_FLASHGRENADE_SCALE);
		m_moveDirection.Normalize();
		break;
	case enIncendiaryGrenade:
		m_skinModelRender->Init("Assets/modelData/SmokeGrenade.tkm");
		m_skinModelRender->SetScale(BOMB_INCENDIARY_SCALE);
		m_moveDirection.Normalize();
		break;
	default:
		MessageBoxA(nullptr, "���݂��Ȃ����e�̌`��ł��B\n", "�G���[", MB_OK);
		break;
	}

	//�X�e�[�W�Ƃ̓����蔻��p
	m_stageModel = FindGO<BackGround>("background");

	//�Q�[���V�[�����擾
	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}

void Bomb::Update()
{
	//�|�[�Y�������U���g�V�[���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause || m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		return;
	}

	//�O�t���[���̍��W���L�^
	m_oldPosition = m_position;

	//���e�̏�Ԃŕ���
	switch (m_bombState)
	{
	case enDrop:
		//�n�ʂɗ����Ă��鎞�̋���
		AsDropBehave();
		break;
	case enBullet:
		//�e�Ƃ��Ĕ��˂���Ă��鎞�̋���
		AsBulletBehave();
		break;
	case enHold:
		//�v���C���[�ɕێ�����Ă��鎞�̋���
		AsHoldBehave();
		break;
	case enPop:
		//�����ɓ�����������̋���
		AsPopBehave();
		break;
	default:
		MessageBoxA(nullptr, "���݂��Ȃ����e�̏�Ԃł��B", "�G���[", MB_OK);
		break;
	}

	m_skinModelRender->SetPosition(m_position);

	//Bomb�����ɗ��������A�����B
	if (m_position.y <= BOMB_DESTROY_HEIGHT) {
		DeleteGO(this);
	}
}


//�n�ʂɗ����Ă��鎞�̋���
void Bomb::AsDropBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool {

		Vector3 toPlayer = player->GetPosition() - m_position;

		//���͂̎��̂�
		if (player->GetMagnetState() == MAGNETSTATE_GRAVITY)
		{
			//�o�[�X�g���Ă�����������Ă���
			if (player->IsBurst() == true && toPlayer.Length() > BURST_AFFECT_RANGE_MIN && toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
			{
				m_isOnGround = false;

				Vector3 moveDir = toPlayer;
				moveDir.Normalize();

				//x�Az�Ay���ꂼ��ʁX�ő���
				m_position.x += moveDir.x * BOMB_BIRST_MOVE_SPEED_X;
				//�ǂɂԂ������Ƃ�
				Vector3 crossPoint;
				bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.z += moveDir.z * BOMB_BIRST_MOVE_SPEED_Z;
				//�ǂɂԂ������Ƃ�
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.y += moveDir.y * BOMB_BIRST_MOVE_SPEED_Y;
				//�n�ʂɂԂ������Ƃ�
				crossPoint;
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;
			}

			//�߂��ɒe�������3���ȓ��Ȃ�E����
			if (toPlayer.Length() < BOMB_OBTAINABLE_RANGE && player->m_holdBombVector.size() < PLAYER_HOLD_BOMB_SIZE_MAX)
			{
				m_parent = player;
				m_bombState = enHold;

				//�v���C���[�̕ێ����锚�e�R���e�i�Ɋi�[
				player->m_holdBombVector.push_back(this);
				//�v���C���[���I�����Ă��锚�e�����Z�b�g�B
				player->m_selectBombNo = 0;
			}
		}

		//�˗͂̎�
		else if (player->GetMagnetState() == MAGNETSTATE_REPULSION)
		{
			//�o�[�X�g���Ă���e����΂�
			if (player->IsBurst() == true && toPlayer.Length() > BURST_AFFECT_RANGE_MIN && toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
			{
				//�e����΂��̂Ńv���C���[�ւ̌����Ƃ͔��Α�
				Vector3 moveDir = toPlayer * -1;

				//y�����ɂ̓v���C���[�̈ʒu�ˑ��ł͈ړ������Ȃ�(���𔲂���\��������̂�)
				moveDir.y = 0.0f;
				moveDir.Normalize();

				//x�Az���ꂼ��ʁX�ő���
				m_position.x += moveDir.x * BOMB_BIRST_MOVE_SPEED_X;
				//�ǂɂԂ������Ƃ�
				Vector3 crossPoint;
				bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.z += moveDir.z * BOMB_BIRST_MOVE_SPEED_Z;
				//�ǂɂԂ������Ƃ�
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.y += BOMB_BIRST_MOVE_SPEED_Y;
			}

		}
		return true;
		});
	//�d�͏���
	if (m_isOnGround == false)
	{
		m_position.y -= BOMB_GROUND_GRAVITY_SPEED;
		Vector3 crossPoint;
		bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
		if (isHit == true)
		{
			m_position = m_oldPosition;
			m_isOnGround = true;
		}
	}
	if (m_position.y != m_oldPosition.y)
	{
		m_isOnGround = false;
	}
}

//�e�Ƃ��Ĕ��˂���Ă��鎞�̋���
void Bomb::AsBulletBehave()
{
	//�v���C���[�Ƃ̓����蔻��p
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetRadius(BOMB_COLLISION_RADIUS);

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//���˂����v���C���[�ƈႤ��
			if (player->GetPlayerNum() != m_parent->GetPlayerNum())
			{
				//�G�v���C���[�����̓o�[�X�g���Ă��鎞
				if (player->IsBurst() == true)
				{
					Vector3 toPlayer = player->GetMagPosition() - m_position;

					//�G�Ƃ̋�����500�����Ȃ�
					if (toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
					{						
						//���͂Ȃ�
						if (player->GetMagnetState() == MAGNETSTATE_GRAVITY)
						{
							//�v���C���[�Ɍ������x�N�g���ƌ��݂̈ړ������̕��ς��V�����ړ������ɂȂ�
							toPlayer.Normalize();
							Vector3 newDirection = m_moveDirection + toPlayer;
							newDirection /= 2;
							newDirection.Normalize();
							m_moveDirection = newDirection;
						}
						else //�˗͂Ȃ�
						{
							//�v���C���[���痣��Ă����x�N�g���ƌ��݂̈ړ������̕��ς��V�����ړ������ɂȂ�
							toPlayer.Normalize();
							Vector3 newDirection = m_moveDirection - toPlayer;
							newDirection /= 2;
							newDirection.Normalize();
							m_moveDirection = newDirection;
						}
					}
				}

				//�ړ�����(TODO:�������e�ƈႤ�v���C���[��1�l�������Ȃ��̂�1�񂵂��Ă΂�Ȃ��̂ő��v�����A�ꏊ�̈ړ��͌�������A
				//���̏ꍇ�AQueryGOs���ړ������Ɠ����蔻�菈����2��ɕ����Ă��邱�ƂɂȂ邩��)
				m_position += m_moveDirection * BOMB_SPEED;
				m_moveDirection.y -= BOMB_FALLING_DIRECTION_MINUS_VALUE;
				m_moveDirection.Normalize();

				//�ړ���̓����蔻����X�V
				m_bulletCollider.SetEndPoint(m_position);

				//�����蔻��Ƀq�b�g���Ă���Ȃ�N���B
				if (player->IsBulletHitCollider(m_bulletCollider))
				{
					//��������������|�b�v������
					m_bombState = enPop;
					m_isOnGround = false;
				}
			}
			return true;
		});

	//�X�e�[�W�Ƃ̓����蔻��
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		m_position = crossPoint;

		Vector3 moveDir = m_position - m_oldPosition;
		moveDir.Normalize();

		//������������菭����O����|�b�v������
		m_position -= moveDir * BOMB_POP_POSITION_SPACE;

		m_bombState = enPop;
	}
}


//�v���C���[�ɕێ�����Ă��鎞�̋���
void Bomb::AsHoldBehave()
{
	//�v���C���[�̌����ɔ��e�̃��f����������B
	//���e�̉�]�N�H�[�^�j�I��
	Quaternion DebrisRot;
	//�L�����̌������擾
	Vector3 CharacterDirection = m_parent->GetCameraFront();
	//�㉺�����̌����͖�������B
	CharacterDirection.y = 0.0f;
	CharacterDirection.Normalize();

	//�L�����̌����Ɗ����AxisZ�Ƃ̊p�x�����߂�
	float dot = CharacterDirection.Dot(Vector3::AxisZ);
	float angle = acosf(dot);//�A�[�N�R�T�C��
	//�p�x�������܂�Ȃ��̂ŃL�����̌����łǂ����肩�����߂�
	//�����v���Ȃ琳�̐��@���v���Ȃ畉�̐�
	if (CharacterDirection.x < 0)
	{
		angle *= -1;
	}
	//Y���܂��ɉ�]���Z�b�g
	DebrisRot.SetRotation(Vector3::AxisY, angle);
	//���f���ɉ�]��K�p�B
	m_skinModelRender->SetRotation(DebrisRot);
}

//�����ɓ�����������̋���
void Bomb::AsPopBehave()
{
	m_position.y -= BOMB_GROUND_GRAVITY_SPEED;

	//�X�e�[�W�Ƃ̓����蔻��
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		//�����������Ƀh���b�v������
		m_position = m_oldPosition;

		//���e�̎�ނɂ���ĕ���B
		switch(m_bombShape)
		{
			case enGrenade:
			m_explosionCount++;
			if (m_explosionCount >= BOMB_EXPLOSION_COUNT_EXPLOSION) {
				Explosion* explosion = NewGO<Explosion>(0);
				explosion->SetPosition(crossPoint);
				DeleteGO(this);
			}
			break;
			case enFlashGrenade:
			m_explosionCount++;
			if (m_explosionCount >= BOMB_EXPLOSION_COUNT_EXPLOSION) {
				Flash* flash = NewGO<Flash>(0);
				flash->SetPosition(crossPoint);
				flash->SetParentNum(m_parent->GetPlayerNum());
				DeleteGO(this);
			}
			break;
			case enIncendiaryGrenade:
			m_explosionCount++;
			if (m_explosionCount >= BOMB_EXPLOSION_COUNT_EXPLOSION) {
				Incendia* incendia = NewGO<Incendia>(0);
				incendia->SetPosition(crossPoint);
				DeleteGO(this);
			}
			break;
		}
	}
}