#include "stdafx.h"
#include "Debris.h"

#include "Player.h"
#include "BackGround.h"
#include "GameScene.h"
#include "Explosion.h"
#include <random>

namespace
{
	const float DEBRIS_SPEED = 50.0f;													//�K���L�̒e��
	const int SCRAP_SHAPE_LOTTERY_MOD_VALUE = 3;										//�ʏ�e�̌`�������_���Ɍ��߂邽�߂̗]�艉�Z�l
	const int SCRAP_SHAPE_LOTTERY_PIPE = 0;												//�ʏ�e�̌`��:�p�C�v
	const int SCRAP_SHAPE_LOTTERY_GEAR = 1;												//�ʏ�e�̌`��:����
	const int SCRAP_SHAPE_LOTTERY_SCREW = 2;											//�ʏ�e�̌`��:�l�W
	const Vector3 MODEL_SCRAP_SCALE = { 1.0f,1.0f,1.0f };								//�ʏ�e�̊g�嗦
	const Vector3 MODEL_SWORD_SCALE = { 0.5f,0.5f,0.5f };								//���̊g�嗦
	const Vector3 MODEL_SPECIALCHARGER_SCALE = { 0.1f, 0.1f, 0.1f };					//�X�y�V�����`���[�W���[�̊g�嗦
	const float DEBRIS_DESTROY_HEIGHT = -1000.0f;										//�K���L�����ł��鍂��
	const float DEBRIS_BIRST_MOVE_SPEED_X = 30.0f;										//�o�[�X�g���̈ړ����x_X
	const float DEBRIS_BIRST_MOVE_SPEED_Y = 10.0f;										//�o�[�X�g���̈ړ����x_Y
	const float DEBRIS_BIRST_MOVE_SPEED_Z = 30.0f;										//�o�[�X�g���̈ړ����x_Z
	const float DEBRIS_OBTAINABLE_RANGE = 100.0f;										//�v���C���[�ɏE���鋗��
	const float DEBRIS_GROUND_GRAVITY_SPEED = 10.0f;									//�����Ă��鎞�̏d��
	const int FONT_BULLETNUM_DOUBLEDIGIT_BORDER = 10;									//�e�̏�������2���̋��E
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER1_DOUBLEDIGIT = { -207.0f, -270.0f };	//�v���C���[1�̒e��������2�����������̃t�H���g�̍��W
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER1_SINGLEDIGIT = { -170.0f, -270.0f };	//�v���C���[1�̒e��������1�����������̃t�H���g�̍��W
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER2_DOUBLEDIGIT = { 23.0f, -270.0f };		//�v���C���[2�̒e��������2�����������̃t�H���g�̍��W
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER2_SINGLEDIGIT = { 60.0f, -270.0f };		//�v���C���[2�̒e��������1�����������̃t�H���g�̍��W
	const float DEBRIS_COLLISION_RADIUS = 60.0f;										//�K���L�̓����蔻��̔��a
	const float SOUND_SE_SCRAP_HIT_VOLUME = 1.2f;										//�ʏ�e�������������̌��ʉ��̃{�����[��
	const float SOUND_SE_SWORD_HIT_VOLUME = 1.5f;										//���������������̌��ʉ��̃{�����[��
	const float SOUND_SE_SPECIALCHARGER_HIT_VOLUME = 1.2f;								//�X�y�V�����`���[�W���[�������������̌��ʉ��̃{�����[��
	const int DAMAGE_SCRAP = 80;														//�ʏ�e�̃_���[�W
	const int DAMAGE_SWORD = 120;														//���̃_���[�W
	const int DAMAGE_SPECIALCHARGER = 40;												//�X�y�V�����`���[�W���[�̃_���[�W
	const float DEBRIS_POP_POSITION_SPACE = 30.0f;										//�X�e�[�W�ɓ����������ǂɖ��܂�Ȃ��悤�ɂ��邽�߂��炷����
	const int SPECIALCHARGER_COUNT_CHARGE = 50;											//�X�y�V�����`���[�W���[���v���C���[�̕K�E�Z�Q�[�W���`���[�W����Ԋu
	const int SPECIALCHARGER_CHARGEVALUE = 2;											//�X�y�V�����`���[�W���[���v���C���[�̕K�E�Z�Q�[�W���`���[�W����l

	//���̃N���X�ł��g�������Ȓ萔
	const int PLAYER_HOLD_DEBRIS_SIZE_MAX = 10;											//�v���C���[�����Ă�K���L�̍ő吔
	const float PLAYER_KNOCKBACK_SPEED = 10.0f;											//�v���C���[�̃m�b�N�o�b�N���x
}
Debris::~Debris()
{
	DeleteGO(m_skinModelRender);
	
	//�Q�[���V�[���̒e�̐������炷�B
	if (m_gameScene != nullptr)
	{
		m_gameScene->BulletDecrease();
	}
}

bool Debris::Start()
{
	//�K���L�̃��f�����쐬
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->SetShadowCasterFlag(true);
	int random;
	//�K���L�̌`��œǂݍ��ރ��f���𕪊�
	std::random_device device;
	std::mt19937_64 rnd(device());
	switch (m_debrisShape)
	{
	case enScrap:
		//�X�N���b�v�̃��f��
		random = rnd() % SCRAP_SHAPE_LOTTERY_MOD_VALUE;
		if (random == SCRAP_SHAPE_LOTTERY_PIPE)
		{
			m_skinModelRender->Init("Assets/modelData/pipe.tkm");
		}
		if (random == SCRAP_SHAPE_LOTTERY_GEAR)
		{
			m_skinModelRender->Init("Assets/modelData/gear.tkm");
		}
		if (random == SCRAP_SHAPE_LOTTERY_SCREW)
		{
			m_skinModelRender->Init("Assets/modelData/neji.tkm");
		}
		m_skinModelRender->SetScale(MODEL_SCRAP_SCALE);
		break;
	case enSword:
		//���̃��f��
		m_skinModelRender->Init("Assets/modelData/KnightBullet.tkm");
		m_skinModelRender->SetScale(MODEL_SWORD_SCALE);
		break;
	case enSpecialCharger:
		//�X�y�V�����`���[�W���[�̃��f��
		m_skinModelRender->Init("Assets/modelData/Gravity.tkm");
		m_skinModelRender->SetScale(MODEL_SPECIALCHARGER_SCALE);
		break;
	default:
		MessageBoxA(nullptr, "���݂��Ȃ��K���L�̌`��ł��B\n", "�G���[", MB_OK);
		break;
	}

	//�X�e�[�W�Ƃ̓����蔻��p
	m_stageModel = FindGO<BackGround>("background");

	//�Q�[���V�[�����擾
	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}

void Debris::Update()
{
	//�|�[�Y�������U���g�V�[���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause || m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		return;
	}

	//�O�t���[���̍��W���L�^
	m_oldPosition = m_position;

	//�K���L�̏�Ԃŕ���
	switch (m_debrisState)
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
		MessageBoxA(nullptr, "���݂��Ȃ��K���L�̏�Ԃł��B", "�G���[", MB_OK);
		break;
	}

	m_skinModelRender->SetPosition(m_position);

	//Debris�����ɗ��������A�����B
	if (m_position.y <= DEBRIS_DESTROY_HEIGHT) {
		DeleteGO(this);
	}
}


//�n�ʂɗ����Ă��鎞�̋���
void Debris::AsDropBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool {

		Vector3 toPlayer = player->GetPosition() - m_position;
			//���͂̎��̂�
			if (player->GetMagnetState() == MAGNETSTATE_GRAVITY)
			{
				//�o�[�X�g���Ă�����������Ă���
				if (player->IsBurst() == true && BURST_AFFECT_RANGE_MIN < toPlayer.Length() && toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
				{
					m_isOnGround = false;

					Vector3 moveDir = toPlayer;
					moveDir.Normalize();

					//x�Az�Ay���ꂼ��ʁX�ő���
					m_position.x += moveDir.x * DEBRIS_BIRST_MOVE_SPEED_X;
					//�ǂɂԂ������Ƃ�
					Vector3 crossPoint;
					bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;

					m_position.z += moveDir.z * DEBRIS_BIRST_MOVE_SPEED_Z;
					//�ǂɂԂ������Ƃ�
					isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;

					m_position.y += moveDir.y * DEBRIS_BIRST_MOVE_SPEED_Y;
					//�n�ʂɂԂ������Ƃ�
					crossPoint;
					isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;
				}

				//�߂��ɒe�������10���ȓ��Ȃ�E����
				if (toPlayer.Length() < DEBRIS_OBTAINABLE_RANGE && player->m_holdDebrisVector.size() < PLAYER_HOLD_DEBRIS_SIZE_MAX)
				{
					m_parent = player;
					m_debrisState = enHold;

					//�v���C���[�̕ێ�����K���L�R���e�i�Ɋi�[
					player->m_holdDebrisVector.push_back(this);
					//�e�L�X�g�X�V
					if (player->m_bulletNumFont != nullptr)
					{
						player->m_bulletNumFont->SetText(std::to_wstring(player->m_holdDebrisVector.size()));
						if (player->GetPlayerNum() == NUMBER_PLAYER1)
						{
							if (player->m_holdDebrisVector.size() >= FONT_BULLETNUM_DOUBLEDIGIT_BORDER)
								player->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_DOUBLEDIGIT);

							else
								player->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_SINGLEDIGIT);
						}
						else
						{
							if (player->m_holdDebrisVector.size() >= FONT_BULLETNUM_DOUBLEDIGIT_BORDER)
								player->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_DOUBLEDIGIT);
							else
								player->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_SINGLEDIGIT);
						}
					}
				}
			}

			//�˗͂̎�
			else if (player->GetMagnetState() == MAGNETSTATE_REPULSION)
			{
				//�o�[�X�g���Ă���e����΂�
				if (player->IsBurst() == true && BURST_AFFECT_RANGE_MIN < toPlayer.Length() && toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
				{
					//�e����΂��̂Ńv���C���[�ւ̌����Ƃ͔��Α�
					Vector3 moveDir = toPlayer * -1;

					//y�����ɂ̓v���C���[�̈ʒu�ˑ��ł͈ړ������Ȃ�(���𔲂���\��������̂�)
					moveDir.y = 0.0f;
					moveDir.Normalize();

					//x�Az���ꂼ��ʁX�ő���
					m_position.x += moveDir.x * DEBRIS_BIRST_MOVE_SPEED_X;
					//�ǂɂԂ������Ƃ�
					Vector3 crossPoint;
					bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;

					m_position.z += moveDir.z * DEBRIS_BIRST_MOVE_SPEED_Z;
					//�ǂɂԂ������Ƃ�
					isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;

					m_position.y += DEBRIS_BIRST_MOVE_SPEED_Y;
				}

			}
		return true;
		});

	//�d�͏���
	if (m_isOnGround == false)
	{
		m_position.y -= DEBRIS_GROUND_GRAVITY_SPEED;
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
void Debris::AsBulletBehave()
{
	//�v���C���[�Ƃ̓����蔻��p
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetRadius(DEBRIS_COLLISION_RADIUS);

	bool isPlayerHit = false;
	bool isStageHit = false;
	Player* hitPlayer = nullptr;

	QueryGOs<Player>("Player", [this,&isPlayerHit,&hitPlayer](Player* player)->bool
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
				m_position += m_moveDirection * DEBRIS_SPEED;

				//�ړ���̓����蔻����X�V
				Vector3 dir = m_position - m_oldPosition;
				dir.Normalize();

				//���݂̍��W���J�v�Z���̒[�Ƃ���
				Vector3 CapsuleEnd = m_position - dir * DEBRIS_COLLISION_RADIUS;
				m_bulletCollider.SetEndPoint(CapsuleEnd);

				//�v���C���[�̓����蔻��Ƀq�b�g���Ă���Ȃ�q�b�g�t���O
				if (player->IsBulletHitCollider(m_bulletCollider))
				{
					isPlayerHit = true;

					hitPlayer = player;
				}
			}
			return true;
		});


	//�X�e�[�W�Ƃ̓����蔻��
	Vector3 stageCrossPoint;
	isStageHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, stageCrossPoint);

	//�X�e�[�W�ƃv���C���[�ɓ����q�b�g���Ă��鎞
	if (isPlayerHit == true && isStageHit == true)
	{
		//�����������������̂ݑ���
		Vector3 toPlayer = hitPlayer->GetPosition() - m_oldPosition;
		toPlayer.y = 0;
		Vector3 toStage = stageCrossPoint - m_oldPosition;
		toStage.y = 0;

		//�������߂����Ƀq�b�g
		if (toPlayer.Length() > toStage.Length())
		{
			StageHitAsBullet(stageCrossPoint);
		}
		else
		{
			PlayerHitAsBullet(hitPlayer);
		}
	}
	else if (isStageHit == true)
	{
		//�X�e�[�W�ɂ̂݃q�b�g
		StageHitAsBullet(stageCrossPoint);
	}
	else if (isPlayerHit == true)
	{
		//�v���C���[�ɂ̂݃q�b�g
		PlayerHitAsBullet(hitPlayer);
	}
}

void Debris::PlayerHitAsBullet(Player* player)
{
	//�K���L�̌`��Ń_���[�W������
	switch (m_debrisShape)
	{
	case enScrap:
		//�����Đ�
		if (player->GetHP() > 0) {
			SoundOneShotPlay(L"Assets/sound/DamageSE.wav", SOUND_SE_SCRAP_HIT_VOLUME);
		}
		player->Damage(DAMAGE_SCRAP);
		break;
	case enSword:
		//�����Đ�
		if (player->GetHP() > 0) {
			SoundOneShotPlay(L"Assets/sound/����������.wav", SOUND_SE_SWORD_HIT_VOLUME);
		}
		player->Damage(DAMAGE_SWORD);
		break;
	case enSpecialCharger:
		//�����Đ�(��)
		if (player->GetHP() > 0) {
			SoundOneShotPlay(L"Assets/sound/DamageSE.wav", SOUND_SE_SPECIALCHARGER_HIT_VOLUME);
		}
		player->Damage(DAMAGE_SPECIALCHARGER);
		break;
	}

	//�_���[�W�̃G�t�F�N�g�̐��ʂ�ݒ�
	player->SetDamageEffectFront(m_moveDirection * -1);

	//��������������|�b�v������
	m_debrisState = enPop;
	m_isOnGround = false;

	//�v���C���[���m�b�N�o�b�N������B
	player->SetKnockBackFlag(true);
	player->SetMoveAmount(m_moveDirection * PLAYER_KNOCKBACK_SPEED);
}

void Debris::StageHitAsBullet(const Vector3& crossPoint)
{
	Vector3 moveDir = m_position - m_oldPosition;
	moveDir.Normalize();

	//������������菭����O����|�b�v������
	m_position = crossPoint - (moveDir * DEBRIS_POP_POSITION_SPACE);

	m_debrisState = enPop;
}


//�v���C���[�ɕێ�����Ă��鎞�̋���
void Debris::AsHoldBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//�X�y�V�����`���[�W���[�������Ă���ƁA�Q�[�W�����������܂�B
			if (m_debrisShape == enSpecialCharger) {
				if (player->GetPlayerNum() == m_parent->GetPlayerNum()) {

					m_specialChargeCount++;

					//�J�E���g�����܂�ƃv���C���[�̕K�E�Z�Q�[�W�ʂ𑝂₷�B
					if (m_specialChargeCount == SPECIALCHARGER_COUNT_CHARGE) {
						player->ChargeSpecialAttackGauge(SPECIALCHARGER_CHARGEVALUE);
						m_specialChargeCount = 0;
					}
				}
			}
		return true;
	});

	//�v���C���[�̌����ɃK���L�̃��f����������B
	//�K���L�̉�]�N�H�[�^�j�I��
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
void Debris::AsPopBehave()
{
	m_position.y -= DEBRIS_GROUND_GRAVITY_SPEED;

	//�X�e�[�W�Ƃ̓����蔻��
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		//���������ꍇ�A�ړ��O�̃t���[���ɂ����ꏊ�Ƀh���b�v������
		m_position = m_oldPosition;
		m_debrisState = enDrop;
	}
}
