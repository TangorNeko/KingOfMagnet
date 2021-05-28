#include "stdafx.h"
#include "GravityBullet.h"

#include "Player.h"
#include "BackGround.h"
#include "SampleScene.h"
#include "Debris.h"

GravityBullet::~GravityBullet()
{
	DeleteGO(m_effect);

	DeleteGO(m_effect2);
	DeleteGO(m_wearingEffect);
	DeleteGO(m_inflateEffect);
}

bool GravityBullet::Start()
{
	OutputDebugStringA("beforeStart\n");

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Gravity2.tkm");
	m_skinModelRender->SetScale({ 0.05f,0.05f,0.05f, });

	m_stageModel = FindGO<BackGround>("background");

	m_gameScene = FindGO<SampleScene>("gamescene");

	//�G�t�F�N�g
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/���͒e.efk");
	m_effect->SetScale({ 25.0f, 25.0f, 25.0f });

	m_effect2 = NewGO<prefab::CEffect>(0);
	m_effect2->Init(u"Assets/effect/���͒e.efk");
	m_effect2->SetScale({ 25.0f, 25.0f, 25.0f });

	m_inflateEffect = NewGO<prefab::CEffect>(0);
	m_inflateEffect->Init(u"Assets/effect/���͒e2.efk");
	m_inflateEffect->SetScale({ 25.0f, 25.0f, 25.0f });

	m_wearingEffect = NewGO<prefab::CEffect>(0);
	m_wearingEffect->Init(u"Assets/effect/Blackhole3.efk");
	m_wearingEffect->SetScale({ 25.0f, 25.0f, 25.0f });
	m_wearingEffect->SetPosition(m_position);
	m_wearingEffect->Play();

	OutputDebugStringA("afterStart\n");

	return true;
}

void GravityBullet::Update()
{
	OutputDebugStringA("beforeUpdate\n");

	//�|�[�Y���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPause)
	{
		return;
	}

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

	OutputDebugStringA("afterUpdate\n");
}

void GravityBullet::AsBulletBehave()
{
	OutputDebugStringA("beforeBulletBehave\n");
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

	//�e���Z���G�t�F�N�g�̃|�W�V����
	m_wearingEffect->SetPosition(m_position);

	//�e����]������B
	angle += 0.2f;
	if (angle >= 360.0f)
		angle = 0.0f;
	Quaternion qRot;
	qRot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(qRot);
	m_skinModelRender->SetPosition(m_position);

	OutputDebugStringA("afterBulletBehave\n");
}

void GravityBullet::AsExplodeBehave()
{
	OutputDebugStringA("beforeExplodeBehave\n");

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

	//�����̏u�ԂɃG�t�F�N�g�𔭐�������B
	m_effect->SetPosition(m_position);
	m_effect->Play();
	m_inflateEffect->SetPosition(m_position);
	m_inflateEffect->Play();

	//�e���Z���G�t�F�N�g���X�g�b�v
	m_wearingEffect->Stop();
	//�e�̃��f���͍폜
	DeleteGO(m_skinModelRender);

	//����炷
	prefab::CSoundSource* ssGravity = NewGO<prefab::CSoundSource>(0);
	ssGravity->Init(L"Assets/sound/�Í����@.wav", SoundType::enSE);
	ssGravity->SetVolume(1.5f);
	ssGravity->Play(false);

	//���������̂ň��͂𔭐��������Ԃ�
	m_gravityBulletState = enGravity;

	OutputDebugStringA("afterExplodeBehave\n");
}

void GravityBullet::AsGravityBehave()
{
	OutputDebugStringA("beforeGravityBehave\n");

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
				if (diff.Length() > 60.0f &&	//�߂����Ă��_��
					diff.Length() < 400.0f && player != m_parent)
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
	if (m_gravityTimeCount % 60 == 0) 
	{
		m_effect2->SetPosition(m_position);
		m_effect2->Play();
	}
	else if (m_gravityTimeCount % 30 == 0)
	{
		m_effect->SetPosition(m_position);
		m_effect->Play();
	}

	OutputDebugStringA("afterGravityBehave\n");
}

void GravityBullet::AsFinishBehave()
{
	OutputDebugStringA("beforeFinishBehave\n");
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

	OutputDebugStringA("afterFinishBehave\n");
}
