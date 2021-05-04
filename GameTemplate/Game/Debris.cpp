#include "stdafx.h"
#include "Debris.h"

#include "Player.h"
#include "BackGround.h"

#include "Explosion.h"

Debris::~Debris()
{
	DeleteGO(m_skinModelRender);
}

bool Debris::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	//�K���L�̌`��œǂݍ��ރ��f���𕪊�
	switch (m_debrisShape)
	{
	case enStone:
		//�΂̃��f��
		m_skinModelRender->Init("Assets/modelData/MageBullet.tkm");
		m_skinModelRender->SetScale({ 1.0f,1.0f,1.0f });
		break;
	case enSword:
		//���̃��f��
		m_skinModelRender->Init("Assets/modelData/KnightBullet.tkm");
		m_skinModelRender->SetScale({ 0.5f, 0.5f, 0.5f });
		break;
	case enGrenade:
		m_skinModelRender->Init("Assets/modelData/FlashGrenade.tkm");
		m_skinModelRender->SetScale({ 1.0f, 1.0f, 1.0f });
		//������p�x��������ɂ���B
		m_moveDirection.y += 0.4f;
		m_moveDirection.Normalize();
		break;
	case enSpecialCharger:
		m_skinModelRender->Init("Assets/modelData/Gravity.tkm");
		m_skinModelRender->SetScale({ 0.1f, 0.1f, 0.1f });
		//������p�x��������ɂ���B
		m_moveDirection.Normalize();
		break;
	default:
		MessageBoxA(nullptr, "���݂��Ȃ��K���L�̌`��ł��B\n", "�G���[", MB_OK);
		break;
	}

	//�X�e�[�W�Ƃ̓����蔻��p
	m_stageModel = FindGO<BackGround>("background");

	return true;
}

void Debris::Update()
{
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
	if (m_position.y <= -1000.0f) {
		DeleteGO(this);
	}
}


//�n�ʂɗ����Ă��鎞�̋���
void Debris::AsDropBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool {

		Vector3 toPlayer = player->m_position - m_position;

		//���͂̎��̂�
		if (player->m_magPower == -1)
		{
			//�o�[�X�g���Ă�����������Ă���
			if (toPlayer.Length() > 50 && toPlayer.Length() < 500.0f && player->m_isBurst == true)
			{
				toPlayer.y += 10.0f;
				Vector3 moveDir = toPlayer;
				moveDir.Normalize();

				//x�Az���ꂼ��ʁX�ő���
				m_position.x += moveDir.x *= 30.0f;
				//�ǂɂԂ������Ƃ�
				Vector3 crossPoint;
				bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.z += moveDir.z *= 30.0f;
				//�ǂɂԂ������Ƃ�
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;
			}

			//�߂��ɒe�������10���ȓ��Ȃ�E����
			if (toPlayer.Length() < 100.0f && player->m_holdDebrisVector.size() < 10)
			{
				m_parent = player;
				m_debrisState = enHold;

				//�v���C���[�̕ێ�����K���L�R���e�i�Ɋi�[
				player->m_holdDebrisVector.push_back(this);
			}
		}

		//�˗͂̎�
		if (player->m_magPower == 1)
		{
			//�o�[�X�g���Ă�����������Ă���
			if (toPlayer.Length() > 50 && toPlayer.Length() < 500.0f && player->m_isBurst == true)
			{
				Vector3 moveDir = toPlayer;
				moveDir.y = 0.0f;
				moveDir.Normalize();

				//x�Az���ꂼ��ʁX�ő���
				m_position.x += moveDir.x *= -30.0f;
				//�ǂɂԂ������Ƃ�
				Vector3 crossPoint;
				bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.z += moveDir.z *= -30.0f;
				//�ǂɂԂ������Ƃ�
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.y += 10.0f;
			}
			
		}
		return true;
		});
	//�d�͏���
	m_position.y -= 5.0f;
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		m_position = m_oldPosition;
	}
}

//�e�Ƃ��Ĕ��˂���Ă��鎞�̋���
void Debris::AsBulletBehave()
{
	if (m_debrisShape == enGrenade)
	{
		//�v������p�B�����ɃO���l�[�h�̒e�Ƃ��Ă̋����������ĂˁB
		//RB2�{�^������isTrigger�����܂������Ȃ������̂�Y�{�^���Ŕ��˂���悤�Ɋ��蓖�ĂĂ��܂��B
		//�T���v���A�v���C���[���Ō�Ɉړ����������ɓ����悤�ɂ��Ă���B

		//�v���C���[�Ƃ̓����蔻��p
		m_bulletCollider.SetStartPoint(m_oldPosition);
		m_bulletCollider.SetRadius(60.0f);

		QueryGOs<Player>("Player", [this](Player* player)->bool
			{
				//���˂����v���C���[�ƈႤ��
				if (player->m_playerNum != m_parent->m_playerNum)
				{
					//�G�v���C���[�����̓o�[�X�g���Ă��鎞
					if (player->m_isBurst == true)
					{
						Vector3 toPlayer = player->m_magPosition - m_position;

						//�G�Ƃ̋�����500�����Ȃ�
						if (toPlayer.Length() < 500.0f)
						{
							//���͂Ȃ�
							if (player->m_magPower == -1)
							{
								toPlayer.Normalize();
								Vector3 newDirection = m_moveDirection + toPlayer;
								newDirection /= 2;
								newDirection.Normalize();
								m_moveDirection = newDirection;
							}
							else //�˗͂Ȃ�
							{
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
					m_position += m_moveDirection * m_velocity * 0.5f;
					m_moveDirection.y -= 5.0f * 5.0f * 0.002f;
					m_moveDirection.Normalize();

					//�ړ���̓����蔻����X�V
					m_bulletCollider.SetEndPoint(m_position);

					//�����蔻��Ƀq�b�g���Ă���Ȃ�_���[�W�B
					if (player->m_collider.isHitCapsule(m_bulletCollider))
					{
						//��������������|�b�v������
						m_debrisState = enPop;
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
			m_position -= moveDir * 30.0f;

			m_debrisState = enPop;
		}
	}

	else
	{
		//�v���C���[�Ƃ̓����蔻��p
		m_bulletCollider.SetStartPoint(m_oldPosition);
		m_bulletCollider.SetRadius(60.0f);

		QueryGOs<Player>("Player", [this](Player* player)->bool
			{
				//���˂����v���C���[�ƈႤ��
				if (player->m_playerNum != m_parent->m_playerNum)
				{
					//�G�v���C���[�����̓o�[�X�g���Ă��鎞
					if (player->m_isBurst == true)
					{
						Vector3 toPlayer = player->m_magPosition - m_position;

						//�G�Ƃ̋�����500�����Ȃ�
						if (toPlayer.Length() < 500.0f)
						{
							//���͂Ȃ�
							if (player->m_magPower == -1)
							{
								toPlayer.Normalize();
								Vector3 newDirection = m_moveDirection + toPlayer;
								newDirection /= 2;
								newDirection.Normalize();
								m_moveDirection = newDirection;
							}
							else //�˗͂Ȃ�
							{
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
					m_position += m_moveDirection * m_velocity;

					//�ړ���̓����蔻����X�V
					m_bulletCollider.SetEndPoint(m_position);

					//�����蔻��Ƀq�b�g���Ă���Ȃ�_���[�W�B
					if (player->m_collider.isHitCapsule(m_bulletCollider))
					{
						//�q�b�g��
						prefab::CSoundSource* ssHit = NewGO<prefab::CSoundSource>(0);;
						//�K���L�̌`��Ń_���[�W������
						switch (m_debrisShape)
						{
						case enStone:
							//�����Đ�
							ssHit->Init(L"Assets/sound/�_���[�W��.wav");
							ssHit->Play(false);

							player->Damage(50.0f);
							break;
						case enSword:
							//�����Đ�
							ssHit->Init(L"Assets/sound/����������.wav");
							ssHit->Play(false);

							player->Damage(100.0f);
							break;
						case enSpecialCharger:
							//�����Đ�(��)
							ssHit->Init(L"Assets/sound/����������.wav");
							ssHit->Play(false);

							player->Damage(20.0f);
							break;
						}
						//��������������|�b�v������
						m_debrisState = enPop;

						//�v���C���[���m�b�N�o�b�N������B
						player->m_isKnockBack = true;
						player->m_moveSpeed = m_moveDirection * 10.0f;
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
			m_position -= moveDir * 30.0f;

			m_debrisState = enPop;
		}
	}
}


//�v���C���[�ɕێ�����Ă��鎞�̋���
void Debris::AsHoldBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//�X�y�V�����`���[�W���[�������Ă���ƁA�Q�[�W�����������܂�B
			if (m_debrisShape == enSpecialCharger) {
				if (player->m_playerNum == m_parent->m_playerNum) {

					m_specialChargeCount += 1.0f;

					if (m_specialChargeCount == 50.0f) {
						player->ChargeSpecialAttackGauge(1);
						m_specialChargeCount = 0.0f;
					}
				}
			}
		return true;
	});
}

//�����ɓ�����������̋���
void Debris::AsPopBehave()
{
	m_position.y -= 10.0f;

	//�X�e�[�W�Ƃ̓����蔻��
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		//�����������Ƀh���b�v������(TODO:�󒆂Ɏc��̂ł͂Ȃ��n�ʂɗ��Ƃ�����)
		m_position = m_oldPosition;

		if (m_debrisShape == enGrenade) {
			m_explosionCount++;
			if (m_explosionCount >= 80) {
				Explosion* m_explosion = NewGO<Explosion>(0);
				m_explosion->m_position = crossPoint;
				DeleteGO(this);
			}
		}

		else {
			m_debrisState = enDrop;
		}
	}
}