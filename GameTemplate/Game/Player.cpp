#include "stdafx.h"
#include "Player.h"

#include "BackGround.h"
#include "Debris.h"
#include "DamageDisplay.h"

Player::~Player()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_statusFontRender);
	DeleteGO(m_resultSpriteRender);
	DeleteGO(m_crosshairRender);
}

bool Player::Start()
{
	//�A�j���[�V�����̏�����
	animationClips[enAnimationClip_Run].Load("Assets/animData/Mage_Run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Idle].Load("Assets/animData/Mage_Idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Attack].Load("Assets/animData/Mage_Attack.tka");
	animationClips[enAnimationClip_Attack].SetLoopFlag(false);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Walk].Load("Assets/animData/Mage_Walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Fall].Load("Assets/animData/Mage_Fall.tka");
	animationClips[enAnimationClip_Fall].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B

	//���f���̏�����
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/Mage.tkm", "Assets/modelData/Mage.tks", animationClips, enAnimationClip_num);
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetScale(m_scale);

	//�L�����R���̏�����
	m_charaCon.Init(10.0f, 50.0f, m_position);

	//�X�e�[�^�X�\���̏�����
	m_statusFontRender = NewGO<prefab::CFontRender>(1);
	m_statusFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)m_playerNum);
	m_statusFontRender->SetPosition({ -625.0f, 350.0f });

	//�Ə��\���̏�����
	m_crosshairRender = NewGO<prefab::CSpriteRender>(1);
	m_crosshairRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));
	m_crosshairRender->Init("Assets/Image/Sight.dds", 32, 32);

	//�X�e�[�W�̃N���X���擾
	m_stageModel = FindGO<BackGround>("background");

	return true;
}

void Player::Update()
{
	//�v���C���[�ԍ����^�����Ă��Ȃ���Ή������Ȃ�
	if (m_playerNum == -1)
	{
		return;
	}

	//���͂̕ω�
	ChangeMagnetPower();

	//�̗͓��X�e�[�^�X�̃e�L�X�g��\��(��ɉ摜�ɂ���B)
	DisplayStatus();

	//���W�ɉ����ĎO�p�`�̓����蔻��̏ꏊ���Z�b�g�B
	Collision();

	//�ړ�
	Move();

	//�ێ����Ă���K���L�̈ʒu�𐧌䂷��
	HoldDebris();

	//�o�[�X�g���g�p���Ă���?
	if (m_isBurst == true)
	{
		MagneticBurst();
	}
	else
	{
		MagneticBehavior();
	}
	
	//��ԍX�V�B
	UpdateState();
	//�A�j���[�V�����I���B
	AnimationSelect();

	//�J�����̈ړ�
	Camera();

}

//�̗́A���r�E�X�Q�[�W�̕\��
void Player::DisplayStatus()
{
	//�̗́A�`���[�W�A���݂̎����̎��͂̏�Ԃ̕\��
	std::wstring powerText;
	switch (m_magPower)
	{
	case -1:
		powerText = L"����";
		m_statusFontRender->SetColor({ 0.0f,0.0f,1.0f,1.0f });
		break;
	case 1:
		powerText = L"�˗�";
		m_statusFontRender->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		break;
	default:
		powerText = L"error";
	}

	wchar_t charge[256];
	swprintf_s(charge, L"%.1f", m_charge / 10.0f);

	m_statusFontRender->SetText(L"HP:" + std::to_wstring(m_hp)
		+ L"\nCharge:" + charge
		+ L"%\n\n\n\n\n\n\n\n\n����:" + powerText
	);
}

//�ړ�
void Player::Move()
{
	//�J�����̑O����
	front = m_position - g_camera3D[m_playerNum]->GetPosition();
	front.y = 0.0f;
	front.Normalize();

	//�J�����̉E����
	right = Cross(g_vec3AxisY, front);

	n = front.Dot(Vector3::AxisZ);//����
	float angle = acosf(n);//�A�[�N�R�T�C��
	if (front.x < 0) {
		angle *= -1;
	}
	rot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(rot);
	m_moveSpeed = front * g_pad[m_playerNum]->GetLStickYF() * m_characterSpeed + right * g_pad[m_playerNum]->GetLStickXF() * m_characterSpeed;
	if (m_charaCon.IsOnGround() == false)
	{
		m_fallLoop++;
		if (m_moveSpeed.y > -0.001)
		{
			m_moveSpeed.y -= 0.005f * (m_fallLoop * m_fallLoop);
		}
	}
	else
	{
		m_fallLoop = 0;
	}

	if (m_moveSpeed.Length() != 0)
	{
		m_characterDirection = m_moveSpeed;
		m_characterDirection.y = 0;
		m_characterDirection.Normalize();
	}

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f);
	m_magPosition = m_position;
	m_magPosition.y += 50.0f;
	m_skinModelRender->SetPosition(m_position);
}

//�U��
void Player::Attack()
{
	m_attackCount--;
	//�U���̃N�[���^�C�����I���ƈړ����x��߂�
	if (m_attackCount <= 0)
	{
		m_attackCount = 0;

		m_isAttacking = false;

		m_characterSpeed = 6.0f;
	}

	if (g_pad[m_playerNum]->IsPress(enButtonRB1) && m_attackCount == 0)
	{
		//�K���L����ł������Ă���Ȃ�
		if (m_holdDebrisVector.empty() == false)
		{
			//��ԍŏ��ɕێ������K���L�𔭎�
			auto debris = m_holdDebrisVector.front();
			//�ێ������K���L�𔭎˃��[�h�ɂ���
			debris->m_debrisState = Debris::enBullet;

			//�L�����N�^�[�̃X�s�[�h��x������B
			m_characterSpeed = 0.5f;

			//�U�����̃t���O���I��
			m_isAttacking = true;

			//�U���̌��̎�������
			m_attackCount = 10;
			
			//���ː�̌���
			if (m_isLock)
			{
				Vector3 dir = m_enemy->m_magPosition - debris->m_position;
				dir.Normalize();
				debris->m_moveDirection = dir;
			}
			else
			{
				//���b�N�I�����Ă��Ȃ��̂ŁA���ː�����߂�K�v������B
				//�J�����̈ʒu��������Ă�������ɔ�΂����C���쐬�B
				//�L�����N�^�[�̈ʒu���炶��Ȃ����Ƃɒ��ӁB
				//���C�̌���
				Vector3 testRayDir = g_camera3D[m_playerNum]->GetForward();
				//���C�̎n�_
				Vector3 testRayStart = g_camera3D[m_playerNum]->GetPosition();
				//���C�̎n�_�ƌ������狁�߂����C�̏I�_(10000�ȏ�̋����_�����Ƃ͂Ȃ��Ǝv���̂ŋ�����10000�ɐݒ�)
				Vector3 testRayEnd = testRayStart + testRayDir * 10000.0f;

				//��_(�L�����N�^�[�̈ʒu���炱�̈ʒu�Ɍ������Ĕ��˂���邱�ƂɂȂ�)
				Vector3 crossPoint;

				//�����������t���O�B
				bool hitFlag = false;

				//�܂��G�L�����N�^�[�t�߂̔|�������蔻�����������B
				for (auto tricollider : m_enemy->m_triCollider)
				{
					hitFlag = tricollider.isHit(testRayStart, testRayEnd, crossPoint);
					if (hitFlag == true)
					{
						//1��ł��q�b�g���Ă�����break
						break;
					}
				}

				//�G�L�����N�^�[�t�߂Ƀq�b�g���Ȃ�������X�e�[�W�̃��f������������B
				if (hitFlag == false)
				{
					hitFlag = m_stageModel->isLineHitModel(testRayStart, testRayEnd, crossPoint);
				}

				if (hitFlag)
				{
					//�Ə��̎w�������ɔ�΂�
					debris->m_moveDirection = crossPoint - debris->m_position;
					debris->m_moveDirection.Normalize();
				}
				else
				{
					debris->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
					debris->m_moveDirection.y = 0.0f;
					debris->m_moveDirection.Normalize();
				}
			}

			//���˂����K���L��ێ����X�g����폜
			m_holdDebrisVector.erase(m_holdDebrisVector.begin());
		}

	}
}

//�ێ����Ă���K���L�̓����𐧌䂷��
void Player::HoldDebris()
{
	//��]�p�x�𑝉������Ă����B
	m_holdDebrisRotateDeg += 0.5f;
	if (m_holdDebrisRotateDeg >= 360.0f)
	{
		m_holdDebrisRotateDeg -= 360.0f;
	}

	//��]�̒��S��ݒ肷��B
	Vector3 centerOfRotation = m_position;
	centerOfRotation.y += 100.0f;

	//��]�̒��S���v���C���[�����ɁB
	Vector3 cameraDir = g_camera3D[m_playerNum]->GetForward();
	cameraDir.y = 0;
	cameraDir.Normalize();
	centerOfRotation -= cameraDir * 30.0f;
	
	Vector3 toDebris = { 0.0f,50.0f,0.0f };

	//�ێ����Ă���K���L��1�ȏ゠���
	if (m_holdDebrisVector.empty() == false)
	{
		//360�x���K���L�̐�-1�̐��Ŋ�����1������̊p�x�����߂�
		//���ɔ��˂���1�͐g�̂̑O�ɒu���̂�-1
		int vectorSize = m_holdDebrisVector.size();
		float degPerOneDebris = 360.0f / (vectorSize - 1);

		//autoFor���̒��ł��񐔂��J�E���g���邽�߂̕ϐ�
		int i = 0;
		for (auto debris : m_holdDebrisVector)
		{
			//���ɔ��˂���K���L�̂ݎ����̑O�ɁB
			if(i == 0)
			{ 
				debris->m_position = m_magPosition;
				debris->m_position += cameraDir * 100.0f;
			}
			else
			{
				//Y������50�̃x�N�g������]�����ĉ�]�̒��S�_����L�΂�
				Quaternion debrisRot;

				//i�̒l�ɂ���ď��Ԃɕ��ׂ�
				debrisRot.SetRotationDeg(cameraDir, m_holdDebrisRotateDeg + degPerOneDebris * (i-1));

				//toDebris�{�̂ɉ�]��K�p����Ƒ��̏ꏊ�ɂ��e�����o��̂ŃR�s�[���Ă����]��K�p����B
				Vector3 tmp = toDebris;
				debrisRot.Apply(tmp);

				//��]�̒��S�_����L�΂�
				debris->m_position = centerOfRotation + tmp;
			}

			i++;
		}
	}
}

//���̓o�[�X�g���g�p���Ă��Ȃ����̒ʏ�̓���
void Player::MagneticBehavior()
{
	//���͂̏�Ԃɂ���ē���������B
	switch (m_magPower)
	{
	case -1://����

		break;
	case 1: //�˗�

		//�˗̓��[�h�ł̂݃K���L�𔭎˂ł���
		Attack();

		break;
	default:
		MessageBoxA(nullptr, "���͂̏�Ԃ��ُ�ł��B\n", "�G���[", MB_OK);
		break;
	}

	//LB1�������čU�����łȂ������ꍇ�o�[�X�g��ԂɈڍs
	if (g_pad[m_playerNum]->IsTrigger(enButtonLB1) && m_isAttacking == false)
	{
		//���̓Q�[�W��300����B
		m_charge -= 300.0f;
		if (m_charge < 0)
		{
			m_charge = 0;
		}

		m_isBurst = true;
		m_burstCount = 60;
	}
}

//���̓o�[�X�g
void Player::MagneticBurst()
{
	//�o�[�X�g���͈ړ����x��0��
	m_characterSpeed = 0.0f;

	//�G�܂ł̃x�N�g��
	Vector3 toEnemy = m_enemy->m_position - m_position;

	//�G�ɗ^�����
	Vector3 force = toEnemy;
	//�c���͉e�����Ȃ�
	force.y = 0;
	force.Normalize();

	//���͂̏�Ԃɂ���ē���������B
	switch (m_magPower)
	{
	case -1://����

		//��������̂Ń}�C�i�X��
		force *= -10.0f;
		//�߂����鎞�͈�������Ȃ�
		if (toEnemy.Length() > 100.0f && toEnemy.Length() < 750.0f)
		{
			m_enemy->m_charaCon.Execute(force, 1.0f);

			//�G�̒e���܂��D���Ă��Ȃ����A�G�̕ێ�����e��1���ȏ゠�鎞
			if (m_isSteal == false && m_enemy->m_holdDebrisVector.size() != 0)
			{
				int i = 0;
				//�����Ă�K���L���h���b�v������
				//�S���ł͂Ȃ�3�܂łɂ��Ă݂�B

				//�G�̎����Ă���K���L�̃��X�g�𑖍�
				for (auto iterator = m_enemy->m_holdDebrisVector.begin();iterator != m_enemy->m_holdDebrisVector.end();iterator++)
				{
					//�h���b�v��Ԃɂ����Ă����B�����z���̂Ń|�b�v��Ԃł͂Ȃ��B
					(*iterator)->m_debrisState = Debris::enDrop;

					//�J�E���g�������߂�
					i++;

					//3�ɂȂ�����3�z�����̂Ńu���C�N�B
					if (i == 3)
					{
						break;
					}
				}

				//�R���e�i�̃T�C�Y��3�ȉ��Ȃ�폜
				if (m_enemy->m_holdDebrisVector.size() < 3)
				{
					m_enemy->m_holdDebrisVector.clear();
				}
				else//3�ȏ�Ȃ�3���폜
				{
					m_enemy->m_holdDebrisVector.erase(m_enemy->m_holdDebrisVector.begin(), m_enemy->m_holdDebrisVector.begin() + 3);
				}

				//�����G�̒e��D�����̂Ńt���O�ύX
				m_isSteal = true;
			}
		}


		break;
	case 1: //�˗�

		force *= 10.0f;
		if (toEnemy.Length() < 750.0f)
		{
			m_enemy->m_charaCon.Execute(force, 1.0f);
		}

		break;
	default:
		MessageBoxA(nullptr, "���͂̏�Ԃ��ُ�ł��B\n", "�G���[", MB_OK);
		break;
	}

	m_burstCount--;
	//�o�[�X�g�J�E���g��0�ɂȂ�ƃo�[�X�g�I���B
	if (m_burstCount <= 0)
	{
		m_isBurst = false;

		//�G�̒e��D�����t���O�����Z�b�g�B
		m_isSteal = false;

		//�X�s�[�h���߂�
		m_characterSpeed = 6.0f;
	}
}

void Player::ChangeMagnetPower()
{
	//���̓Q�[�W�̎��R����
	m_charge -= 1.0f;
	if (m_charge < 0)
	{
		m_charge = 0;
	}

	//���̓Q�[�W��0�ȉ����A�o�[�X�g���łȂ����
	if (m_charge <= 0 && m_isBurst == false)
	{
		//���͂̏�Ԃ�-1��1�Ȃ̂ŁA-1���|����Δ��]����B
		//���ʂ�switch���Ă����������B
		m_magPower *= -1;

		if (m_isAttacking == true)
		{
			m_isAttacking = false;
			m_characterSpeed = 6.0f;
		}

		//�`���[�W���񕜁B
		m_charge = 1000;
	}
}

//�J�����̈ړ�
void Player::Camera()
{
	//�J�����֘A
	if (g_pad[m_playerNum]->IsTrigger(enButtonRB3))
	{
		m_isLock = !m_isLock;//���b�N�؂�ւ�
	}

	//�v���C���[�̓���90����Ƃ��āA��������J�����ɐL�т�x�N�g������]������B
	Vector3 targetPos = m_position;
	targetPos.y += 90.0f;

	if (m_isLock)
	{
		//���b�N���Ă��鎞�́A�G�̒��S����v���C���[�̓���ւ̃x�N�g���̉�������ɃJ����������B
		Vector3 toEnemy = m_enemy->m_magPosition - targetPos;
		toEnemy.Normalize();
		m_toCameraDir = toEnemy * -1.0f;
	}
	else
	{
		//���b�N����Ă��Ȃ����̓J�����ւ̃x�N�g������]������B
		qRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * 1.5);
		qRotY.Apply(m_toCameraDir);

		Quaternion qRotX;
		Vector3 right = g_camera3D[m_playerNum]->GetRight();
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -1.5);

		//��]��̊p�x���m�F���ďシ���鎞�܂��͉������鎞�͉�]�����Ȃ��B
		Vector3 checkToCamera = m_toCameraDir;
		qRotX.Apply(checkToCamera);
		checkToCamera.Normalize();
		float t = checkToCamera.Dot(Vector3::Up);
		if (t > 0.99f || t < -0.99f)
		{

		}
		else
		{
			qRotX.Apply(m_toCameraDir);
		}
	}

	cameraPos = targetPos + m_toCameraDir * 125.0f;

	g_camera3D[m_playerNum]->SetPosition(cameraPos);
	g_camera3D[m_playerNum]->SetTarget(targetPos);
}

//�����蔻��
void Player::Collision()
{
	//�O�p�`�̓����蔻��~2�Ŏl�p�`�̓����蔻�������
	//�G�ւ̌�����Y���̊O�ςŁA�G���猩���������擾�B
	Vector3 diff = m_enemy->m_position - m_position;
	diff.Normalize();
	Vector3 side = Cross(diff, Vector3::AxisY);

	//�l�p�`�̍���
	Vector3 squarePos1 = m_position;
	squarePos1.y += 250.0f;
	squarePos1 += side * 250.0f;

	//�l�p�`�̍���
	Vector3 squarePos2 = m_position;
	squarePos2.y -= 250.0f;
	squarePos2 += side * 250.0f;

	//�l�p�`�̉E��
	Vector3 squarePos3 = m_position;
	squarePos3.y += 250.0f;
	squarePos3 -= side * 250.0f;

	//�l�p�`�̉E��
	Vector3 squarePos4 = m_position;
	squarePos4.y -= 250.0f;
	squarePos4 -= side * 250.0f;

	//1�ڂ̎O�p�`�̒��_�͍���A�����A�E��B
	m_triCollider[0].SetVertex(squarePos1, squarePos2, squarePos3);
	//2�ڂ̎O�p�`�̒��_�͉E���A�E��A�����B
	m_triCollider[1].SetVertex(squarePos4, squarePos3, squarePos2);

	//�J�v�Z���̓����蔻�������B
	Vector3 legPos = m_position;
	Vector3 headPos = m_position;
	headPos.y += 100.0f;

	//�J�v�Z���̎n�_�ƏI�_���X�V����B(���a��Start�֐��ōŏ��������߂Ă��悢)
	m_collider.SetStartPoint(legPos);
	m_collider.SetEndPoint(headPos);
	m_collider.SetRadius(20.0f);
}

//�����̗̑͂Ƀ_���[�W��^����
void Player::Damage(int damage)
{
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;

		//�̗͂̍X�V�B
		DisplayStatus();

		Lose();

		m_enemy->Win();
	}


	//�^�����_���[�W�ʂ𑊎�ɕ\������
	DamageDisplay* damagedisplay = NewGO<DamageDisplay>(0, "damagedisplay");
	damagedisplay->m_damagePos = m_position;
	damagedisplay->m_enemyNum = m_enemy->m_playerNum;
	damagedisplay->m_damage = damage;
}

//����������
void Player::Win()
{
	m_resultSpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_resultSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);
	m_resultSpriteRender->Init("Assets/Image/Syouri.dds", 256, 256);
}

//�s�k������
void Player::Lose()
{
	m_resultSpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_resultSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);
	m_resultSpriteRender->Init("Assets/Image/Haiboku.dds", 256, 256);
}

//�U����Ԃɐ؂�ւ��ł�����؂�ւ���B
void Player::TryChangeStatusAttack()
{
	if (g_pad[m_playerNum]->IsPress(enButtonX) || g_pad[m_playerNum]->IsPress(enButtonRB1)) {
		m_animStatus = enStatus_Attack;
	}
}

//�����Ԃɐ؂�ւ��ł�����؂�ւ���B
void Player::TryChangeStatusRun()
{
	if (m_moveSpeed.LengthSq() > 5.0f) {
		m_animStatus = enStatus_Run;
	}
}

//������Ԃɐ؂�ւ��ł�����؂�ւ���B
void Player::TryChangeStatusWalk()
{
	if (m_moveSpeed.LengthSq() <= 5.0f && m_moveSpeed.LengthSq() > 0.0f) {
		m_animStatus = enStatus_Walk;
	}
}

//������Ԃɐ؂�ւ���
void Player::TryChangeStatusFall()
{
	if (m_charaCon.IsOnGround() == false)
	{
		m_animStatus = enStatus_Fall;
	}
}

//�ҋ@��Ԃɐ؂�ւ��ł�����؂�ւ���B
void Player::TryChangeStatusIdle()
{
	if (m_moveSpeed.LengthSq() <= 0.001f) {
		m_animStatus = enStatus_Idle;
	}
}

//�A�j���[�V�����̏�ԍX�V
void Player::UpdateState()
{
	switch (m_animStatus) {
	case enStatus_Attack:
		TryChangeStatusFall();
		TryChangeStatusAttack();
		if (m_skinModelRender->IsPlayingAnimation() == false)
		{
			m_animStatus = enStatus_Idle;
		}
		break;
	case enStatus_Run:
		TryChangeStatusAttack();
		TryChangeStatusWalk();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		break;
	case enStatus_Walk:
		TryChangeStatusAttack();
		TryChangeStatusRun();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		break;
	case enStatus_Idle:
		TryChangeStatusAttack();
		TryChangeStatusRun();
		TryChangeStatusWalk();
		TryChangeStatusFall();
		break;
	case enStatus_Fall:
		TryChangeStatusAttack();
		TryChangeStatusRun();
		TryChangeStatusWalk();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		break;
	}
}

void Player::AnimationSelect()
{
	m_skinModelRender->m_animation_speed = 1.0;

	switch (m_animStatus)
	{
	case enStatus_Attack:
		m_skinModelRender->m_animation_speed = 4.0;
		m_skinModelRender->PlayAnimation(enAnimationClip_Attack);
		break;
	case enStatus_Run:
		m_skinModelRender->PlayAnimation(enAnimationClip_Run);
		break;
	case enStatus_Walk:
		m_skinModelRender->PlayAnimation(enAnimationClip_Walk);
		break;
	case enStatus_Idle:
		m_skinModelRender->PlayAnimation(enAnimationClip_Idle);
		break;
	case enStatus_Fall:
		m_skinModelRender->PlayAnimation(enAnimationClip_Fall);
		break;
	}

}