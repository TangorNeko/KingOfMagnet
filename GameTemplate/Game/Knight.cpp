#include "stdafx.h"
#include "Character_base.h"
#include "Bullet.h"
#include "ChargeShot.h"
#include <string>
#include "Knight.h"
#include "BackGround.h"
#include "UsedGravityGrenade.h"

Knight::~Knight()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_pointLight);
	DeleteGO(m_fontRender);
	DeleteGO(m_spriteRender);
}

bool Knight::Start()
{
	animationClips[enAnimationClip_Attack].Load("Assets/animData/Knight_Attack.tka");
	animationClips[enAnimationClip_Attack].SetLoopFlag(false);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight_Idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Run].Load("Assets/animData/Knight_Run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Walk].Load("Assets/animData/Knight_Walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Move].Load("Assets/animData/Mage_Attack.tka");
	animationClips[enAnimationClip_Move].SetLoopFlag(false);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Fall].Load("Assets/animData/Knight_Fall.tka");
	animationClips[enAnimationClip_Fall].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B

//�}�V���K���p
	animationClips[enAnimationClip_Gun_Idle].Load("Assets/animData/Gun_Idle.tka");
	animationClips[enAnimationClip_Gun_Idle].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Gun_Run].Load("Assets/animData/Gun_Run.tka");
	animationClips[enAnimationClip_Gun_Run].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Gun_Walk].Load("Assets/animData/Gun_Walk.tka");
	animationClips[enAnimationClip_Gun_Walk].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
	
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Knight.tkm", "Assets/modelData/Knight.tks",animationClips,enAnimationClip_num);

	m_skinModelRender->SetShadowCasterFlag(true);
	m_charaCon.Init(10.0f, 50.0f, m_position);

	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 0.0f,0.0f,0.0f });
	m_pointLight->SetRange(300.0f);

	m_fontRender = NewGO<prefab::CFontRender>(1);
	m_fontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)m_playerNum);
	m_fontRender->SetPosition({ -625.0f, 350.0f });
	
	//floor_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	//floor_skinModelRender->Init("Assets/modelData/mag_floor.tkm");

	m_crosshairRender = NewGO<prefab::CSpriteRender>(1);
	m_crosshairRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));
	m_crosshairRender->Init("Assets/Image/Sight.dds", 32, 32);

	m_weaponModel = NewGO<prefab::CSkinModelRender>(1);
	//m_weaponModel->Init("Assets/modelData/Knight_Weapon.tkm");
	m_weaponModel->Init("Assets/modelData/Knight_Weapon.tkm");

	m_skinModelRender->SetScale({ Scale });
	front.y = 0;

	m_stageModel = FindGO<BackGround>("background");
	return true;
}
void Knight::Update()
{
	
	if (g_pad[0]->IsTrigger(enButtonStart) || g_pad[1]->IsTrigger(enButtonStart))
	{
		m_isSceneStop = !m_isSceneStop;
	}
	//�v���C���[�ԍ����^�����Ă��Ȃ���Ή������Ȃ�
	if (m_playerNum == -1)
	{
		return;
	}

	if (m_isSceneStop == false)
	{
		
		//���͂̕ω�
		ChangeMagnetPower();

		//���W�ɉ����ĎO�p�`�̓����蔻��̏ꏊ���Z�b�g�B
		Collision();

		//�̗͓��X�e�[�^�X�̃e�L�X�g��\��(��ɉ摜�ɂ���B)
		DisplayStatus();

		//�ړ��֘A
		
		//�J�����̑O����
		front = m_position - g_camera3D[m_playerNum]->GetPosition();
		front.y = 0.0f;
		front.Normalize();

		//�J�����̉E����
		right = Cross(g_vec3AxisY, front);

		n = front.Dot(Vector3::AxisZ);//����
		angle = acosf(n);//�A�[�N�R�T�C��
		if (front.x < 0) {
			angle *= -1;
		}
		rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(rot);

		m_moveSpeed = front * g_pad[m_playerNum]->GetLStickYF() * m_Speed + right * g_pad[m_playerNum]->GetLStickXF() * m_Speed + m_Yspeed0 + m_Yspeed1;
		if (m_charaCon.IsOnGround() == false)
		{
			m_loop++;
			m_moveSpeed.y -= 0.005 * (m_loop * m_loop);
		}
		else
		{
			m_loop = 0;
		}
		if (m_moveSpeed.Length() != 0)
		{
			m_characterDirection = m_moveSpeed;
			m_characterDirection.Normalize();
		}

		//�ړ��A�N�V����
		MoveAction();


		//�U���֘A
		//�ʏ�U��
		NormalAttack();

		//�`���[�W
		Charge();

		//�ŗL�U��
		SpecialAttack();

		//�ړ��֘A2
		
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f);
		m_magPosition = m_position;
		m_magPosition.y += 50.0f;
		m_skinModelRender->SetPosition(m_position);
		////////////////////////////////////////////////////////////////////////////////////
		//�L�����N�^�[�ɂ����鎥�͂̉e��
		PlayerMagneticMove();

		//��̃{�[���̃��[���h�s����擾
		Matrix handmatrix = m_skinModelRender->GetWorldMatrixFromBoneName(L"B_R_Hand");

		//����̊g��{��

		float Scale;
		if (m_charge < 500.0f)
		{
			Scale = 1.0f;
		}
		else if (m_charge < 1000.0f)
		{
			Scale = 1.5f;
		}
		else
		{
			Scale = 2.0f;
		}

		Matrix mScale;
		mScale.m[0][0] = Scale;
		mScale.m[1][1] = Scale;
		mScale.m[2][2] = Scale;

		//��̃{�[���̍s��
		handmatrix.Multiply(mScale, handmatrix);

		m_weaponModel->SetMatrix(handmatrix);
		/*
		m_weaponModel->SetPosition({ handmatrix.m[3][0],handmatrix.m[3][1],handmatrix.m[3][2] });

		m_weaponModel->SetScale({ 0.5f,0.5f,0.5f });
		Quaternion Rot;

		Rot.SetRotation(handmatrix);

		m_weaponModel->SetRotation(Rot);
		*/
		//�}�V���K�����������Ƃ�
		HaveMachinegun();
//�O���r�e�B���������Ƃ�
		HaveGravityGrenade();		
		//��ԍX�V�B
		UpdateState();
		//�A�j���[�V�����I���B
		AnimationSelect();

		//�J�����֘A
		Character_base::Camera();
	}
}

void Knight::DisplayStatus()
{
	//�̗́A�`���[�W�A���݂̎����̎��͂̏�Ԃ̕\��
	std::wstring powerText;
	switch (m_magPower)
	{
	case -2:
		powerText = L"����Lv2";
		m_fontRender->SetColor({ 0.0f,0.0f,1.0f,1.0f });
		break;
	case -1:
		powerText = L"����Lv1";
		m_fontRender->SetColor({ 0.0f,0.5f,1.0f,1.0f });
		break;
	case 0:
		powerText = L"���͂Ȃ�";
		m_fontRender->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		break;
	case 1:
		powerText = L"�˗�Lv1";
		m_fontRender->SetColor({ 1.0f,0.4f,0.4f,1.0f });
		break;
	case 2:
		powerText = L"�˗�Lv2";
		m_fontRender->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		break;
	default:
		powerText = L"error";
	}

	wchar_t charge[256];
	swprintf_s(charge, L"%.1f", m_charge / 10.0f);

	m_fontRender->SetText(L"HP:" + std::to_wstring(m_hp)
		+ L"\nCharge:" + charge
		+ L"%\n\n\n\n\n\n\n����:" + powerText
		+ L"\n���͂̕ω��܂�:" + std::to_wstring((600 - m_timer) / 60)
		+ L"\n�ړ��A�N�V����:" + std::to_wstring(m_moveActionCount / 60));
}
void Knight::MoveAction()
{
	//�ړ��A�N�V����
	//A��������đO�̈ړ��A�N�V��������600�t���[�������Ă�����
	if (g_pad[m_playerNum]->IsTrigger(enButtonA) && m_moveActionCount == 0 && !(g_pad[m_playerNum]->IsPress(enButtonLB2)))
	{
		m_moveActionCount = 600;
		m_move_on = true;
	}
	else
	{
		m_moveActionCount--;
		if (m_moveActionCount < 0)
		{
			m_moveActionCount = 0;
		}
	}
	if (m_move_on == true&&m_move_count<10)
	{
		m_move_count++;
		m_moveSpeed += m_characterDirection * 50.0f;
		front = g_camera3D[m_playerNum]->GetForward();
		m_to_enemy = m_position - m_enemy->m_position;//��������G�܂ł̃x�N�g�� 
		m_angle_with_enemy = front.Dot(m_to_enemy);//�G�ɂǂꂾ�������Ă��邩
		m_position_with_enemy = m_to_enemy;//��������G�܂ł̃x�N�g��
		if (m_angle_with_enemy < -0.5 && m_position_with_enemy.Length() < 100&&m_move_attack==true) {//�G���O�ɂ����Ԃ��A�������߂����
			m_enemy->Damage(5*m_chargelevel);
			m_move_attack = false;//1�񂾂��_���[�W��^���郋�[�v���J�n����
		}
	}
	else
	{
		m_move_count = 0;
		m_move_on = false;
		m_move_attack = true;
	}
}

void Knight::NormalAttack()
{
	//�ʏ�U��
	if (--m_normalAttackCount < 0)
	{
		m_normalAttackCount = 0;
	}

	if (g_pad[m_playerNum]->IsTrigger(enButtonRB1) && m_normalAttackCount == 0
		&& m_GravityGrenadeHave == false)
	{
		if (m_isLock)
		{
			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_CharaNum = 0;
			bullet->m_position = m_position;
			bullet->m_position.y += 50;
			Vector3 dir = m_enemy->m_magPosition - m_magPosition;
			dir.Normalize();
			bullet->m_moveDirection = dir;
			bullet->m_velocity = 25.0f;
			bullet->m_parentNo = m_playerNum;
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

			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_CharaNum = 0;
			bullet->m_position = m_position;
			bullet->m_position.y += 50;

			if (hitFlag)
			{
				//�Ə��̎w�������ɔ�΂�
				bullet->m_moveDirection = crossPoint - m_magPosition;
				bullet->m_moveDirection.Normalize();
			}
			else
			{
				bullet->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				bullet->m_moveDirection.y = 0.0f;
				bullet->m_moveDirection.Normalize();
			}
			bullet->m_velocity = 25.0f;
			bullet->m_parentNo = m_playerNum;
		}
		if (m_MachinegunHave == false)
		{
			m_normalAttackCount = 30;
		}
		else
		{
			m_normalAttackCount = 1;
		}
		
	}
}
void Knight::Charge()
{
	//�`���[�W
	if (g_pad[m_playerNum]->IsPress(enButtonLB2) && g_pad[m_playerNum]->IsPress(enButtonRB2))
	{
		m_charge += 10.0f - m_magPower * 2.5f;
		if (m_charge < 333.3f) {
			m_chargelevel = 1;
		}
		else if (m_charge < 666.6) {
			m_chargelevel = 2;
		}
		else if (m_charge < 1000.0f) {
			m_chargelevel = 3;
		}
		else if (m_charge >= 1000.0f)
		{
			m_chargelevel = 4;
			m_charge = 1000.0f;
		}
		//���̍Đ�
		if (m_chargeSound == nullptr)
		{
			m_chargeSound = NewGO<prefab::CSoundSource>(0);
			m_chargeSound->Init(L"Assets/sound/�r�[���C�`���[�W.wav");
			m_chargeSoundVolume = 0.6f;
			m_chargeSound->SetVolume(m_chargeSoundVolume);
			m_chargeSound->Play(true);
		}
		//���x
		m_Speed = 1.0f;
	}
	else
	{
		//���x
		m_Speed = 6.0f;
		if (m_chargeSound != nullptr)
		{
			m_chargeSoundVolume -= 0.05f;
			if (m_chargeSoundVolume <= 0.0f)
			{
				DeleteGO(m_chargeSound);
				m_chargeSound = nullptr;
			}
			else
			{
				m_chargeSound->SetVolume(m_chargeSoundVolume);
			}
		}
	}

	//�`���[�W�m�F�p
	if (m_charge < 1000.0f) {
		m_pointLight->SetColor({ 0.0f,m_charge / 100,0.0f });
	}
	else
	{
		m_pointLight->SetColor({ 10.0f,0.0f,0.0f });
	}
	m_pointLight->SetPosition(m_position);
}
void Knight::SpecialAttack()
{
	//�ŗL�U��
	if (g_pad[m_playerNum]->IsPress(enButtonX)&& SpecialAttack_flag==false)
	{	
		SpecialAttack_flag = true;
		
		m_to_enemy = m_position - m_enemy->m_position;//��������G�܂ł̃x�N�g��
		m_position_with_enemy = m_to_enemy;//��������G�܂ł̃x�N�g��
		m_to_enemy.Normalize();
		front = g_camera3D[m_playerNum]->GetForward();
		front.y = 0;
		front.Normalize();
		m_angle_with_enemy =front.Dot(m_to_enemy);//�G�ɂǂꂾ�������Ă��邩
		if (m_angle_with_enemy < -0.7&&m_position_with_enemy.Length() < 100) {//�G���O�ɂ����Ԃ��A�������߂����
			loop_flag = true;//�_���[�W��^���郋�[�v���J�n����
			//���̍Đ�
			m_swordSound = NewGO<prefab::CSoundSource>(0);
			m_swordSound->Init(L"Assets/sound/����������.wav");
			m_swordSound->SetVolume(0.5f);
			m_swordSound->Play(false);

		}
		else {
			//���̍Đ�
			m_swordSound = NewGO<prefab::CSoundSource>(0);
			m_swordSound->Init(L"Assets/sound/���̑f�U��2.wav");
			m_swordSound->SetVolume(0.5f);
			m_swordSound->Play(false);

		}
		m_charge = 0;//�`���[�W���O�ɂ���
		
	}
	if(SpecialAttack_flag==true) {
		SpecialAttack_count++;
		m_moveSpeed = front * g_pad[m_playerNum]->GetLStickYF() * 1.0f + right * g_pad[m_playerNum]->GetLStickXF() * 1.0f;
	}
	if (SpecialAttack_count > 52) {
		SpecialAttack_count=0;
		SpecialAttack_flag = false;
	}
	if (loop_flag == true) {//�A�j���[�V�����ɍ��킹�Ēx��������
		loop_count++;		
	}
	
	if (loop_count > 10) {//10F����
		//���x���ɉ������_���[�W��^����
		if (m_chargelevel == 1) { 
			m_enemy->Damage(2.5 * (m_magPower + 3)); 
		}
		if (m_chargelevel == 2) {
			m_enemy->Damage(15 * (m_magPower + 3));
		}
		if (m_chargelevel == 3) {
			m_enemy->Damage(25 * (m_magPower + 3));
		}
		if (m_chargelevel == 4) {
			m_enemy->Damage(60 * (m_magPower + 3));
		}
		m_chargelevel = 1;
		loop_count = 0;
		loop_flag = false;
	}
	
}
void Knight::TryChangeStatusAttack()
{	
	if (g_pad[m_playerNum]->IsPress(enButtonX)) {
		status = enStatus_Attack;
	}
	
}
void Knight::TryChangeStatusRun()
{
	if (m_moveSpeed.LengthSq() >= 5.0f){
		status = enStatus_Run;
	}
}
void Knight::TryChangeStatusWalk()
{
	if (m_moveSpeed.LengthSq() < 5.0f && m_moveSpeed.LengthSq() > 0.0f) {
		status = enStatus_Walk;
	}
}
void Knight::TryChangeStatusIdle()
{
	if (m_moveSpeed.LengthSq() <= 0.0f) {
		status = enStatus_Idle;
	}
}
void Knight::TryChangeStatusMove()
{
	if (g_pad[m_playerNum]->IsPress(enButtonRB1)) {
		status = enStatus_Move;
	}
}
void Knight::TryChangeStatusFall()
{
	if (m_charaCon.IsOnGround() == false)
	{
		status = enStatus_Fall;
	}
}
void Knight::UpdateState()
{
	switch (status) {
	case enStatus_Attack:
		TryChangeStatusFall();
		TryChangeStatusAttack();
		if (m_skinModelRender->IsPlayingAnimation() == false)
		{
			status = enStatus_Idle;
		}
		
		break;
	case enStatus_Move:
		TryChangeStatusMove();
		if (m_skinModelRender->IsPlayingAnimation() == false)
		{
			status = enStatus_Idle;
		}
		TryChangeStatusFall();
		break;
	case enStatus_Run:
		TryChangeStatusAttack();	
		TryChangeStatusMove();
		TryChangeStatusWalk();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		break;
	case enStatus_Walk:
		TryChangeStatusAttack();
		TryChangeStatusMove();	
		TryChangeStatusRun();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		break;
	case enStatus_Idle:
		TryChangeStatusAttack();
		TryChangeStatusRun();		
		TryChangeStatusMove();
		TryChangeStatusWalk();
		TryChangeStatusFall();
		break;
	case enStatus_Fall:
		TryChangeStatusAttack();
		TryChangeStatusRun();
		TryChangeStatusMove();
		TryChangeStatusWalk();
		TryChangeStatusFall();
		break;
	}
}
void Knight::AnimationSelect()
{
	m_skinModelRender->m_animation_speed = 1.0;
	
	if (m_MachinegunHave == false) {
		switch (status) {
		case enStatus_Attack:			
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
		case enStatus_Move:
			m_skinModelRender->m_animation_speed = 4.0;
			m_skinModelRender->PlayAnimation(enAnimationClip_Move);

			break;
		case enStatus_Fall:
			m_skinModelRender->PlayAnimation(enAnimationClip_Fall);
			break;
		}
	}
	else
	{
		switch (status) {
		case enStatus_Attack:
			status = enStatus_Run;
			//m_skinModelRender->PlayAnimation(enAnimationClip_Attack);
			break;
		case enStatus_Run:
			m_skinModelRender->PlayAnimation(enAnimationClip_Gun_Run);
			break;
		case enStatus_Walk:
			m_skinModelRender->PlayAnimation(enAnimationClip_Gun_Walk);
			break;

		case enStatus_Idle:
			m_skinModelRender->PlayAnimation(enAnimationClip_Gun_Idle);

			break;
		case enStatus_Move:
			status = enStatus_Run;
			//m_skinModelRender->m_animation_speed = 4.0;
			//m_skinModelRender->PlayAnimation(enAnimationClip_Move);

			break;
		}
	}
}
void Knight::HaveMachinegun()
{

	if (m_MachinegunHave == true)
	{
		if (m_Machinegun_deletetime > 0 && m_Machinegun_bulletNum > 0) {
			m_Machinegun_deletetime--;
			if (g_pad[m_playerNum]->IsTrigger(enButtonRB1))
			{
				m_Machinegun_bulletNum--;
			}
		}
		else
		{
			m_MachinegunHave = false;
			m_Machinegun_deletetime = 500;
			m_Machinegun_bulletNum = 100;
			m_weaponModel->Init("Assets/modelData/Knight_Weapon.tkm");			
		}
	}
}
void Knight::HaveGravityGrenade()
{
	if (m_GravityGrenadeHave == true)
	{
		if (m_GravityGrenade_deletetime > 0 && m_GravityGrenadeUse == false) {
			m_GravityGrenade_deletetime--;
			if (g_pad[m_playerNum]->IsTrigger(enButtonRB1))
			{
				if (m_isLock)
				{
					UsedGravityGrenade* usedGravityGrenade = NewGO<UsedGravityGrenade>(0, "usedGravityGrenade");
					usedGravityGrenade->m_CharaNum = 1;
					usedGravityGrenade->m_position = m_position;
					usedGravityGrenade->m_position.y += 50;
					Vector3 dir = m_enemy->m_magPosition - m_magPosition;
					dir.Normalize();
					usedGravityGrenade->m_moveDirection = dir;
					usedGravityGrenade->m_velocity = 25.0f;
					usedGravityGrenade->m_parentNo = m_playerNum;
				}
				else
				{
					Vector3 testRayDir = g_camera3D[m_playerNum]->GetForward();
					Vector3 testRayStart = g_camera3D[m_playerNum]->GetPosition();
					Vector3 testRayEnd = testRayStart + testRayDir * 10000.0f;

					Vector3 crossPoint;

					bool hitFlag = m_stageModel->isLineHitModel(testRayStart, testRayEnd, crossPoint);

					UsedGravityGrenade* usedGravityGrenade = NewGO<UsedGravityGrenade>(0, "usedGravityGrenade");
					usedGravityGrenade->m_CharaNum = 1;
					usedGravityGrenade->m_position = m_position;
					usedGravityGrenade->m_position.y += 50;

					if (hitFlag)
					{
						//�Ə��̎w�������ɔ�΂�
						usedGravityGrenade->m_moveDirection = crossPoint - m_magPosition;
						usedGravityGrenade->m_moveDirection.Normalize();
					}
					else
					{
						usedGravityGrenade->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
						usedGravityGrenade->m_moveDirection.y = 0.0f;
						usedGravityGrenade->m_moveDirection.Normalize();
					}
					usedGravityGrenade->m_velocity = 25.0f;
					usedGravityGrenade->m_parentNo = m_playerNum;
				}
				m_GravityGrenadeUse = true;
			}
		}
		else
		{
			m_GravityGrenadeHave = false;
			m_GravityGrenade_deletetime = 500;
			m_GravityGrenadeUse = false;
			
			m_weaponModel->Init("Assets/modelData/Knight_Weapon.tkm");
		}
	}
}