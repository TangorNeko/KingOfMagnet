#include "stdafx.h"
#include "Character_base.h"
#include "Bullet.h"
#include "ChargeShot.h"
#include <string>
#include "Knight.h"

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
	animationClips[enAnimationClip_Idle].SetLoopFlag(false);	//���[�v���[�V�����ɂ���B
	animationClips[enAnimationClip_Run].Load("Assets/animData/Knight_Run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Knight.tkm", "Assets/modelData/Knight.tks",animationClips,enAnimationClip_num);

	m_charaCon.Init(10.0f, 50.0f, m_position);

	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 0.0f,0.0f,0.0f });
	m_pointLight->SetRange(300.0f);

	m_fontRender = NewGO<prefab::CFontRender>(1);
	m_fontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)m_playerNum);
	m_fontRender->SetPosition({ -625.0f, 350.0f });
	
	//floor_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	//floor_skinModelRender->Init("Assets/modelData/mag_floor.tkm");

	m_weaponModel = NewGO<prefab::CSkinModelRender>(1);
	m_weaponModel->Init("Assets/modelData/Knight_Weapon.tkm");

	m_skinModelRender->SetScale({ Scale });
	front.y = 0;
	return true;
}
void Knight::Update()
{
	//��ԍX�V�B
	UpdateState();
	//�A�j���[�V�����I���B
	AnimationSelect();

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
		Vector3 front = m_position - g_camera3D[m_playerNum]->GetPosition();
		front.y = 0.0f;
		front.Normalize();

		//�J�����̉E����
		Vector3 right = Cross(g_vec3AxisY, front);

		float n = front.Dot(Vector3::AxisZ);//����
		float angle = acosf(n);//�A�[�N�R�T�C��
		if (front.x < 0) {
			angle *= -1;
		}
		rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(rot);

		m_moveSpeed = front * g_pad[m_playerNum]->GetLStickYF() * 3.0f + right * g_pad[m_playerNum]->GetLStickXF() * 3.0f;

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
		m_moveSpeed.y = -2.0f;

		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f);
		m_magPosition = m_position;
		m_magPosition.y += 50.0f;
		m_skinModelRender->SetPosition(m_position);

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

	m_fontRender->SetText(L"HP:" + std::to_wstring(m_hp)
		+ L"\nCharge:" + std::to_wstring(m_charge / 10.0f)
		+ L"%\n\n\n\n\n\n\n\n\n\n\n\n\n����:" + powerText
		+ L"\n���͂̕ω��܂�:" + std::to_wstring((600 - m_timer) / 60)
		+ L"�@�@�@�@�@�@�@�@  �ړ��A�N�V����:" + std::to_wstring(m_moveActionCount / 60));
}
void Knight::MoveAction()
{
	//�ړ��A�N�V����
	if (g_pad[m_playerNum]->IsTrigger(enButtonA) && m_moveActionCount == 0 && !(g_pad[m_playerNum]->IsPress(enButtonLB2)))
	{
		//m_skinModelRender->SetScale({ 0.0f,0.0f,0.0f });
		m_moveActionCount = 20;
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
		to_enemy = m_position - m_enemy->m_position;//��������G�܂ł̃x�N�g�� 
		angle_with_enemy = front.Dot(to_enemy);//�G�ɂǂꂾ�������Ă��邩
		position_with_enemy = to_enemy;//��������G�܂ł̃x�N�g��
		if (angle_with_enemy < -0.5 && position_with_enemy.Length() < 100&&m_move_attack==true) {//�G���O�ɂ����Ԃ��A�������߂����
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

	if (g_pad[m_playerNum]->IsTrigger(enButtonRB1) && m_normalAttackCount == 0)
	{
		if (m_isLock)
		{
			Bullet* bullet = NewGO<Bullet>(0, "bullet");
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
			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_position = m_position;
			bullet->m_position.y += 50;
			bullet->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
			bullet->m_moveDirection.y = 0.0f;
			bullet->m_moveDirection.Normalize();
			bullet->m_velocity = 25.0f;
			bullet->m_parentNo = m_playerNum;
		}

		m_normalAttackCount = 30;
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
		

		m_moveSpeed = { 0.0f,0.0f,0.0f };
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
	if (g_pad[m_playerNum]->IsPress(enButtonX))
	{	 		
		//�U���A�j���[�V����
		m_skinModelRender->PlayAnimation(enAnimationClip_Attack);

		to_enemy = m_position - m_enemy->m_position;//��������G�܂ł̃x�N�g��
		position_with_enemy = to_enemy;//��������G�܂ł̃x�N�g��
		to_enemy.Normalize();
		front = g_camera3D[m_playerNum]->GetForward();
		front.y = 0;
		front.Normalize();
		angle_with_enemy =front.Dot(to_enemy);//�G�ɂǂꂾ�������Ă��邩
		if (angle_with_enemy < -0.7&&position_with_enemy.Length() < 100) {//�G���O�ɂ����Ԃ��A�������߂����
			loop_flag = true;//�_���[�W��^���郋�[�v���J�n����
		}		
		m_charge = 0;//�`���[�W���O�ɂ���
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
void Knight::UpdateState()
{
	switch (status) {
	case enStatus_Attack:
		if (m_skinModelRender->IsPlayingAnimation() == false) {
			status = enStatus_Idle;
		}
		break;
	case enStatus_Run:
		TryChangeStatusAttack();
		break;
	case enStatus_Idle:
		TryChangeStatusAttack();
		break;
	case enStatus_Walk:
		TryChangeStatusAttack();
		break;
	}
}
void Knight::AnimationSelect()
{
	switch (status) {
	case enStatus_Attack:
		m_skinModelRender->PlayAnimation(enAnimationClip_Attack);
		break;
	case enStatus_Run:
		m_skinModelRender->PlayAnimation(enAnimationClip_Run);
		break;
	case enStatus_Idle:
		m_skinModelRender->PlayAnimation(enAnimationClip_Idle);
		break;
	case enStatus_Walk:
		m_skinModelRender->PlayAnimation(enAnimationClip_Walk);
		break;
	}
#if 0
	if ()
	{
		m_anim_num = enAnimationClip_Attack;
	}
	else if (m_moveSpeed.LengthSq() > 0.0f * 0.0f)
	{
		m_anim_num = enAnimationClip_Run;
	}
	
	else if (m_moveSpeed.LengthSq() <= 0.0f)
	{
		m_anim_num = enAnimationClip_Idle;
	}
	


	if (m_anim_num == 0)
	{
		m_skinModelRender->PlayAnimation(enAnimationClip_Attack);
	}
	else if (m_anim_num == 1)
	{
		m_skinModelRender->PlayAnimation(enAnimationClip_Run);
	}
	else
	{
		m_skinModelRender->PlayAnimation(enAnimationClip_Idle);
	}
#endif
}
