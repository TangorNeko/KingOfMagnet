#include "stdafx.h"
#include "GravityBullet.h"

#include "Player.h"
#include "BackGround.h"
#include "Debris.h"

GravityBullet::~GravityBullet()
{
	DeleteGO(m_skinModelRender);
}

bool GravityBullet::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Gravity.tkm");
	m_skinModelRender->SetScale({ 0.1f,0.1f,0.1f, });

	m_stageModel = FindGO<BackGround>("background");

	return true;
}

void GravityBullet::Update()
{
	//�O�t���[���̍��W���L�^
	m_oldPosition = m_position;

	switch (m_gravityBulletState)
	{
	case enBullet:
		//���˂���Ă���r���̋���
		AsBulletBehave();
		break;
	case enExplode:
		//���e�����u�Ԃ̋���
		AsExplodeBehave();
		break;
	case enGravity:
		//���e������̋���
		AsGravityBehave();
		break;
	case enFinish:
		//�t�B�j�b�V���U�����̋���
		AsFinishBehave();
		break;
	}

	m_skinModelRender->SetPosition(m_position);
}

void GravityBullet::AsBulletBehave()
{
	//�ړ������B
	m_position += m_moveDirection * m_velocity;

	//�v���C���[�Ƃ̓����蔻��p
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetEndPoint(m_position);
	m_bulletCollider.SetRadius(30.0f);

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//���˂����v���C���[�ƈႤ��
			if (player->m_playerNum != m_parent->m_playerNum)
			{
				//�����蔻��Ƀq�b�g���Ă���Ȃ甚���B
				if (player->m_collider.isHitCapsule(m_bulletCollider))
				{
					m_gravityBulletState = enExplode;
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

		//�X�e�[�W�̂ǂ����ɓ��������̂Ŕ����B
		m_gravityBulletState = enExplode;
	}
}

void GravityBullet::AsExplodeBehave()
{
	//���͂̃K���L�𕂂����郂�[�h�ɁB
	QueryGOs<Debris>("debris", [this](Debris* debris)->bool
		{
			Vector3 diff = m_position - debris->m_position;

			if (diff.Length() < 400.0f && debris->m_debrisState == Debris::enDrop)
			{
				//HACK:�����Ă�r���ɏE���Ȃ����_���[�W�𒼂Ŏ󂯂Ȃ��悤��Pop�ɂ��Ă���B
				//��������܂�ǂ��Ƃ͎v��Ȃ� Debris��State�𑝂₷?
				debris->m_debrisState = Debris::enPop;

				//���������K���L����قǍU����ԂɈڍs�����邽�߂ɃR���e�i�Ɋi�[�B
				m_controlDebrisVector.push_back(debris);
			}

			return true;
		});

	//�����̏u�Ԃɉ����f����傫��(TODO:�G�t�F�N�g�ɂ�������)
	m_skinModelRender->SetScale({ 0.25f,0.25f,0.25f });

	//���������̂ň��͂𔭐��������Ԃ�
	m_gravityBulletState = enGravity;
}

void GravityBullet::AsGravityBehave()
{
	//HACK:Pop��Ԃł͏��m_position.y���}�C�i�X10���ꑱ���Ă���̂�
	//���t���[��+11���邱�Ƃ�1�������`�ɂȂ��Ă���B
	//�܂�enPop��Ԃ�m_position.y���}�C�i�X10���ꂽ���n�`�ɓ������Ă�����
	//Drop��ԂɈڍs���邽�ߖ��t���[��Pop�ɂ��Ă���B�����������B
	//TODO:�ォ�璼���B
	for (auto debris : m_controlDebrisVector)
	{
		debris->m_debrisState = Debris::enPop;
		debris->m_position.y += 11.0f;
	}

	//���͏�Ԃ̃J�E���^�[��i�߂�
	m_gravityTimeCount++;

	//�J�E���^�[��180�ȏォ�A�v���C���[����U���w�����o����t�B�j�b�V����Ԃֈڍs�B
	if (m_gravityTimeCount >= 180 || m_parent->m_isGravityBulletAttack == true)
	{
		m_gravityBulletState = enFinish;
	}
	//�������Ă���v���C���[���z���񂹎n�߂�܂łɏ����^�C�����O��݂���B
	else if (m_gravityTimeCount >= 30)
	{
		QueryGOs<Player>("Player", [this](Player* player)->bool
			{
				Vector3 diff = m_position - player->m_position;
				if (diff.Length() < 400.0f && player != m_parent)
				{
					//Y�����z���񂹂�Ə��������邱�Ƃ�����̂�y���������B
					Vector3 toGravity = diff;
					toGravity.y = 0.0f;
					toGravity.Normalize();

					//�z���񂹃X�s�[�h����Z
					toGravity *= m_bacuumSpeed;

					//�G�v���C���[�̃L�����R���Ɏ��s������
					//�������̂������ǂ��Ƃ�����̂�������Ȃ��B
					player->m_charaCon.Execute(toGravity, 1.0f);
				}
				return true;
			});
	}
}

void GravityBullet::AsFinishBehave()
{
	//���������K���L�ɍU�����w���B
	for (auto debris : m_controlDebrisVector)
	{
		//�K���L�̏�Ԃ�Bullet��
		debris->m_debrisState = Debris::enBullet;

		//�ǂ���̃v���C���[�̍U�������K���L�ɓn���B
		debris->m_parent = m_parent;

		//�U����͈��͒e�̒��S(�G�v���C���[�̈ʒu�ɔ�Ԃ킯�ł͂Ȃ�)
		Vector3 toGravity = m_position - debris->m_position;
		toGravity.Normalize();

		//�K���L�Ɉړ��������w��B
		debris->m_moveDirection = toGravity;
	}

	//�t�B�j�b�V���U�����w�������̂ň��͒e�Ƃ��Ă̖�ڂ͏I���B
	DeleteGO(this);
}
