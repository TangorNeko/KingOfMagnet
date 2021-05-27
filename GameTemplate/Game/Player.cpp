#include "stdafx.h"
#include "Player.h"

#include "BackGround.h"
#include "Debris.h"
#include "Bomb.h"
#include "DamageDisplay.h"
#include "GravityBullet.h"
#include "SampleScene.h"
#include "MobiusGauge.h"
#include "ResultScene.h"

Player::Player()
{
}
Player::~Player()
{
	DeleteGO(m_skinModelRender);

	if (m_bulletNumFont != nullptr)
	{
		DeleteGO(m_bulletNumFont);
	}
	if (m_bulletNumFont2 != nullptr)
	{
		DeleteGO(m_bulletNumFont2);
	}

	if (m_crosshairRender != nullptr)
	{
		DeleteGO(m_crosshairRender);
	}

	if (m_HPBarSpriteRender != nullptr)
	{
		DeleteGO(m_HPBarSpriteRender);
	}

	if (m_HPBarDarkSpriteRender != nullptr)
	{
		DeleteGO(m_HPBarDarkSpriteRender);
	}

	if (m_mobiusGauge != nullptr)
	{
		DeleteGO(m_mobiusGauge);
	}

	m_magEffect[0]->Stop();
	m_magEffect[1]->Stop();
	m_burstEffect->Stop();
	m_hitEffect->Stop();
	m_SPEffect->Stop();
	m_SPGaugeMaxEffect->Stop();
	DeleteGO(m_magEffect[0]);
	DeleteGO(m_magEffect[1]);
	DeleteGO(m_burstEffect);
	DeleteGO(m_hitEffect);
	DeleteGO(m_SPEffect);
	DeleteGO(m_SPGaugeMaxEffect);
	DeleteGO(m_SPFirstEffectRed);
	DeleteGO(m_SPFirstEffectBlue);

	DeleteGO(m_spotLight);

	if (m_ChargeSPFontRender != nullptr)
		DeleteGO(m_ChargeSPFontRender);	
	DeleteGO(m_winnerFont);
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
	animationClips[enAnimationClip_SpecialAttack].Load("Assets/animData/Mage_RangeAttack.tka");
	animationClips[enAnimationClip_SpecialAttack].SetLoopFlag(false);
	animationClips[enAnimationClip_Burst].Load("Assets/animData/Mage_SpecialAttack2.tka");
	animationClips[enAnimationClip_Burst].SetLoopFlag(false);
	animationClips[enAnimationClip_Hit].Load("Assets/animData/Mage_Hit.tka");
	animationClips[enAnimationClip_Hit].SetLoopFlag(false);
	animationClips[enAnimationClip_Death].Load("Assets/animData/Mage_Death.tka");
	animationClips[enAnimationClip_Death].SetLoopFlag(false);
	animationClips[enAnimationClip_Winner].Load("Assets/animData/Mage_Winner.tka");
	animationClips[enAnimationClip_Winner].SetLoopFlag(false);	//���[�v���[�V�����ɂ���B
	
	//���f���̏�����
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	if (m_playerNum == 0)//�v���C���[�̐F��ς���
		m_skinModelRender->Init("Assets/modelData/Player1.tkm", "Assets/modelData/Mage.tks", animationClips, enAnimationClip_num);
	if (m_playerNum == 1)
		m_skinModelRender->Init("Assets/modelData/Player2.tkm", "Assets/modelData/Mage.tks", animationClips, enAnimationClip_num);

	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetPosition(m_position);

	//�L�����R���̏�����
	m_charaCon.Init(10.0f, 50.0f, m_position);

	//�c�e���\���̏�����
	m_bulletNumFont = NewGO<prefab::CFontRender>(6);
	m_bulletNumFont->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_bulletNumFont->SetShadowFlag(true);
	m_bulletNumFont->SetShadowColor({ 0,0,0,1 });
	m_bulletNumFont->SetShadowOffset(3);
	if (m_playerNum == 0)
	{
		m_bulletNumFont->SetPosition({ -170.0f, -270.0f });
	}
	else
	{
		m_bulletNumFont->SetPosition({ 60.0f, -270.0f });
	}
	m_bulletNumFont->SetScale({ 1.0f,1.0f });
	m_bulletNumFont->SetColor({ 1.0f,1.0f, 1.0f,1.0f });
	m_bulletNumFont->SetText(std::to_wstring(m_holdDebrisVector.size()));

	m_bulletNumFont2 = NewGO<prefab::CFontRender>(6);
	m_bulletNumFont2->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_bulletNumFont2->SetShadowFlag(true);
	m_bulletNumFont2->SetShadowColor({ 0,0,0,1 });
	m_bulletNumFont2->SetShadowOffset(3);
	if (m_playerNum == 0)
	{
		m_bulletNumFont2->SetPosition({ -130.0f, -283.0f });
	}
	else
	{
		m_bulletNumFont2->SetPosition({ 100.0f, -283.0f });
	}
	m_bulletNumFont2->SetScale({ 0.7f,0.7f });
	m_bulletNumFont2->SetColor({ 1.0f,1.0f, 1.0f,1.0f });
	m_bulletNumFont2->SetText(L"/10");

	//�K�E�Q�[�W�̗��܂���\������t�H���g
	m_ChargeSPFontRender = NewGO<prefab::CFontRender>(6);
	m_ChargeSPFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_ChargeSPFontRender->SetShadowFlag(true);
	m_ChargeSPFontRender->SetShadowColor({ 0,0,0,1 });
	m_ChargeSPFontRender->SetShadowOffset(3);
	if (m_playerNum == 0)
	{
		m_ChargeSPFontRender->SetPosition({ -553.0f, -225.0f });
	}
	else
	{
		m_ChargeSPFontRender->SetPosition({ 498.0f, -225.0f });
	}
	m_ChargeSPFontRender->SetScale({ 0.7f,0.7f });
	m_ChargeSPFontRender->SetColor({ 1.0f,1.0f, 0.0f,1.0f });
	m_ChargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");

	//�Ə��\���̏�����
	m_crosshairRender = NewGO<prefab::CSpriteRender>(1);
	m_crosshairRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));
	m_crosshairRender->Init("Assets/Image/Sight.dds", 32, 32);

	//�X�e�[�W�̃N���X���擾
	m_stageModel = FindGO<BackGround>("background");

	//HP�o�[
	m_HPBarSpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_HPBarSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);

	m_HPBarSpriteRender->Init("Assets/Image/HP_Bar.dds",308, 32);
	if (m_playerNum == 0) {
		m_HPBarSpriteRender->SetPosition({ -10.0f,325.0f,0.0f });
	}
	else if (m_playerNum == 1) {
		m_HPBarSpriteRender->SetScale({ -1.0f, 1.0f, -1.0f });
		m_HPBarSpriteRender->SetPosition({ 10.0f,325.0f,0.0f });
	}

	m_HPBarDarkSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_HPBarDarkSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);

	m_HPBarDarkSpriteRender->Init("Assets/Image/HP_Bar_Dark_12.dds", 316, 36);
	if (m_playerNum == 0) {
		m_HPBarDarkSpriteRender->SetPosition({ 290.0f,325.0f,0.0f });
		//({ 290.0f,325.0f,0.0f });
	}
	else if (m_playerNum == 1) {
		m_HPBarDarkSpriteRender->SetScale({ -1.0f, 1.0f, -1.0f });
		m_HPBarDarkSpriteRender->SetPosition({ -290.0f,325.0f,0.0f });
	}

	m_mobiusGauge = NewGO<MobiusGauge>(0);
	if (m_playerNum == 0) {
		m_mobiusGauge->SetPosition({ -525.0f,-280.0f,0.0f });
	}
	else if (m_playerNum == 1) {
		m_mobiusGauge->SetPosition({ 525.0f,-280.0f,0.0f });
	}

	//�v���C���[�̃��C�g
	m_spotLight = NewGO<prefab::CSpotLight>(0);
	m_spotLight->SetColor({ 10.0f,10.0f,10.0f });
	m_spotLight->SetRange(250.0f);
	m_spotLight->SetAngleDeg(30.0f);
	
	
	//�G�t�F�N�g�֘A
	m_magEffect[0] = NewGO<prefab::CEffect>(0);
	m_magEffect[1] = NewGO<prefab::CEffect>(0);
	m_magEffect[0]->SetScale({ 25.0f, 25.0f, 25.0f });
	m_magEffect[1]->SetScale({ 25.0f, 25.0f, 25.0f });
	if (m_magPower == 1) {
		m_magEffect[0]->Init(u"Assets/effect/�˗�.efk");
		m_magEffect[1]->Init(u"Assets/effect/�˗�.efk");
	}
	else if (m_magPower == -1) {
		m_magEffect[0]->Init(u"Assets/effect/����.efk");
		m_magEffect[1]->Init(u"Assets/effect/����.efk");
	}

	m_burstEffect = NewGO<prefab::CEffect>(0);
	m_burstEffect->SetScale({ 50.0f, 50.0f, 50.0f });

	m_hitEffect = NewGO<prefab::CEffect>(0);
	m_hitEffect->Init(u"Assets/effect/�_���[�W.efk");
	m_hitEffect->SetScale({ 10.0f, 10.0f, 10.0f });

	m_SPEffect = NewGO<prefab::CEffect>(0);
	m_SPEffect->SetScale({ 20.0f, 20.0f, 20.0f });

	m_SPGaugeMaxEffect = NewGO<prefab::CEffect>(0);
	m_SPGaugeMaxEffect->Init(u"Assets/effect/�L���s�[��.efk");
	m_SPGaugeMaxEffect->SetScale({ 8.0f, 8.0f, 8.0f });

	m_SPFirstEffectRed = NewGO<prefab::CEffect>(0);
	m_SPFirstEffectRed->SetScale({ 70.0f, 70.0f, 70.0f });
	m_SPFirstEffectRed->Init(u"Assets/effect/�˗̓`���[�W.efk");

	m_SPFirstEffectBlue = NewGO<prefab::CEffect>(0);
	m_SPFirstEffectBlue->SetScale({ 70.0f, 70.0f, 70.0f });
	m_SPFirstEffectBlue->Init(u"Assets/effect/���̓`���[�W.efk");
	//�����܂ŃG�t�F�N�g

	m_gameScene = FindGO<SampleScene>("gamescene");
	return true;
}

void Player::Update()
{
	//�|�[�Y���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPause)
	{
		return;
	}

	//�I�[�v�j���O�J�����B
	if (m_gameScene->GetGameState() == SampleScene::GameState::enBirdseye)
	{
		OpeningCamera();
	}
	//�X�^�[�g�_�E���J�E���g�_�E����
	else if (m_gameScene->GetGameState() == SampleScene::GameState::enStartCountDown)
	{
		//�J�����̈ړ��B
		Camera();

		//�L������O�Ɍ�������

		//�J�����̑O����
		m_front = m_position - g_camera3D[m_playerNum]->GetPosition();
		m_front.y = 0.0f;
		m_front.Normalize();

		//�J�����̉E����
		right = Cross(g_vec3AxisY, m_front);

		n = m_front.Dot(Vector3::AxisZ);//����
		float angle = acosf(n);//�A�[�N�R�T�C��
		if (m_front.x < 0) {
			angle *= -1;
		}
		m_rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(m_rot);

		//��ԍX�V�B
		UpdateState();
		//�A�j���[�V�����I���B
		AnimationSelect();
	}
	else if(m_gameScene->GetGameState() == SampleScene::GameState::enPlaying)
	{		
		//�̗͓��X�e�[�^�X�̃e�L�X�g��\��(��ɉ摜�ɂ���B)
		DisplayStatus();

		//���W�ɉ����ĎO�p�`�̓����蔻��̏ꏊ���Z�b�g�B
		Collision();

		//���͂̕ω�
		ChangeMagnetPower();
		if (m_isKnockBack == true)
		{
			KnockBack();
		}
		else if (m_isKnockBack == false)
		{
			//�ړ�
			Move();
			//�K�E�Z
			SpecialAttack();
			//�ێ����Ă���K���L�̈ʒu�𐧌䂷��
			HoldDebris();
			//�ێ����Ă��锚�e�̈ʒu�𐧌䂷��
			HoldBomb();
			//�o�[�X�g���g�p���Ă���?
			if (m_isBurst == true)
			{
				MagneticBurst();
			}
			else
			{
				MagneticBehavior();
			}

			//���e�𓊂���
			ThrowBomb();

			//�O���l�[�h�p�B���ł��B
			/*if (g_pad[m_playerNum]->IsTrigger(enButtonY))
			{
				Bomb* debris = NewGO<Bomb>(0, "bomb");
				debris->m_bombShape = Bomb::enIncendiaryGrenade;
				debris->m_bombState = Bomb::enDrop;
				debris->m_parent = this;
				debris->m_position = m_magPosition;
				debris->m_moveDirection = m_characterDirection;
			}*/
		}

		//�U����̌��̃^�C�}�[�����炵�Ă���
		m_attackCount--;
		//�U���̃N�[���^�C�����I���ƈړ����x��߂�
		if (m_attackCount <= 0 && m_isBurst == false)
		{
			m_attackCount = 0;
			m_isAttacking = false;
			m_characterSpeed = 6.0;
		}
		//��ԍX�V�B
		UpdateState();
		//�A�j���[�V�����I���B
		AnimationSelect();

		//�J�����̈ړ�
		Camera();

		//�˗́E���̓G�t�F�N�g			

		//���̓G�t�F�N�g���Đ�
		if (m_magEffectCallCount == 40) {
			m_magEffect[1]->Play();
		}
		else if (m_magEffectCallCount <= 0) {
			m_magEffect[0]->Play();
			m_magEffectCallCount = 80;
		}
		m_magEffectCallCount--;

		m_magEffect[0]->SetPosition(m_position);
		m_magEffect[1]->SetPosition(m_position);
			
		if(m_SPGaugeMaxEffect->IsPlay())
			m_SPGaugeMaxEffect->SetPosition({ m_position.x,m_position.y + 50.0f, m_position.z });
	
	}
	else if(m_gameScene->GetGameState() == SampleScene::GameState::enResult)
	{
		//�t�@�C�i���q�b�g�J�����B
		FinalHit();
		//��ԍX�V�B
		UpdateState();
		//�A�j���[�V�����I���B
		AnimationSelect();
	}
	else if (m_gameScene->GetGameState() == SampleScene::GameState::enDraw)
	{
		//�ێ����Ă���K���L�𕂗V������B
		HoldDebris();

		//�ێ����Ă��锚�e�𕂗V������B
		HoldBomb();

		//�J�����̈ړ��B
		Camera();

		//�L������O�Ɍ�������

		//�J�����̑O����
		m_front = m_position - g_camera3D[m_playerNum]->GetPosition();
		m_front.y = 0.0f;
		m_front.Normalize();

		//�J�����̉E����
		right = Cross(g_vec3AxisY, m_front);

		n = m_front.Dot(Vector3::AxisZ);//����
		float angle = acosf(n);//�A�[�N�R�T�C��
		if (m_front.x < 0) {
			angle *= -1;
		}
		m_rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(m_rot);

		m_animStatus = enStatus_Idle;
		//�A�j���[�V�����I���B
		AnimationSelect();
	}
	
}
//�̗́A���r�E�X�Q�[�W�̕\��
void Player::DisplayStatus()
{
	//�̗́A�`���[�W�A���݂̎����̎��͂̏�Ԃ̕\��

	//���r�E�X�Q�[�W�̐F�����͂��猈��
	if (m_magPower == 1)
	{
		m_mobiusGauge->m_isRed = true;
	}
	else
	{
		m_mobiusGauge->m_isRed = false;
	}

	//���r�E�X�Q�[�W�Ɍ��݂̕K�E�Z�̃`���[�W�ʂ�n��
	m_mobiusGauge->m_charge = m_charge;

	m_mobiusGauge->m_spCharge = m_specialAttackGauge;
}

//�ړ�
void Player::Move()
{
	Vector3 oldPos = m_position;

	//�J�����̑O����
	m_front = m_position - g_camera3D[m_playerNum]->GetPosition();
	m_front.y = 0.0f;
	m_front.Normalize();

	//�J�����̉E����
	right = Cross(g_vec3AxisY, m_front);

	n = m_front.Dot(Vector3::AxisZ);//����
	float angle = acosf(n);//�A�[�N�R�T�C��
	if (m_front.x < 0) {
		angle *= -1;
	}
	m_rot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(m_rot);
	m_moveSpeed = m_front * g_pad[m_playerNum]->GetLStickYF() * m_characterSpeed + right * g_pad[m_playerNum]->GetLStickXF() * m_characterSpeed;
	if (m_charaCon.IsOnGround() == false)
	{
		if (m_fallLoop < 75)
		{
			m_fallLoop++;
		}
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

	//���ɗ��������̏���
	if (m_position.y <= -750.0f) {
		m_LandingNum++;//��������
		Damage(100);

		//�G����ł��������X�|�[���n�_�Ɉړ�����B
		Vector3 respawnPoint = m_stageModel->GetRespawnPoint(m_enemy->m_position);
		m_position = respawnPoint;
		m_charaCon.SetPosition(m_position);
		m_skinModelRender->SetPosition(m_position);

		//���X�|�[�������̂ŗ��������p�̃J�E���g�����Z�b�g�B
		m_fallLoop = 0;
	}

	//����
	if (g_pad[m_playerNum]->GetLStickXF() || g_pad[m_playerNum]->GetLStickYF())
	{
		m_footstepsTimer++;
		
		if (m_animStatus == enStatus_Walk) {
			if (m_footstepsTimer >= 25)
			{
				prefab::CSoundSource* ssShoot = NewGO<prefab::CSoundSource>(0);;
				ssShoot->Init(L"Assets/sound/����.wav");
				ssShoot->SetVolume(0.2f);
				ssShoot->Play(false);

				m_footstepsTimer = 0;
			}
		}
		if (m_animStatus == enStatus_Run) {
			if (m_footstepsTimer >= 17)
			{
				prefab::CSoundSource* ssShoot = NewGO<prefab::CSoundSource>(0);;
				ssShoot->Init(L"Assets/sound/����.wav");
				ssShoot->SetVolume(0.2f);
				ssShoot->Play(false);
				
				m_footstepsTimer = 0;
			}
		}
	}
	else
		m_footstepsTimer = 0;

	if (oldPos.y >= -50.0f && m_position.y < -50.0f)
	{
		prefab::CSoundSource* ssShoot = NewGO<prefab::CSoundSource>(0);;
		ssShoot->Init(L"Assets/sound/������.wav");
		ssShoot->SetVolume(0.5f);
		ssShoot->Play(false);
	}	
}

//�U��
void Player::Attack()
{
	if (g_pad[m_playerNum]->IsPress(enButtonRB1) && m_attackCount == 0)
	{
		//�K���L����ł������Ă���Ȃ�
		if (m_holdDebrisVector.empty() == false)
		{
			m_AttackNum++;//�U����
			//����炷
			prefab::CSoundSource* ssShoot = NewGO<prefab::CSoundSource>(0);;
			ssShoot->Init(L"Assets/sound/�V���[�g��.wav");
			ssShoot->SetVolume(0.5f);
			ssShoot->Play(false);

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
			
			//���̏ꏊ�Ɍ������Č���(GetShootPoint�̒��ł̎Q�Ǝ󂯎��p)
			Vector3 crossPoint;
			//���ː���v�Z�B
			bool hitFlag = GetShootPoint(crossPoint);

			//���ː�ɓG�������̓X�e�[�W�̃��f��������B
			if (hitFlag)
			{
				//�Ə��̎w�������ɔ�΂�
				debris->m_moveDirection = crossPoint - debris->m_position;
				debris->m_moveDirection.Normalize();
			}
			else //�Ȃ��B
			{
				debris->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				debris->m_moveDirection.y = 0.0f;
				debris->m_moveDirection.Normalize();
			}

			//���˂����K���L��ێ����X�g����폜
			m_holdDebrisVector.erase(m_holdDebrisVector.begin());

			//�e�L�X�g�X�V
			m_bulletNumFont->SetText(std::to_wstring(m_holdDebrisVector.size()));
			if (m_playerNum == 0)
			{
				if (m_holdDebrisVector.size() >= 10)
					m_bulletNumFont->SetPosition({ -207.0f, -270.0f });

				else 
					m_bulletNumFont->SetPosition({ -170.0f, -270.0f });
			}
			else
			{
				if (m_holdDebrisVector.size() >= 10)
					m_bulletNumFont->SetPosition({ 23.0f, -270.0f });
				else
					m_bulletNumFont->SetPosition({ 60.0f, -270.0f });
			}
		}

	}
}

//�K�E�Z
void Player::SpecialAttack()
{
	//���̓��[�h�̕K�E�Z�̃^�C�~���O���w���ł���B
	if (m_isGravityBulletAttack == true)
	{
		m_isGravityBulletAttack = false;
	}
	else if (g_pad[m_playerNum]->IsTrigger(enButtonLB3))
	{
		m_isGravityBulletAttack = true;
	}

	//�K�E�Z�|�C���g�����܂��Ă��ă{�^������������
	//�A�j���[�V�����ɔ��˃^�C�~���O�����킹��B
	if (m_specialAttackGauge >= 100 && g_pad[m_playerNum]->IsTrigger(enButtonLB3))
	{
		m_SpecialAttackOn = true;		//�A�j���[�V������K�E�Z�ɂ���B
		m_specialShotFlag = true;
	}
	if (m_specialShotFlag == true)
	{
		//���˂���O�ɁA�`���[�W����悤�ȃG�t�F�N�g
		switch (m_magPower)
		{
		case -1:
			m_SPFirstEffectBlue->SetPosition({ m_position.x, m_position.y + 50.0f, m_position.z });
			if (m_specialShotCount == 0)
				m_SPFirstEffectBlue->Play();
			break;
		case 1:
			m_SPFirstEffectRed->SetPosition({ m_position.x, m_position.y + 50.0f, m_position.z });
			if (m_specialShotCount == 0)
				m_SPFirstEffectRed->Play();		
			break;
		}
		if (m_specialShotCount == 0)
		{
			prefab::CSoundSource* ssSPCharge = NewGO<prefab::CSoundSource>(0);
			ssSPCharge->Init(L"Assets/sound/�p���[�`���[�W.wav");
			ssSPCharge->Play(false);
		}

		m_specialShotCount += 1;

		//���ˑO�Ƀ_���[�W���󂯂���L�����Z��
		if (m_HitOn == true) 
		{
			//�Q�[�W��0�ɁB
			m_specialAttackGauge = 0;
			m_specialShotFlag = false;
			m_specialShotCount = 0;
			if (m_playerNum == 0)
			{
				m_ChargeSPFontRender->SetPosition({ -553.0f, -225.0f });
				m_ChargeSPFontRender->SetScale({ 0.7f,0.7f });
				m_ChargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
			}
			else
			{
				m_ChargeSPFontRender->SetPosition({ 498.0f, -225.0f });
				m_ChargeSPFontRender->SetScale({ 0.7f,0.7f });
				m_ChargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
			}
		}
	}

	if (m_specialShotCount >= 35) 
	{
		//���͂Ȃ�
		if (m_magPower == -1)
		{
			//m_SPFirstEffectBlue->Stop();
			//����炷
			prefab::CSoundSource* ssSPShot = NewGO<prefab::CSoundSource>(0);
			ssSPShot->Init(L"Assets/sound/���͒e����.wav");
			ssSPShot->SetVolume(0.8f);
			ssSPShot->Play(false);

			//���˃G�t�F�N�g
			m_SPEffect->Init(u"Assets/effect/���͒e����.efk");
			m_SPEffect->SetPosition({
				m_position.x + m_front.x * 50.0f,
				m_position.y + 50.0f,
				m_position.z + m_front.z * 50.0f
				});
			m_SPEffect->Play();

			GravityBullet* gravityBullet = NewGO<GravityBullet>(0, "gravitybullet");
			gravityBullet->m_position = m_magPosition;
			gravityBullet->m_parent = this;

			//���̏ꏊ�Ɍ������Č���(GetShootPoint�̒��ł̎Q�Ǝ󂯎��p)
			Vector3 crossPoint;
			//���ː���v�Z�B
			bool hitFlag = GetShootPoint(crossPoint);

			if (hitFlag)
			{
				//�Ə��̎w�������ɔ�΂�
				gravityBullet->m_moveDirection = crossPoint - gravityBullet->m_position;
				gravityBullet->m_moveDirection.Normalize();
			}
			else
			{
				gravityBullet->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				gravityBullet->m_moveDirection.y = 0.0f;
				gravityBullet->m_moveDirection.Normalize();
			}

			//�������̂ŕK�E�Z�Q�[�W��0��
			m_specialAttackGauge = 0;
			m_specialShotFlag = false;
			m_specialShotCount = 0;
			if (m_playerNum == 0)
			{
				m_ChargeSPFontRender->SetPosition({ -553.0f, -225.0f });
				m_ChargeSPFontRender->SetScale({ 0.7f,0.7f });
				m_ChargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
			}
			else
			{
				m_ChargeSPFontRender->SetPosition({ 498.0f, -225.0f });
				m_ChargeSPFontRender->SetScale({ 0.7f,0.7f });
				m_ChargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
			}
		}
		else//�˗͂Ȃ�
		{
			//m_SPFirstEffectRed->Stop();
			//�e��1���ł������Ă�?
			if (m_holdDebrisVector.size() != 0)
			{
				//����炷
				prefab::CSoundSource* ssSPShot = NewGO<prefab::CSoundSource>(0);;
				ssSPShot->Init(L"Assets/sound/�C�e1.wav");
				ssSPShot->Play(false);

				//���˃G�t�F�N�g
				m_SPEffect->Init(u"Assets/effect/�˗͒e����.efk");
				m_SPEffect->SetPosition({ 
					m_position.x + m_front.x * 50.0f,
					m_position.y + 50.0f,
					m_position.z + m_front.z * 50.0f
					});
				m_SPEffect->Play();

				//���̏ꏊ�Ɍ������Č���(GetShootPoint�̒��ł̎Q�Ǝ󂯎��p)
				Vector3 crossPoint;
				//���ː���v�Z�B
				bool hitFlag = GetShootPoint(crossPoint);

				if (hitFlag)
				{
					//�Ə��̎w�������ɔ�΂�
					for (auto debris : m_holdDebrisVector)
					{
						m_AttackNum++;//�U����
						debris->m_debrisState = Debris::enBullet;
						debris->m_moveDirection = crossPoint - debris->m_position;
						debris->m_moveDirection.Normalize();
					}
				}
				else
				{
					for (auto debris : m_holdDebrisVector)
					{
						m_AttackNum++;//�U����
						debris->m_debrisState = Debris::enBullet;
						debris->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
						debris->m_moveDirection.y = 0.0f;
						debris->m_moveDirection.Normalize();
					}
				}

				m_holdDebrisVector.clear();

				//�e�L�X�g�X�V
				m_bulletNumFont->SetText(std::to_wstring(m_holdDebrisVector.size()));
				if (m_playerNum == 0)					
					m_bulletNumFont->SetPosition({ -170.0f, -270.0f });
				
				else
					m_bulletNumFont->SetPosition({ 60.0f, -270.0f });
				


				//�������̂ŕK�E�Z�Q�[�W��0��
				m_specialAttackGauge = 0;
				m_specialShotFlag = false;
				m_specialShotCount = 0;
				if (m_playerNum == 0)
				{
					m_ChargeSPFontRender->SetPosition({ -553.0f, -225.0f });
					m_ChargeSPFontRender->SetScale({0.7f,0.7f});
					m_ChargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
				}
				else
				{
					m_ChargeSPFontRender->SetPosition({ 498.0f, -225.0f });
					m_ChargeSPFontRender->SetScale({ 0.7f,0.7f });
					m_ChargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
				}
			}
			else
			{
				//1�����e�����ĂȂ�����s���ɂ���B�G���[��?�Q�[�W������Ȃ��Ă���
				m_specialShotFlag = false;
				m_specialShotCount = 0;
			}
		}
	}
}

void Player::ThrowBomb()
{
	if (g_pad[m_playerNum]->IsPress(enButtonRB2) && m_attackCount == 0)
	{
		//���e����ł������Ă���Ȃ�
		if (m_holdBombVector.empty() == false)
		{
			m_AttackNum++;//�U����
			//����炷
			prefab::CSoundSource* ssThrow = NewGO<prefab::CSoundSource>(0);;
			ssThrow->Init(L"Assets/sound/�����鉹.wav");
			ssThrow->Play(false);

			//�I�����Ă��锚�e�𔭎�
			auto debris = m_holdBombVector.begin() + m_selectBombNo;
			//�ێ��������e�𔭎˃��[�h�ɂ���
			(*debris)->m_bombState = Bomb::enBullet;

			//�L�����N�^�[�̃X�s�[�h��x������B
			m_characterSpeed = 0.5f;

			//�U�����̃t���O���I��
			m_isAttacking = true;

			//�U���̌��̎�������
			m_attackCount = 60;

			Vector3 front = g_camera3D[m_playerNum]->GetForward();
			front.y = 0;
			front.Normalize();
			(*debris)->m_moveDirection = front;

			//���˂������e��ێ����X�g����폜
			m_holdBombVector.erase(m_holdBombVector.begin() + m_selectBombNo);

			//�I�����Ă��锚�e�̃i���o�[�����Z�b�g�B
			m_selectBombNo = 0;
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

//�ێ����Ă��锚�e�𕂗V������B
void Player::HoldBomb()
{
	//�I�����锚�e������
	if (g_pad[m_playerNum]->IsTrigger(enButtonRight))
	{
		//�I�����Ă��锚�e�̔ԍ����R���e�i�̃T�C�Y�𒴂��Ă�����
		if (++m_selectBombNo >= m_holdBombVector.size())
		{
			//��������0�ɂȂ�
			m_selectBombNo = 0;
		}
	}
	else if (g_pad[m_playerNum]->IsTrigger(enButtonLeft))
	{
		//�I�����Ă��锚�e�̔ԍ����}�C�i�X�ɂȂ��Ă�����
		if (--m_selectBombNo < 0)
		{
			//�������ăR���e�i�̃T�C�Y-1�ɂȂ�
			m_selectBombNo = m_holdBombVector.size() - 1;
		}
	}

	//��]�̒��S��ݒ肷��B
	Vector3 centerOfRotation = m_position;
	centerOfRotation.y += 100.0f;

	//��]�̒��S���v���C���[�����ɁB
	Vector3 cameraDir = g_camera3D[m_playerNum]->GetForward();
	cameraDir.y = 0;
	cameraDir.Normalize();
	centerOfRotation -= cameraDir * 30.0f;

	Vector3 toDebris = { 0.0f,30.0f,0.0f };

	//�ێ����Ă��锚�e��1�ȏ゠���
	if (m_holdBombVector.empty() == false)
	{
		//360�x���K���L�̐�-1�̐��Ŋ�����1������̊p�x�����߂�
		//���ɔ��˂���1�͐g�̂̑O�ɒu���̂�-1
		int vectorSize = m_holdBombVector.size();
		float degPerOneDebris = 360.0f / vectorSize;

		//autoFor���̒��ł��񐔂��J�E���g���邽�߂̕ϐ�
		int i = 0;
		for (auto debris : m_holdBombVector)
		{
			//Y������30�̃x�N�g������]�����ĉ�]�̒��S�_����L�΂�
			Quaternion debrisRot;

			//�I��ł��锚�e����Ɉ�ԏ�ɂȂ�悤�ɂ���B
			debrisRot.SetRotationDeg(cameraDir, degPerOneDebris * (i - m_selectBombNo));

			//toDebris�{�̂ɉ�]��K�p����Ƒ��̏ꏊ�ɂ��e�����o��̂ŃR�s�[���Ă����]��K�p����B
			Vector3 tmp = toDebris;
			debrisRot.Apply(tmp);

			//��]�̒��S�_����L�΂�
			debris->m_position = centerOfRotation + tmp;

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
		m_BurstNum++;//�o�[�X�g���g������
		//���̓Q�[�W��300����B
		m_charge -= 300.0f;
		if (m_charge < 0)
		{
			m_charge = 0;
		}

		m_isBurst = true;
		m_burstCount = 60;

		//�o�[�X�g��
		prefab::CSoundSource* ssBurst = NewGO<prefab::CSoundSource>(0);;

		switch (m_magPower)
		{
		case -1://����
			//�o�[�X�g�����Đ�
			ssBurst->Init(L"Assets/sound/���̓o�[�X�g��.wav");
			ssBurst->SetVolume(1.5);
			ssBurst->Play(false);		
			//�G�t�F�N�g��\��
			m_burstEffect->Init(u"Assets/effect/���̓o�[�X�g.efk");
			m_burstEffect->Play();
			break;

		case 1://�˗�
			//�o�[�X�g�����Đ�
			ssBurst->Init(L"Assets/sound/�˗̓o�[�X�g��.wav");
			ssBurst->SetVolume(1.5);
			ssBurst->Play(false);
			//�G�t�F�N�g��\��
			m_burstEffect->Init(u"Assets/effect/�˗̓o�[�X�g.efk");
			m_burstEffect->Play();
			break;
		}
	}
}

//���̓o�[�X�g
void Player::MagneticBurst()
{
	//�o�[�X�g�G�t�F�N�g�̈ʒu��ݒ�
	m_burstEffect->SetPosition(m_position);

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

			//�G�̒e���܂��D���Ă��Ȃ���
			if (m_isSteal == false)
			{

				//�G�̕ێ�����e��1���ȏ゠��Ȃ�
				if (m_enemy->m_holdDebrisVector.size() != 0)
				{
					int i = 0;
					//�����Ă�K���L���h���b�v������
					//�S���ł͂Ȃ�3�܂łɂ��Ă݂�B

					//�G�̎����Ă���K���L�̃��X�g�𑖍�
					for (auto iterator = m_enemy->m_holdDebrisVector.begin(); iterator != m_enemy->m_holdDebrisVector.end(); iterator++)
					{
						m_StealNum++;//�G�̒e��D������
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

					//�e�L�X�g�X�V
					m_enemy->m_bulletNumFont->SetText(std::to_wstring(m_enemy->m_holdDebrisVector.size()));
					if (m_enemy->m_playerNum == 0)
					{
						if (m_enemy->m_holdDebrisVector.size() >= 10)
							m_enemy->m_bulletNumFont->SetPosition({ -207.0f, -270.0f });

						else
							m_enemy->m_bulletNumFont->SetPosition({ -170.0f, -270.0f });
					}
					else
					{
						if (m_enemy->m_holdDebrisVector.size() >= 10)
							m_enemy->m_bulletNumFont->SetPosition({ 23.0f, -270.0f });
						else
							m_enemy->m_bulletNumFont->SetPosition({ 60.0f, -270.0f });
					}

					//�����G�̒e��D�����̂Ńt���O�ύX
					m_isSteal = true;
				}

				//�G��1�ł����e�������Ă����
				if (m_enemy->m_holdBombVector.size() != 0)
				{
					//���e���h���b�v������B
					(*m_enemy->m_holdBombVector.begin())->m_bombState = Bomb::enDrop;

					//�h���b�v���������e�𑊎�̃R���e�i����폜
					m_enemy->m_holdBombVector.erase(m_enemy->m_holdBombVector.begin());

					//�����G�̒e��D�����̂Ńt���O�ύX
					m_isSteal = true;
				}

				if (m_isSteal == true)
				{
					//���X�N�̂���s���𐬌��������̂ŕK�E�Z�Q�[�W���v���X�B
					ChargeSpecialAttackGauge(25);
				}
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
	m_charge -= 2.0f;
	if (m_charge < 0)
	{
		m_charge = 0;
	}

	//���̓Q�[�W��0�ȉ����A�o�[�X�g����K�E�Z�������łȂ����
	if (m_charge <= 0 && 
		m_isBurst == false &&
		m_specialShotFlag == false	
		)	{
		//���͂̏�Ԃ�-1��1�Ȃ̂ŁA-1���|����Δ��]����B
		//���ʂ�switch���Ă����������B
		m_magPower *= -1;

		if (m_isAttacking == true)
		{
			m_isAttacking = false;
			m_characterSpeed = 6.0f;
		}

		//�`���[�W���񕜁B
		m_charge = 1000.0f;
	
		//���̓G�t�F�N�g�ύX
		if (m_magPower == 1) {
			m_magEffect[0]->Init(u"Assets/effect/�˗�.efk");
			m_magEffect[1]->Init(u"Assets/effect/�˗�.efk");
		}
		else if (m_magPower == -1) {
			m_magEffect[0]->Init(u"Assets/effect/����.efk");
			m_magEffect[1]->Init(u"Assets/effect/����.efk");
		}
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

		//�R���g���[���[�̓��͂ŃJ�����̌�����������Ƃ��点��
		qRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * 5.0f);
		qRotY.Apply(m_toCameraDir);

		Quaternion qRotX;
		Vector3 right = g_camera3D[m_playerNum]->GetRight();
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -5.0f);

		qRotX.Apply(m_toCameraDir);
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
		if (t > 0.9999f || t < -0.9999f)
		{

		}
		else
		{
			qRotX.Apply(m_toCameraDir);
		}
	}

	//�{���̃J�����ʒu
	cameraPos = targetPos + m_toCameraDir * 125.0f;

	//�o�l�J�����I�Ȃ��́@�J�����̃^�[�Q�b�g�ʒu����{���̃J�����ʒu�ւ̃x�N�g�����X�e�[�W�ɏՓ˂��Ă��邩����
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(targetPos, cameraPos, crossPoint);

	if (isHit == false)
	{
		//�������Ă��Ȃ��Ȃ�{���̃J�����ʒu
		g_camera3D[m_playerNum]->SetPosition(cameraPos);
	}
	else
	{
		//�������Ă���Ȃ瓖�������ʒu�ɃJ�������ړ�
		g_camera3D[m_playerNum]->SetPosition(crossPoint);
	}
	g_camera3D[m_playerNum]->SetTarget(targetPos);

	//���C�g
	Vector3 PlayerWaistPos = m_position;
	PlayerWaistPos.y += 40;
	Vector3 Direction = PlayerWaistPos - cameraPos;
	Direction.Normalize();
	m_spotLight->SetDirection(Direction);
	m_spotLight->SetPosition(cameraPos);
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
	m_ReceivedDamage += damage;//�󂯂��_���[�W
	m_hp -= damage;
	m_HitOn = true;//�A�j���[�V�����t���O
	m_Hitcount = 30;//
	ChargeSpecialAttackGauge(10);
	m_enemy->ChargeSpecialAttackGauge(5);

	if (m_hp <= 0)
	{
		m_hp = 0;

		Lose();

		m_enemy->Win();

		m_gameScene->SetGameState(SampleScene::GameState::enResult);
	}

	//�^�����_���[�W�ʂ𑊎�ɕ\������
	DamageDisplay* damagedisplay = NewGO<DamageDisplay>(0, "damagedisplay");
	damagedisplay->m_damagePos = m_position;
	damagedisplay->m_enemyNum = m_enemy->m_playerNum;
	damagedisplay->m_damage = damage;

	//HP�o�[�X�V
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPlaying)
	{
		if (m_playerNum == 0) {
			m_HPBarDarkSpriteRender->SetPosition({ -9.0f + m_hp / 1000.0f * 299, 325.0f,0.0f });
		}
		else if (m_playerNum == 1) {
			m_HPBarDarkSpriteRender->SetPosition({ 9.0f + m_hp / 1000.0f * -299, 325.0f,0.0f });
		}
	}

	//�_���[�W�G�t�F�N�g
	m_hitEffect->SetPosition({ m_position.x, m_position.y + 50, m_position.z });

	//�J�����̑O����
	m_damegeEffectFront.y = 0.0f;
	m_damegeEffectFront.Normalize();

	float innerProduct = m_damegeEffectFront.Dot(Vector3::AxisZ); //����
	float angle = acosf(innerProduct);//�A�[�N�R�T�C��
	/*if (m_damegeEffectFront.x < 0) {
		angle *= -1;
	}*/
	Quaternion rot;
	rot.SetRotation(Vector3::AxisY, angle);

	m_hitEffect->SetRotation(rot);
	m_hitEffect->Play();
	//�����܂ŃG�t�F�N�g����
}

//�K�E�Z�Q�[�W���`���[�W����B
void Player::ChargeSpecialAttackGauge(int charge)
{
	m_specialAttackGauge += charge;
	
	if (m_specialAttackGauge >= 100)
	{
		m_specialAttackGauge = 100;

		if (m_oldSpecialAttackGauge < 100)
		{
			//�G�t�F�N�g
			m_SPGaugeMaxEffect->SetPosition({ m_position.x,m_position.y + 50.0f, m_position.z });
			m_SPGaugeMaxEffect->Play();
			//SE
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/����[��.wav");
			ss->SetVolume(0.5f);
			ss->Play(false);

			m_ChargeSPFontRender->SetText(L"MAX");
			if (m_playerNum == 0)
			{
				m_ChargeSPFontRender->SetPosition({ -585.0f, -215.0f });
			}
			else
			{
				m_ChargeSPFontRender->SetPosition({ 465.0f, -215.0f });
			}
			m_ChargeSPFontRender->SetScale({1.0f,1.0f});
		}
	}
	else
	{
		m_SaveSP += charge;//���܂����K�E�Z�|�C���g

		m_ChargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");

		if (m_playerNum == 0)
		{
			if (m_specialAttackGauge < 10)
				m_ChargeSPFontRender->SetPosition({ -553.0f, -225.0f });
			else
				m_ChargeSPFontRender->SetPosition({ -565.0f, -225.0f });
		}
		else
		{
			if (m_specialAttackGauge < 10)
				m_ChargeSPFontRender->SetPosition({ 498.0f, -225.0f });
			else											 
				m_ChargeSPFontRender->SetPosition({ 485.0f, -225.0f });
		}
	}

	m_oldSpecialAttackGauge = m_specialAttackGauge;
}

//����������
void Player::Win()
{	
	/*m_resultSpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_resultSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);
	m_resultSpriteRender->Init("Assets/Image/Syouri.dds", 256, 256);*/
	//m_winnerNum = m_playerNum;
	Vector3 crossPoint;
	bool hitflag = false;

	Vector3 linestart = m_position;
	Vector3 lineend = m_position;
	linestart.y += 50.0f;
	lineend.y -= 1000.0f;

	hitflag = m_stageModel->isLineHitModel(linestart, lineend, crossPoint);

	if (hitflag == true)
	{
		m_position = crossPoint;
	}
	else
	{
		m_position = m_stageModel->GetRespawnPoint(m_enemy->m_position);
	}

	m_skinModelRender->SetPosition(m_position);

	m_LastFront = m_front;
}

//�s�k������
void Player::Lose()
{	
	m_Lose = true;
	/*m_resultSpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_resultSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);
	m_resultSpriteRender->Init("Assets/Image/Haiboku.dds", 256, 256);*/
	//m_loserNum = m_playerNum;


	//�󒆂œ|�ꂽ���p�ɁA���݈ʒu���牺�����Ɍ������ă��C���΂��B
	Vector3 crossPoint;
	bool hitflag = false;

	Vector3 linestart = m_position;
	Vector3 lineend = m_position;
	linestart.y += 50.0f;
	lineend.y -= 1000.0f;

	hitflag = m_stageModel->isLineHitModel(linestart, lineend, crossPoint);

	if (hitflag == true)
	{
		//���C���q�b�g���Ă���Ȃ�q�b�g�����ʒu�Ɉړ��B
		m_position = crossPoint;
	}
	else
	{
		//�q�b�g���Ȃ��Ƃ������Ƃ͌��̏�Ȃ̂Ń��X�|�[���ʒu�Ƀv���C���[�̈ʒu���ړ��B
		m_position = m_stageModel->GetRespawnPoint(m_enemy->m_position);
	}

	m_skinModelRender->SetPosition(m_position);

	m_LastFront = m_front;
}



//�U����Ԃɐ؂�ւ��ł�����؂�ւ���B
void Player::TryChangeStatusAttack()
{
	if (m_magPower == 1 && m_holdDebrisVector.empty() == false && g_pad[m_playerNum]->IsPress(enButtonRB1)) {
		m_animStatus = enStatus_Attack;		
	}
}

//����U����Ԃɐ؂�ւ���
void Player::TryChangeStatusSpecialAttack()
{
	if (m_SpecialAttackOn==true)
	{
		m_animStatus = enStatus_SpecialAttack;	
		m_SpecialAttackOn = false;	
	}
}

//�o�[�X�g��Ԃɐ؂�ւ���
void Player::TryChangeStatusBurst()
{
	if (m_isBurst == true)
	{
		m_animStatus = enStatus_Burst;
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
		m_HitOn = false;//��e��ԃ��Z�b�g
	}
}

//�ҋ@��Ԃɐ؂�ւ��ł�����؂�ւ���B
void Player::TryChangeStatusIdle()
{
	if (m_moveSpeed.LengthSq() <= 0.001f) {
		m_animStatus = enStatus_Idle;
	}
}

//��e��Ԃɐ؂�ւ���
void Player::TryChangeStatusHit()
{
	if (m_HitOn == true )
	{		
		m_animStatus = enStatus_Hit;
	}	
}

//���S��Ԃɐ؂�ւ���
void Player::TryChangeStatusDeath()
{
	if (m_Lose==true)
	{
		m_animStatus = enStatus_Death;				
	}
}

//������Ԃɐ؂�ւ���
void Player::TryChangeStatusWin()
{
	if(m_WinAnimOn==true)
	m_animStatus = enStatus_Winner;
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
		TryChangeStatusHit();		
		TryChangeStatusDeath();
		TryChangeStatusWin();
		break;
	case enStatus_SpecialAttack:
		TryChangeStatusFall();
		TryChangeStatusSpecialAttack();
		if (m_skinModelRender->IsPlayingAnimation() == false)
		{
			m_animStatus = enStatus_Idle;			
		}
		TryChangeStatusHit();
		TryChangeStatusDeath();
		TryChangeStatusWin();
		break;
	case enStatus_Burst:
		TryChangeStatusIdle();
		TryChangeStatusBurst();
		break;
	case enStatus_Run:
		TryChangeStatusAttack();
		TryChangeStatusSpecialAttack();
		TryChangeStatusWalk();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		TryChangeStatusHit();
		TryChangeStatusDeath();
		TryChangeStatusWin();
		break;
	case enStatus_Walk:
		TryChangeStatusAttack();
		TryChangeStatusSpecialAttack();
		TryChangeStatusRun();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		TryChangeStatusHit();
		TryChangeStatusDeath();
		TryChangeStatusWin();
		break;
	case enStatus_Idle:
		TryChangeStatusAttack();
		TryChangeStatusSpecialAttack();
		TryChangeStatusRun();
		TryChangeStatusWalk();
		TryChangeStatusFall();
		TryChangeStatusHit();
		TryChangeStatusBurst();
		TryChangeStatusDeath();
		TryChangeStatusWin();
		break;
	case enStatus_Fall:
		TryChangeStatusAttack();
		TryChangeStatusSpecialAttack();
		TryChangeStatusRun();
		TryChangeStatusWalk();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		TryChangeStatusDeath();
		TryChangeStatusWin();
		break;

	case enStatus_Hit:
		TryChangeStatusFall();
		TryChangeStatusAttack();
		TryChangeStatusSpecialAttack();
		if (m_skinModelRender->IsPlayingAnimation() == false)
		{
			m_animStatus = enStatus_Idle;
			m_HitOn = false;
		}
		TryChangeStatusHit();
		TryChangeStatusDeath();
		TryChangeStatusWin();
		break;
	case enStatus_Death:
		TryChangeStatusDeath();
		TryChangeStatusWin();
		break;
	case enStatus_Winner:
		TryChangeStatusWin();
	}
}

void Player::AnimationSelect()
{	
	if (m_LoseCameraFlag == false)
		m_skinModelRender->m_animation_speed = 0.1f;
	else
		m_skinModelRender->m_animation_speed = 1.0;
	switch (m_animStatus)
	{
	case enStatus_Attack:
		m_skinModelRender->m_animation_speed = 4.0;
		m_skinModelRender->PlayAnimation(enAnimationClip_Attack);
		break;
	case enStatus_SpecialAttack:		
		m_skinModelRender->m_animation_speed = 2.0;
		m_skinModelRender->PlayAnimation(enAnimationClip_SpecialAttack);
		break;
	case enStatus_Burst:
		m_skinModelRender->m_animation_speed = 4.0;
		m_skinModelRender->PlayAnimation(enAnimationClip_Burst);
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
	case enStatus_Hit:
		m_skinModelRender->PlayAnimation(enAnimationClip_Hit);
		break;
	case enStatus_Death:		
		m_skinModelRender->PlayAnimation(enAnimationClip_Death);
		break;
	case enStatus_Winner:
		m_skinModelRender->PlayAnimation(enAnimationClip_Winner);
		break;
	}

}

//�J�����������Ă�������ɓG�̃��f���������̓X�e�[�W�̃��f�������邩���v�Z����B
bool Player::GetShootPoint(Vector3& crossPoint)
{
	//���ː�̌���

	//�J�����̈ʒu��������Ă�������ɔ�΂����C���쐬�B
	//�L�����N�^�[�̈ʒu���炶��Ȃ����Ƃɒ��ӁB
	//���C�̌���
	Vector3 testRayDir = g_camera3D[m_playerNum]->GetForward();
	//���C�̎n�_�̓v���C���[�̓���(�J������Target�͓���ɂ���)
	Vector3 testRayStart = g_camera3D[m_playerNum]->GetTarget();
	//���C�̎n�_�ƌ������狁�߂����C�̏I�_(10000�ȏ�̋����_�����Ƃ͂Ȃ��Ǝv���̂ŋ�����10000�ɐݒ�)
	Vector3 testRayEnd = testRayStart + testRayDir * 10000.0f;

	//�����������t���O�B
	bool hitFlag = false;

	//�܂��G�L�����N�^�[�t�߂̔|�������蔻�����������B
	for (auto tricollider : m_enemy->m_triCollider)
	{
		hitFlag = tricollider.isHit(testRayStart, testRayEnd, crossPoint);
		if (hitFlag == true)
		{
			//1��ł��q�b�g���Ă�����return
			return hitFlag;
		}
	}

	//�G�L�����N�^�[�t�߂Ƀq�b�g���Ȃ�������X�e�[�W�̃��f������������B
	if (hitFlag == false)
	{
		hitFlag = m_stageModel->isLineHitModel(testRayStart, testRayEnd, crossPoint);
	}

	return hitFlag;
}
void Player::KnockBack() {
	//�m�b�N�o�b�N���������ݒ�
	if (m_isknockBackCount == 0) {
		m_moveSpeed.y = 0.0f;
	}
	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f);
	m_skinModelRender->SetPosition(m_position);

	m_isknockBackCount++;

	if (m_isknockBackCount >= 7) {
		m_moveSpeed = { 0.0f,0.0f,0.0f };
		m_isknockBackCount = 0;
		m_isKnockBack = false;
	}	
}
void Player::OpeningCamera()
{
	m_cameraLoopCount++;	

	if (g_pad[m_playerNum]->IsTrigger(enButtonA))
	{
		m_gameScene->SetGameState(SampleScene::GameState::enStartCountDown);
	}
	
	if (m_cameraLoopCount < 250)
	{
		Vector3 toPos = m_position;
		toPos.y = 400;
		//toPos���񂷁B
		Quaternion qRotY;
		//Y�����ɂ�����Ƃ�����]����N�H�[�^�j�I��������B
		m_addY += 0.01;
		qRotY.SetRotation(Vector3::AxisY, m_addY);
		//�N�H�[�^�j�I�����g����toPos���񂷁B
		qRotY.Apply(toPos);
		m_cameraPos = toPos;
		g_camera3D[m_playerNum]->SetTarget(m_targetPos);
	}
	else
	{
		Vector3 PlayerPos = m_position;
		PlayerPos.y = m_position.y + 90.0f;//�v���C���[�̓��̈ʒu
		
		
		Vector3 targetVec = PlayerPos - m_cameraPos;
		if (targetVec.Length() < 250)
		{
			m_gameScene->SetGameState(SampleScene::GameState::enStartCountDown);
		}
		targetVec.Normalize();
		m_cameraPos += targetVec*gain;
		gain += 0.1;
		g_camera3D[m_playerNum]->SetTarget(PlayerPos);
	}		
	g_camera3D[m_playerNum]->SetPosition(m_cameraPos);


	//�L���������������Ɍ�������

	//�L�����̑O������m_characterDirection;

	n = m_characterDirection.Dot(Vector3::AxisZ);//����
	float angle = acosf(n);//�A�[�N�R�T�C��
	if (m_characterDirection.x < 0) {
		angle *= -1;
	}
	m_rot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(m_rot);
}

void Player::FinalHit()//�����������Ƃ��̃J����
{	
	m_gameScene->ssBGM->Stop();
	if (m_FirstTime == true) {//��񂾂�����郋�[�v
		//��ʕ������I��
		GameObjectManager::GetInstance()->Set2ScreenMode(false);
		//��ʂɏo�Ă���������
		//HP�o�[�A��ʕ������A���r�E�X�Q�[�W������
		DeleteGO(m_bulletNumFont);
		m_bulletNumFont = nullptr;
		DeleteGO(m_bulletNumFont2);
		m_bulletNumFont2 = nullptr;
		DeleteGO(m_crosshairRender);
		m_crosshairRender = nullptr;
		DeleteGO(m_HPBarSpriteRender);
		m_HPBarSpriteRender = nullptr;
		DeleteGO(m_HPBarDarkSpriteRender);
		m_HPBarDarkSpriteRender = nullptr;
		DeleteGO(m_mobiusGauge);
		DeleteGO(m_ChargeSPFontRender);
		m_ChargeSPFontRender = nullptr;
		m_mobiusGauge = nullptr;
		//�e������
		QueryGOs<Bomb>("bomb", [](Bomb* bomb)->bool
			{
				DeleteGO(bomb);
				return true;
			});
		QueryGOs<Debris>("debris", [](Debris* debris)->bool
			{
				DeleteGO(debris);
				return true;
			});
		m_FirstTime = false;
	}
	Vector3 LastRight;
	Vector3 winnerFrontPos;
	Vector3 winnerHeadPos;
	if (m_playerNum == m_loserNum)//�s�҂��ʂ�
	{	
		LastRight = Cross(g_vec3AxisY, m_LastFront);//�Ō�Ɍ����Ă��������̉E�x�N�g��
		Vector3 targetPos = m_position;//�^�[�Q�b�g�|�W�V����
		m_cameraPos = targetPos;//�J�����̃|�W�V����
		targetPos.y += 20;//������ɐݒ肷��
		targetPos += m_LastFront * -30;//�L�����̏������ɐL�΂�	
		m_cameraPos.y += 100;//�L������菭���キ�炢
		//���[�v�̒l�ɍ��킹�ăX�e�[�^�X��ς���
		if (m_LoseCameraLoop > 0)
		{
			m_LoseCameraFlag = false;//�A�j���[�V�����̍Đ����x��ς���
			m_LastCameraStatus = 0;
		}
		if(m_LoseCameraLoop > 50)
		{ 
			m_LastCameraStatus = 1;
		}
		if (m_LoseCameraLoop > 100)
		{
			m_LastCameraStatus = 2;
		}
		if (m_LoseCameraLoop > 150)
		{
			m_LastCameraStatus = 3;
			m_LoseCameraFlag = true;//�A�j���[�V�����̍Đ����x��߂�
		}
		if (m_LoseCameraLoop > 200)
		{
			m_LastCameraStatus = 4;
		}		
		
		switch (m_LastCameraStatus)
		{
		case 0://�E����̃J����
			m_cameraPos += LastRight * 200;//�E
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 1://������̃J����
			m_cameraPos += LastRight * -200;//��
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 2://�O����̃J����
			m_cameraPos += m_LastFront * 200;//����
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 3://�������ʂ��Ȃ���G���������J����
			winnerHeadPos = m_enemy->m_position;
			winnerHeadPos.y += 50;//���҂̓��̈ʒu
			//�G�̂�����ƑO�Ǝ��������񂾐��𐳋K�����Č��ɏ����L�΂�
			m_winnerVec=(winnerHeadPos + m_enemy->m_LastFront * 200) - m_position;
			m_winnerVec.Normalize();
			m_cameraPos += m_winnerVec*-200;//���
			m_enemyWaistPos = m_enemy->m_position;//�G�̍��̈ʒu
			m_enemyWaistPos.y += 20;
			g_camera3D[0]->SetTarget(m_enemyWaistPos);
			break;
		case 4://�G�̑O�܂ňړ�����
			//�J������G�̑O�܂ňړ�������				
			winnerFrontPos = (m_enemy->m_position + m_enemy->m_LastFront * 150 ) - (m_position + m_winnerVec * -200);			
			if (m_coef < 1.0f)//�x�N�g���Ɋ|����l
				m_coef += 0.01f;
			if (m_coef > 0.5f)//�J�����������̋����܂ōs���ƃA�j���[�V�������Đ�����
				m_enemy->m_WinAnimOn = true;
			m_cameraPos += (winnerFrontPos * (pow(m_coef,1.5)) );//+ (winnerVec * 200)
			m_cameraPos += m_winnerVec * -200;//���
			
			g_camera3D[0]->SetTarget(m_enemyWaistPos);
			break;
		default:
			break;
		}

		//SE
		if (m_LoseCameraLoop == 0)
		{
			//�����Đ�
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/K.O..wav");
			ss->SetVolume(1.5f);
			ss->Play(false);
		}
		if (m_LoseCameraLoop == 50)
		{
			//�����Đ�
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/K.O..wav");
			ss->SetVolume(1.5f);
			ss->Play(false);
		}
		if (m_LoseCameraLoop == 100)
		{
			//�����Đ�
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/K.O..wav");
			ss->SetVolume(1.5f);
			ss->Play(false);
		}
		if (m_LoseCameraLoop == 250)
		{
			//�W���O�����Đ�
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/yattaze!1.wav");
			ss->SetVolume(1.5f);
			ss->Play(false);
		}
		if (m_LoseCameraLoop == 300)
		{
			m_winnerFont = NewGO<prefab::CFontRender>(0);
			m_winnerFont->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
			m_winnerFont->SetPosition({ -300.0f, -120.0f });
			m_winnerFont->SetScale({ 2.0f, 2.0f });
			m_winnerFont->SetColor({ 0.0f,0.9f,1.0f,1.0f});
			m_winnerFont->SetText(L"YOU WIN!");
			m_winnerFont->SetShadowFlag(true);
			m_winnerFont->SetShadowColor({ 0,0,0,1 });
			m_winnerFont->SetShadowOffset(3);
		}

		g_camera3D[0]->SetPosition(m_cameraPos);
		m_LoseCameraLoop++;
	}
}
void Player::ResultDisplay()
{
	if (m_resultFirstTime == true)
	{
		m_resultFirstTime = false;
		if (m_playerNum == 0)
		{
			//���������v�Z(0�Ŋ���ƃo�O�邽�߁{�P�����Ă���)
			m_HitRate = ((m_enemy->m_TakeAttackNum + 1) / (m_AttackNum + 1)) * 100;

			//�̗́A�`���[�W�A���݂̎����̎��͂̏�Ԃ̕\��

		}
		if (m_playerNum == 1)
		{
			//���������v�Z(0�Ŋ���ƃo�O�邽�߁{�P�����Ă���)
			m_HitRate = ((m_enemy->m_TakeAttackNum + 1) / (m_AttackNum + 1)) * 100;

		}
	}
}
