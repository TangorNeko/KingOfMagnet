#include "stdafx.h"
#include "GravityBullet.h"

#include "Player.h"
#include "BackGround.h"
#include "GameScene.h"
#include "Debris.h"

namespace
{
	const Vector3 MODEL_GRAVITY_SCALE = { 0.05f,0.05f,0.05f, };
	const float MODEL_GRAVITY_ROTATE_ANGLE = 0.2f;
	const float SOUND_SE_GRAVITY_VOLUME = 1.5f;
	const Vector3 EFFECT_GRAVITY_SCALE = { 25.0f,25.0f,25.0f };
	const float GRAVITYBULLET_COLLISION_RADIUS = 30.0f;
	const float GRAVITYBULLET_FLOAT_DEBRIS_RANGE_MIN = 60.0f;
	const float GRAVITYBULLET_FLOAT_DEBRIS_RANGE_MAX = 400.0f;
	const float GRAVITYBULLET_FLOAT_DEBRIS_SPEED = 11.0f;
	const float GRAVITYBULLET_SPEED = 30.0f;
	const float GRAVITYBULLET_BACUUMSPEED = 6.5f;
	const int GRAVITYATTACKCOUNT_ATTACK = 180;
	const int GRAVITYATTACKCOUNT_BACUUM = 30;
	const int EFFECT_GRAVITY1_INTERVAL = 30;
	const int EFFECT_GRAVITY2_INTERVAL = 60;
	const int EFFECT_GRAVITY_PLAY = 0;
}

GravityBullet::~GravityBullet()
{
	DeleteGO(m_gravityEffect);
	DeleteGO(m_gravityEffect2);
	DeleteGO(m_wearingEffect);
	DeleteGO(m_inflateEffect);
}

bool GravityBullet::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Gravity2.tkm");
	m_skinModelRender->SetScale(MODEL_GRAVITY_SCALE);

	m_stageModel = FindGO<BackGround>("background");

	m_gameScene = FindGO<GameScene>("gamescene");

	//�G�t�F�N�g
	m_gravityEffect = NewGO<prefab::CEffect>(0);
	m_gravityEffect->Init(u"Assets/effect/���͒e.efk");
	m_gravityEffect->SetScale(EFFECT_GRAVITY_SCALE);

	m_gravityEffect2 = NewGO<prefab::CEffect>(0);
	m_gravityEffect2->Init(u"Assets/effect/���͒e.efk");
	m_gravityEffect2->SetScale(EFFECT_GRAVITY_SCALE);

	m_inflateEffect = NewGO<prefab::CEffect>(0);
	m_inflateEffect->Init(u"Assets/effect/���͒e2.efk");
	m_inflateEffect->SetScale(EFFECT_GRAVITY_SCALE);

	m_wearingEffect = NewGO<prefab::CEffect>(0);
	m_wearingEffect->Init(u"Assets/effect/Blackhole3.efk");
	m_wearingEffect->SetScale(EFFECT_GRAVITY_SCALE);
	m_wearingEffect->SetPosition(m_position);
	m_wearingEffect->Play();

	return true;
}

void GravityBullet::Update()
{
	//�|�[�Y���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause)
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
}

void GravityBullet::AsBulletBehave()
{
	//�ړ������B
	m_position += m_moveDirection * GRAVITYBULLET_SPEED;

	//�v���C���[�Ƃ̓����蔻��p
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetEndPoint(m_position);
	m_bulletCollider.SetRadius(GRAVITYBULLET_COLLISION_RADIUS);

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//���˂����v���C���[�ƈႤ��
			if (player->GetPlayerNum() != m_parent->GetPlayerNum())
			{
				//�����蔻��Ƀq�b�g���Ă���Ȃ甚���B
				if (player->IsBulletHitCollider(m_bulletCollider))
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
	angle += MODEL_GRAVITY_ROTATE_ANGLE;
	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}

	Quaternion qRot;
	qRot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(qRot);
	m_skinModelRender->SetPosition(m_position);
}

void GravityBullet::AsExplodeBehave()
{
	//���͂̃K���L�𕂂����郂�[�h�ɁB
	QueryGOs<Debris>("debris", [this](Debris* debris)->bool
		{
			Vector3 diff = m_position - debris->GetPosition();

			if (diff.Length() < GRAVITYBULLET_FLOAT_DEBRIS_RANGE_MAX && debris->GetDebrisState() == Debris::enDrop)
			{
				//HACK:�����Ă�r���ɏE���Ȃ����_���[�W�𒼂Ŏ󂯂Ȃ��悤��Pop�ɂ��Ă���B
				//��������܂�ǂ��Ƃ͎v��Ȃ� Debris��State�𑝂₷?
				debris->SetDebrisState(Debris::enPop);

				//���������K���L����قǍU����ԂɈڍs�����邽�߂ɃR���e�i�Ɋi�[�B
				m_controlDebrisVector.push_back(debris);
			}

			return true;
		});

	//�����̏u�ԂɃG�t�F�N�g�𔭐�������B
	m_gravityEffect->SetPosition(m_position);
	m_gravityEffect->Play();
	m_inflateEffect->SetPosition(m_position);
	m_inflateEffect->Play();

	//�e���Z���G�t�F�N�g���X�g�b�v
	m_wearingEffect->Stop();
	//�e�̃��f���͍폜
	DeleteGO(m_skinModelRender);

	//����炷
	prefab::CSoundSource* gravitySound = NewGO<prefab::CSoundSource>(0);
	gravitySound->Init(L"Assets/sound/�Í����@.wav", SoundType::enSE);
	gravitySound->SetVolume(SOUND_SE_GRAVITY_VOLUME);
	gravitySound->Play(false);

	//���������̂ň��͂𔭐��������Ԃ�
	m_gravityBulletState = enGravity;
}

void GravityBullet::AsGravityBehave()
{
	//HACK:Pop��Ԃł͏��m_position.y���}�C�i�X10���ꑱ���Ă���̂�
	//���t���[��+11���邱�Ƃ�1�������`�ɂȂ��Ă���B
	//�܂�enPop��Ԃ�m_position.y���}�C�i�X10���ꂽ���n�`�ɓ������Ă�����
	//Drop��ԂɈڍs���邽�ߖ��t���[��Pop�ɂ��Ă���B�����������B
	for (auto debris : m_controlDebrisVector)
	{
		debris->SetDebrisState(Debris::enPop);
		Vector3 position = debris->GetPosition();
		position.y += GRAVITYBULLET_FLOAT_DEBRIS_SPEED;
		debris->SetPosition(position);
	}

	//���͏�Ԃ̃J�E���^�[��i�߂�
	m_gravityTimeCount++;

	//�J�E���^�[��180�ȏォ�A�v���C���[����U���w�����o����t�B�j�b�V����Ԃֈڍs�B
	if (m_gravityTimeCount >= GRAVITYATTACKCOUNT_ATTACK || m_parent->GetGravityAttackFlag() == true)
	{
		m_gravityBulletState = enFinish;
	}
	//�������Ă���v���C���[���z���񂹎n�߂�܂łɏ����^�C�����O��݂���B
	else if (m_gravityTimeCount >= GRAVITYATTACKCOUNT_BACUUM)
	{
		QueryGOs<Player>("Player", [this](Player* player)->bool
			{
				Vector3 diff = m_position - player->GetPosition();
				if (diff.Length() > GRAVITYBULLET_FLOAT_DEBRIS_RANGE_MIN &&	//�߂����Ă��_��
					diff.Length() < GRAVITYBULLET_FLOAT_DEBRIS_RANGE_MAX && player != m_parent)
				{
					//Y�����z���񂹂�Ə��������邱�Ƃ�����̂�y���������B
					Vector3 toGravity = diff;
					toGravity.y = 0.0f;
					toGravity.Normalize();

					//�z���񂹃X�s�[�h����Z
					toGravity *= GRAVITYBULLET_BACUUMSPEED;

					//�G�v���C���[�̃L�����R���Ɏ��s������
					//�������̂������ǂ��Ƃ�����̂�������Ȃ��B
					player->ExecuteCharacon(toGravity);
				}
				return true;
			});
	}
	if (m_gravityTimeCount % EFFECT_GRAVITY2_INTERVAL == EFFECT_GRAVITY_PLAY)
	{
		m_gravityEffect2->SetPosition(m_position);
		m_gravityEffect2->Play();
	}
	else if (m_gravityTimeCount % EFFECT_GRAVITY1_INTERVAL == EFFECT_GRAVITY_PLAY)
	{
		m_gravityEffect->SetPosition(m_position);
		m_gravityEffect->Play();
	}
}

void GravityBullet::AsFinishBehave()
{
	//���������K���L�ɍU�����w���B
	for (auto debris : m_controlDebrisVector)
	{
		//�K���L�̏�Ԃ�Bullet��
		debris->SetDebrisState(Debris::enBullet);

		//�ǂ���̃v���C���[�̍U�������K���L�ɓn���B
		debris->SetParent(m_parent);

		//�U����͈��͒e�̒��S(�G�v���C���[�̈ʒu�ɔ�Ԃ킯�ł͂Ȃ�)
		Vector3 toGravity = m_position - debris->GetPosition();
		toGravity.Normalize();

		//�K���L�Ɉړ��������w��B
		debris->SetMoveDirection(toGravity);
	}

	//�t�B�j�b�V���U�����w�������̂ň��͒e�Ƃ��Ă̖�ڂ͏I���B
	DeleteGO(this);
}
