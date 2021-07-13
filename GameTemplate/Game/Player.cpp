#include "stdafx.h"
#include "Player.h"

#include "BackGround.h"
#include "Debris.h"
#include "Bomb.h"
#include "DamageDisplay.h"
#include "GravityBullet.h"
#include "GameScene.h"
#include "MobiusGauge.h"
#include "ResultScene.h"

namespace
{
	const float PLAYER_COLLIDER_RADIUS = 20.0f;
	const float PLAYER_COLLIDER_HEIGHT = 100.0f;
	const Vector4 FONT_SHADOWCOLOR = { 0.0f,0.0f,0.0f,1.0f };
	const int FONT_SHADOWOFFSET = 3;
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER1_ONESPLACE = { -170.0f, -270.0f };
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER1_TENSPLACE = { -207.0f, -270.0f };
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER2_ONESPLACE = { 60.0f, -270.0f };
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER2_TENSPLACE = { 23.0f, -270.0f };
	const Vector2 FONT_BULLETMAX_POSITION_PLAYER1 = { -130.0f, -283.0f };
	const Vector2 FONT_BULLETMAX_POSITION_PLAYER2 = { 100.0f, -283.0f };
	const Vector2 FONT_BULLETMAX_SCALE = { 0.7f,0.7f };
	const Vector2 FONT_SPGAUGE_POSITION_PLAYER1 = { -553.0f, -225.0f };
	const Vector2 FONT_SPGAUGE_POSITION_PLAYER2 = { 498.0f, -225.0f };
	const Vector2 FONT_SPGAUGE_SCALE = { 0.7f,0.7f };
	const Vector4 FONT_SPGAUGE_COLOR = { 1.0f,1.0f, 0.0f,1.0f };

	const int SPRITE_SIGHT_WIDTH = 32;
	const int SPRITE_SIGHT_HEIGHT = 32;

	const int SPRITE_HPBAR_WIDTH = 308;
	const int SPRITE_HPBAR_HEIGHT = 32;
	const Vector3 SPRITE_HPBAR_POSITION_PLAYER1 = { -10.0f,325.0f,0.0f };
	const Vector3 SPRITE_HPBAR_POSITION_PLAYER2 = { 10.0f,325.0f,0.0f };
	const int SPRITE_DAMAGEBAR_WIDTH = 316;
	const int SPRITE_DAMAGEBAR_HEIGHT = 36;
	const Vector3 SPRITE_DAMAGEBAR_POSITION_PLAYER1 = { 290.0f,325.0f,0.0f };
	const Vector3 SPRITE_DAMAGEBAR_POSITION_PLAYER2 = { -290.0f,325.0f,0.0f };
	const int SPRITE_DARKBAR_WIDTH = 316;
	const int SPRITE_DARKBAR_HEIGHT = 36;
	const Vector3 SPRITE_DARKHPBAR_POSITION_PLAYER1 = { 290.0f,325.0f,0.0f };
	const Vector3 SPRITE_DARKHPBAR_POSITION_PLAYER2 = { -290.0f,325.0f,0.0f };
	const Vector3 SPRITE_BAR_SCALE_PLAYER2 = { -1.0f, 1.0f, -1.0f };
	const Vector3 MOBIUSGAUGE_POSITION_PLAYER1 = { -525.0f,-280.0f,0.0f };
	const Vector3 MOBIUSGAUGE_POSITION_PLAYER2 = { 525.0f,-280.0f,0.0f };

	const Vector3 LIGHT_PLAYER_SPOTLIGHT_COLOR = { 10.0f, 10.0f, 10.0f };
	const float LIGHT_PLAYER_SPOTLIGHT_RANGE = 250.0f;
	const float LIGHT_PLAYER_SPOTLIGHT_ANGLE = 30.0f;

	const Vector3 EFFECT_MAGNET_SCALE = { 25.0f,25.0f,25.0f };
	const Vector3 EFFECT_BURST_SCALE = { 50.0f, 50.0f, 50.0f };
	const Vector3 EFFECT_HIT_SCALE = { 10.0f, 10.0f, 10.0f };
	const Vector3 EFFECT_SPFIRE_SCALE = { 20.0f, 20.0f, 20.0f };
	const Vector3 EFFECT_SPGAUGEMAX_SCALE = { 8.0f, 8.0f, 8.0f };
	const Vector3 EFFECT_SPCHARGE_SCALE = { 70.0f, 70.0f, 70.0f };
	const int EFFECT_MAGEFFECTCOUNT_CALL1 = 40;
	const int EFFECT_MAGEFFECTCOUNT_CALL2 = 0;
	const int EFFECT_MAGEFFECTCOUNT_AFTERCALL2 = 80;

	const int PLAYER_FALLLOOP_MAX = 75;
	const int PLAYER_FALLLOOP_ZERO = 0;

	const float PLAYER_SPEED_RUN = 6.0f;
	const float PLAYER_SPEED_WALK = 0.5f;
	const float PLAYER_SPEED_STOP = 0.0f;

	const float PLAYER_TO_MAGPOSITION = 50.0f;

	const float HEIGHT_PLAYER_FALL = -750.0f;
	const float HEIGHT_PLAYER_FALLSOUND_PLAY = -50.0f;
	const float SOUND_SE_FALL_VOLUME = 0.5f;
	const int DAMAGE_FALL = 100;

	const int FOOTSTEPTIMER_WALK = 25;
	const int FOOTSTEPTIMER_RUN = 17;
	const int FOOTSTEPTIMER_RESET = 0;
	const float SOUND_SE_FOOTSTEP_VOLUME = 0.2f;

	const int PLAYER_ATTACKCOUNT_CANATTACK = 0;
	const int PLAYER_ATTACKCOUNT_AFTERSHOOT = 10;
	const int PLAYER_ATTACKCOUNT_AFTERBOMB = 60;

	const float SOUND_SE_DEBRISSHOOT_VOLUME = 0.5f;

	const int SIZE_TENSPLACE = 10;

	const int PLAYER_SPGAUGE_MAX = 100;
	const int PLAYER_SPGAUGE_ZERO = 0;
	const int PLAYER_SPECIALSHOTCOUNT_ZERO = 0;
	const int PLAYER_SPECIALSHOTCOUNT_SHOOT = 35;
	const float SOUND_SE_SHOOTGRAVITY_VOLUME = 0.8f;

	const float BULLETHOLDER_TO_CENTER_OF_BULLET_ROTATION = 100.0f;
	const float BULLETHOLDER_TO_BEHIND_PLAYER = 30.0f;

	const float DEBRIS_ROTATEDEG_PER_FRAME = 0.5f;
	const Vector3 CENTER_TO_FIRSTDEBRIS = { 0.0f,50.0f,0.0f };

	const int DEBRIS_NEXTSHOOT_INDEX = 0;
	const float DEBRIS_NEXTSHOOT_DISTANCE = 100.0f;

	const int BOMB_FIRST_OBTAIN_INDEX = 0;
	const Vector3 CENTER_TO_FIRSTBOMB = { 0.0f,30.0f,0.0f };

	const float PLAYER_BURST_COST = 300.0f;
	const float PLAYER_MAGNETCHARGE_ZERO = 0.0f;
	const float PLAYER_MAGNETCHARGE_MAX = 1000.0f;
	const int PLAYER_BURSTCOUNT_START = 60;
	const int PLAYER_BURSTCOUNT_END = 0;
	const float SOUND_SE_BURST_VOLUME = 1.5f;
	const float BURST_RANGE_MIN = 100.0f;
	const float BURST_RANGE_MAX = 750.0f;

	const int SPGAUGE_BULLETSTEAL_POINT = 40;
	
	const float PLAYER_MAGNETCHARGE_DECAY_VALUE = 2.0f;
	
	const float PLAYER_POSITION_TO_CAMERATARGET = 90.0f;
	const float PLAYER_CAMERATARGET_TO_CAMERAPOSITION = 125.0f;

	const float PLAYER_AIMABLE_WIDTH = 250.0f;
	const float PLAYER_AIMABLE_HEIGHT = 250.0f;
}
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
	if (m_bulletMaxFont != nullptr)
	{
		DeleteGO(m_bulletMaxFont);
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
	if (m_DamageBarSpriteRender != nullptr)
	{
		DeleteGO(m_DamageBarSpriteRender);
	}

	if (m_mobiusGauge != nullptr)
	{
		DeleteGO(m_mobiusGauge);
	}

	m_magEffect[0]->Stop();
	m_magEffect[1]->Stop();
	m_burstEffect->Stop();
	m_hitEffect->Stop();
	m_SPFireEffect->Stop();
	m_SPGaugeMaxEffect->Stop();
	DeleteGO(m_magEffect[0]);
	DeleteGO(m_magEffect[1]);
	DeleteGO(m_burstEffect);
	DeleteGO(m_hitEffect);
	DeleteGO(m_SPFireEffect);
	DeleteGO(m_SPGaugeMaxEffect);
	DeleteGO(m_SPChargeEffectRed);
	DeleteGO(m_SPChargeEffectBlue);

	if (m_spotLight != nullptr)
	{
		DeleteGO(m_spotLight);
	}

	if (m_chargeSPFontRender != nullptr)
	{
		DeleteGO(m_chargeSPFontRender);
	}

	DeleteGO(m_resultWinnerSprite);
	DeleteGO(m_resultWinSprite);
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
	if (m_playerNum == NUMBER_PLAYER1)//�v���C���[�̐F��ς���
	{
		m_skinModelRender->Init("Assets/modelData/Player1.tkm", "Assets/modelData/Mage.tks", animationClips, enAnimationClip_num);
	}
	else if (m_playerNum == NUMBER_PLAYER2)
	{
		m_skinModelRender->Init("Assets/modelData/Player2.tkm", "Assets/modelData/Mage.tks", animationClips, enAnimationClip_num);
	}
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetPosition(m_position);

	//�L�����R���̏�����
	m_charaCon.Init(PLAYER_COLLIDER_RADIUS, PLAYER_COLLIDER_HEIGHT, m_position);

	//�c�e���\���̏�����
	m_bulletNumFont = NewGO<prefab::CFontRender>(6);
	m_bulletNumFont->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_bulletNumFont->SetShadowFlag(true);
	m_bulletNumFont->SetShadowColor(FONT_SHADOWCOLOR);
	m_bulletNumFont->SetShadowOffset(FONT_SHADOWOFFSET);
	if (m_playerNum == NUMBER_PLAYER1)
	{
		m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_ONESPLACE);
	}
	else
	{
		m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_ONESPLACE);
	}
	m_bulletNumFont->SetText(std::to_wstring(m_holdDebrisVector.size()));

	m_bulletMaxFont = NewGO<prefab::CFontRender>(6);
	m_bulletMaxFont->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_bulletMaxFont->SetShadowFlag(true);
	m_bulletMaxFont->SetShadowColor(FONT_SHADOWCOLOR);
	m_bulletMaxFont->SetShadowOffset(FONT_SHADOWOFFSET);
	if (m_playerNum == NUMBER_PLAYER1)
	{
		m_bulletMaxFont->SetPosition(FONT_BULLETMAX_POSITION_PLAYER1);
	}
	else
	{
		m_bulletMaxFont->SetPosition(FONT_BULLETMAX_POSITION_PLAYER2);
	}
	m_bulletMaxFont->SetScale(FONT_BULLETMAX_SCALE);
	m_bulletMaxFont->SetText(L"/10");

	//�K�E�Q�[�W�̗��܂���\������t�H���g
	m_chargeSPFontRender = NewGO<prefab::CFontRender>(6);
	m_chargeSPFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)2);
	m_chargeSPFontRender->SetShadowFlag(true);
	m_chargeSPFontRender->SetShadowColor(FONT_SHADOWCOLOR);
	m_chargeSPFontRender->SetShadowOffset(FONT_SHADOWOFFSET);
	if (m_playerNum == NUMBER_PLAYER1)
	{
		m_chargeSPFontRender->SetPosition(FONT_SPGAUGE_POSITION_PLAYER1);
	}
	else
	{
		m_chargeSPFontRender->SetPosition(FONT_SPGAUGE_POSITION_PLAYER2);
	}
	m_chargeSPFontRender->SetScale(FONT_SPGAUGE_SCALE);
	m_chargeSPFontRender->SetColor(FONT_SPGAUGE_COLOR);
	m_chargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");

	//�Ə��\���̏�����
	m_crosshairRender = NewGO<prefab::CSpriteRender>(1);
	m_crosshairRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));
	m_crosshairRender->Init("Assets/Image/Sight.dds", SPRITE_SIGHT_WIDTH, SPRITE_SIGHT_HEIGHT);

	//�X�e�[�W�̃N���X���擾
	m_stageModel = FindGO<BackGround>("background");

	//HP�o�[
	m_HPBarSpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_HPBarSpriteRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));

	m_HPBarSpriteRender->Init("Assets/Image/HP_Bar.dds", SPRITE_HPBAR_WIDTH, SPRITE_HPBAR_HEIGHT);
	if (m_playerNum == NUMBER_PLAYER1) {
		m_HPBarSpriteRender->SetPosition(SPRITE_HPBAR_POSITION_PLAYER1);
	}
	else if (m_playerNum == NUMBER_PLAYER2) {
		m_HPBarSpriteRender->SetScale(SPRITE_BAR_SCALE_PLAYER2);
		m_HPBarSpriteRender->SetPosition(SPRITE_HPBAR_POSITION_PLAYER2);
	}
	//���b�h
	m_DamageBarSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_DamageBarSpriteRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));

	m_DamageBarSpriteRender->Init("Assets/Image/HP_Bar_Damage.dds", SPRITE_DAMAGEBAR_WIDTH, SPRITE_DAMAGEBAR_HEIGHT);
	if (m_playerNum == NUMBER_PLAYER1) {
		m_DamageBarSpriteRender->SetPosition(SPRITE_DAMAGEBAR_POSITION_PLAYER1);
		//({ 290.0f,325.0f,0.0f });
	}
	else if (m_playerNum == NUMBER_PLAYER2) {
		m_DamageBarSpriteRender->SetScale(SPRITE_BAR_SCALE_PLAYER2);
		m_DamageBarSpriteRender->SetPosition(SPRITE_DAMAGEBAR_POSITION_PLAYER2);
	}
	//
	m_HPBarDarkSpriteRender = NewGO<prefab::CSpriteRender>(5);
	m_HPBarDarkSpriteRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));

	m_HPBarDarkSpriteRender->Init("Assets/Image/HP_Bar_Dark_12.dds", SPRITE_DARKBAR_WIDTH, SPRITE_DARKBAR_HEIGHT);
	if (m_playerNum == NUMBER_PLAYER1) {
		m_HPBarDarkSpriteRender->SetPosition(SPRITE_DARKHPBAR_POSITION_PLAYER1);
	}
	else if (m_playerNum == NUMBER_PLAYER2) {
		m_HPBarDarkSpriteRender->SetScale(SPRITE_BAR_SCALE_PLAYER2);
		m_HPBarDarkSpriteRender->SetPosition(SPRITE_DARKHPBAR_POSITION_PLAYER2);
	}

	m_mobiusGauge = NewGO<MobiusGauge>(0);
	if (m_playerNum == NUMBER_PLAYER1) {
		m_mobiusGauge->SetPosition(MOBIUSGAUGE_POSITION_PLAYER1);
	}
	else if (m_playerNum == NUMBER_PLAYER2) {
		m_mobiusGauge->SetPosition(MOBIUSGAUGE_POSITION_PLAYER2);
	}

	//�v���C���[�̃��C�g
	m_spotLight = NewGO<prefab::CSpotLight>(0);
	m_spotLight->SetColor(LIGHT_PLAYER_SPOTLIGHT_COLOR);
	m_spotLight->SetRange(LIGHT_PLAYER_SPOTLIGHT_RANGE);
	m_spotLight->SetAngleDeg(LIGHT_PLAYER_SPOTLIGHT_ANGLE);
	
	
	//�G�t�F�N�g�֘A
	m_magEffect[0] = NewGO<prefab::CEffect>(0);
	m_magEffect[1] = NewGO<prefab::CEffect>(0);
	m_magEffect[0]->SetScale(EFFECT_MAGNET_SCALE);
	m_magEffect[1]->SetScale(EFFECT_MAGNET_SCALE);
	if (m_magnetState == MAGNETSTATE_REPULSION) {
		m_magEffect[0]->Init(u"Assets/effect/�˗�.efk");
		m_magEffect[1]->Init(u"Assets/effect/�˗�.efk");
	}
	else if (m_magnetState == MAGNETSTATE_GRAVITY) {
		m_magEffect[0]->Init(u"Assets/effect/����.efk");
		m_magEffect[1]->Init(u"Assets/effect/����.efk");
	}

	m_burstEffect = NewGO<prefab::CEffect>(0);
	m_burstEffect->SetScale(EFFECT_BURST_SCALE);

	m_hitEffect = NewGO<prefab::CEffect>(0);
	m_hitEffect->Init(u"Assets/effect/�_���[�W.efk");
	m_hitEffect->SetScale(EFFECT_HIT_SCALE);

	m_SPFireEffect = NewGO<prefab::CEffect>(0);
	m_SPFireEffect->SetScale(EFFECT_SPFIRE_SCALE);

	m_SPGaugeMaxEffect = NewGO<prefab::CEffect>(0);
	m_SPGaugeMaxEffect->Init(u"Assets/effect/�L���s�[��.efk");
	m_SPGaugeMaxEffect->SetScale(EFFECT_SPGAUGEMAX_SCALE);

	m_SPChargeEffectRed = NewGO<prefab::CEffect>(0);
	m_SPChargeEffectRed->SetScale(EFFECT_SPCHARGE_SCALE);
	m_SPChargeEffectRed->Init(u"Assets/effect/�˗̓`���[�W.efk");

	m_SPChargeEffectBlue = NewGO<prefab::CEffect>(0);
	m_SPChargeEffectBlue->SetScale(EFFECT_SPCHARGE_SCALE);
	m_SPChargeEffectBlue->Init(u"Assets/effect/���̓`���[�W.efk");
	//�����܂ŃG�t�F�N�g

	m_gameScene = FindGO<GameScene>("gamescene");
	return true;
}

void Player::Update()
{
	//�|�[�Y���Ȃ�X�L�b�v�B
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause)
	{
		return;
	}

	//�I�[�v�j���O�J�����B
	if (m_gameScene->GetGameState() == GameScene::GameState::enBirdseye)
	{
		OpeningCamera();
	}
	//�X�^�[�g�_�E���J�E���g�_�E����
	else if (m_gameScene->GetGameState() == GameScene::GameState::enStartCountDown)
	{
		//�J�����̈ړ��B
		Camera();

		//�L������O�Ɍ�������

		//�J�����̑O����
		m_cameraFront = m_position - g_camera3D[m_playerNum]->GetPosition();
		m_cameraFront.y = 0.0f;
		m_cameraFront.Normalize();

		//�J�����̉E����
		m_cameraRight = Cross(g_vec3AxisY, m_cameraFront);

		float dot = m_cameraFront.Dot(Vector3::AxisZ);//����
		float angle = acosf(dot);//�A�[�N�R�T�C��
		if (m_cameraFront.x < 0) {
			angle *= -1;
		}
		m_rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(m_rot);

		//��ԍX�V�B
		UpdateState();
		//�A�j���[�V�����I���B
		AnimationSelect();
	}
	else if(m_gameScene->GetGameState() == GameScene::GameState::enPlaying)
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
		}

		//�U����̌��̃^�C�}�[�����炵�Ă���
		m_attackCount--;
		//�U���̃N�[���^�C�����I���ƈړ����x��߂�
		if (m_attackCount <= PLAYER_ATTACKCOUNT_CANATTACK && m_isBurst == false)
		{
			m_attackCount = PLAYER_ATTACKCOUNT_CANATTACK;
			m_isAttacking = false;
			m_characterSpeed = PLAYER_SPEED_RUN;
		}
		//��ԍX�V�B
		UpdateState();
		//�A�j���[�V�����I���B
		AnimationSelect();

		//�J�����̈ړ�
		Camera();

		//�˗́E���̓G�t�F�N�g			

		//���̓G�t�F�N�g���Đ�
		if (m_magEffectCallCount == EFFECT_MAGEFFECTCOUNT_CALL1) {
			m_magEffect[0]->Play();
		}
		else if (m_magEffectCallCount <= EFFECT_MAGEFFECTCOUNT_CALL2) {
			m_magEffect[1]->Play();
			m_magEffectCallCount = EFFECT_MAGEFFECTCOUNT_AFTERCALL2;
		}
		m_magEffectCallCount--;

		m_magEffect[0]->SetPosition(m_position);
		m_magEffect[1]->SetPosition(m_position);
			
		//�K�E�Z�Q�[�W�����܂����u�Ԃ̃G�t�F�N�g���Đ�����Ă����
		if (m_SPGaugeMaxEffect->IsPlay())
		{
			m_SPGaugeMaxEffect->SetPosition(m_magPosition);
		}
	
	}
	else if(m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		//�t�@�C�i���q�b�g�J�����B
		FinalHit();
		//��ԍX�V�B
		//UpdateState();
		//�A�j���[�V�����I���B
		//AnimationSelect();
	}
	else if (m_gameScene->GetGameState() == GameScene::GameState::enDraw)
	{
		//�ێ����Ă���K���L�𕂗V������B
		HoldDebris();

		//�ێ����Ă��锚�e�𕂗V������B
		HoldBomb();

		//�J�����̈ړ��B
		Camera();

		//�L������O�Ɍ�������

		//�J�����̑O����
		m_cameraFront = m_position - g_camera3D[m_playerNum]->GetPosition();
		m_cameraFront.y = 0.0f;
		m_cameraFront.Normalize();

		//�J�����̉E����
		m_cameraRight = Cross(g_vec3AxisY, m_cameraFront);

		float dot = m_cameraFront.Dot(Vector3::AxisZ);//����
		float angle = acosf(dot);//�A�[�N�R�T�C��
		if (m_cameraFront.x < 0) {
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
	
	//HP�o�[�X�V
	if (m_gameScene->GetGameState() == GameScene::GameState::enPlaying)
	{
		if (m_playerNum == NUMBER_PLAYER1) {
			m_DamageBarSpriteRender->SetPosition({ -9.0f + m_hp / 1000.0f * 299, 325.0f,0.0f });
			if (m_HPBarDarkSpriteRender->GetPosition().x > m_DamageBarSpriteRender->GetPosition().x) {
				Vector3 DarkPos = m_HPBarDarkSpriteRender->GetPosition();
				DarkPos.x -= 2.0f;
				m_HPBarDarkSpriteRender->SetPosition(DarkPos);
				if (m_HPBarDarkSpriteRender->GetPosition().x < m_DamageBarSpriteRender->GetPosition().x) {	//�I�[�o�[�C��
					DarkPos.x = m_DamageBarSpriteRender->GetPosition().x;
					m_HPBarDarkSpriteRender->SetPosition(DarkPos);
				}
			}
		}
		else if (m_playerNum == NUMBER_PLAYER2) {
			m_DamageBarSpriteRender->SetPosition({ 9.0f + m_hp / 1000.0f * -299, 325.0f,0.0f });
			if (m_HPBarDarkSpriteRender->GetPosition().x < m_DamageBarSpriteRender->GetPosition().x) {
				Vector3 DarkPos = m_HPBarDarkSpriteRender->GetPosition();
				DarkPos.x += 2.0f;
				m_HPBarDarkSpriteRender->SetPosition(DarkPos);
				if (m_HPBarDarkSpriteRender->GetPosition().x > m_DamageBarSpriteRender->GetPosition().x) {	//�I�[�o�[�C��
					DarkPos.x = m_DamageBarSpriteRender->GetPosition().x;
					m_HPBarDarkSpriteRender->SetPosition(DarkPos);
				}
			}
		}
	}

	//���r�E�X�Q�[�W�̐F�����͂��猈��
	if (m_magnetState == MAGNETSTATE_REPULSION)
	{
		m_mobiusGauge->m_isRed = true;
	}
	else
	{
		m_mobiusGauge->m_isRed = false;
	}

	//���r�E�X�Q�[�W�Ɍ��݂̕K�E�Z�̃`���[�W�ʂ�n��
	m_mobiusGauge->m_magnetCharge = m_magnetCharge;

	m_mobiusGauge->m_spCharge = m_specialAttackGauge;
}

//�ړ�
void Player::Move()
{
	Vector3 oldPos = m_position;

	//�J�����̑O����
	m_cameraFront = m_position - g_camera3D[m_playerNum]->GetPosition();
	m_cameraFront.y = 0.0f;
	m_cameraFront.Normalize();

	//�J�����̉E����
	m_cameraRight = Cross(g_vec3AxisY, m_cameraFront);

	float dot = m_cameraFront.Dot(Vector3::AxisZ);//����
	float angle = acosf(dot);//�A�[�N�R�T�C��
	if (m_cameraFront.x < 0) {
		angle *= -1;
	}
	m_rot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(m_rot);
	m_moveAmount = m_cameraFront * g_pad[m_playerNum]->GetLStickYF() * m_characterSpeed + m_cameraRight * g_pad[m_playerNum]->GetLStickXF() * m_characterSpeed;
	if (m_charaCon.IsOnGround() == false)
	{
		if (m_fallLoop < PLAYER_FALLLOOP_MAX)
		{
			m_fallLoop++;
		}
		if (m_moveAmount.y > -0.001)
		{
			m_moveAmount.y -= 0.005f * (m_fallLoop * m_fallLoop);
		}
	}
	else
	{
		m_fallLoop = 0;
	}

	m_position = m_charaCon.Execute(m_moveAmount, 1.0f);
	m_magPosition = m_position;
	m_magPosition.y += PLAYER_TO_MAGPOSITION;
	m_skinModelRender->SetPosition(m_position);

	//���ɗ��������̏���
	if (m_position.y <= HEIGHT_PLAYER_FALL) {
		Damage(DAMAGE_FALL);

		//�G����ł��������X�|�[���n�_�Ɉړ�����B
		Vector3 respawnPoint = m_stageModel->GetRespawnPoint(m_enemy->m_position);
		m_position = respawnPoint;
		m_charaCon.SetPosition(m_position);
		m_skinModelRender->SetPosition(m_position);

		//���X�|�[�������̂ŗ��������p�̃J�E���g�����Z�b�g�B
		m_fallLoop = PLAYER_FALLLOOP_ZERO;
	}

	//����
	if (g_pad[m_playerNum]->GetLStickXF() || g_pad[m_playerNum]->GetLStickYF())
	{
		m_footstepsTimer++;
		
		if (m_animStatus == enStatus_Walk) {
			if (m_footstepsTimer >= FOOTSTEPTIMER_WALK)
			{
				SoundOneShotPlay(L"Assets/sound/����.wav", SOUND_SE_FOOTSTEP_VOLUME);

				m_footstepsTimer = FOOTSTEPTIMER_RESET;
			}
		}
		if (m_animStatus == enStatus_Run) {
			if (m_footstepsTimer >= FOOTSTEPTIMER_RUN)
			{
				SoundOneShotPlay(L"Assets/sound/����.wav", SOUND_SE_FOOTSTEP_VOLUME);
				
				m_footstepsTimer = FOOTSTEPTIMER_RESET;
			}
		}
	}
	else
		m_footstepsTimer = FOOTSTEPTIMER_RESET;

	if (oldPos.y >= HEIGHT_PLAYER_FALLSOUND_PLAY && m_position.y < HEIGHT_PLAYER_FALLSOUND_PLAY)
	{
		SoundOneShotPlay(L"Assets/sound/������.wav", SOUND_SE_FALL_VOLUME);
	}	
}

//�U��
void Player::Attack()
{
	if (g_pad[m_playerNum]->IsPress(enButtonRB1) && m_attackCount == PLAYER_ATTACKCOUNT_CANATTACK)
	{
		//�K���L����ł������Ă���Ȃ�
		if (m_holdDebrisVector.empty() == false)
		{
			//����炷
			SoundOneShotPlay(L"Assets/sound/�V���[�g��.wav", SOUND_SE_DEBRISSHOOT_VOLUME);

			//��ԍŏ��ɕێ������K���L�𔭎�
			auto debris = m_holdDebrisVector.front();
			//�ێ������K���L�𔭎˃��[�h�ɂ���
			debris->SetDebrisState(Debris::enBullet);

			//�L�����N�^�[�̃X�s�[�h��x������B
			m_characterSpeed = PLAYER_SPEED_WALK;

			//�U�����̃t���O���I��
			m_isAttacking = true;

			//�U���̌��̎�������
			m_attackCount = PLAYER_ATTACKCOUNT_AFTERSHOOT;
			
			//���̏ꏊ�Ɍ������Č���(GetShootPoint�̒��ł̎Q�Ǝ󂯎��p)
			Vector3 crossPoint;
			//���ː���v�Z�B
			bool hitFlag = GetShootPoint(crossPoint);

			//���ː�ɓG�������̓X�e�[�W�̃��f��������B
			if (hitFlag)
			{
				//�Ə��̎w�������ɔ�΂�
				debris->SetMoveDirection(crossPoint - debris->GetPosition());
			}
			else //�Ȃ��B
			{
				Vector3 moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				moveDirection.y = 0.0f;
				debris->SetMoveDirection(moveDirection);
			}

			//���˂����K���L��ێ����X�g����폜
			m_holdDebrisVector.erase(m_holdDebrisVector.begin());

			//�e�L�X�g�X�V
			m_bulletNumFont->SetText(std::to_wstring(m_holdDebrisVector.size()));
			if (m_playerNum == NUMBER_PLAYER1)
			{
				if (m_holdDebrisVector.size() >= SIZE_TENSPLACE)
					m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_TENSPLACE);

				else 
					m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_ONESPLACE);
			}
			else
			{
				if (m_holdDebrisVector.size() >= SIZE_TENSPLACE)
					m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_TENSPLACE);
				else
					m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_ONESPLACE);
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
	if (m_specialAttackGauge >= PLAYER_SPGAUGE_MAX && g_pad[m_playerNum]->IsTrigger(enButtonLB3))
	{
		m_SpecialAttackOn = true;		//�A�j���[�V������K�E�Z�ɂ���B
		m_specialShotFlag = true;
	}
	if (m_specialShotFlag == true)
	{
		//���˂���O�ɁA�`���[�W����悤�ȃG�t�F�N�g
		switch (m_magnetState)
		{
		case MAGNETSTATE_GRAVITY:
			m_SPChargeEffectBlue->SetPosition(m_magPosition);
			if (m_specialShotCount == PLAYER_SPECIALSHOTCOUNT_ZERO)
			{
				m_SPChargeEffectBlue->Play();
			}
			break;
		case MAGNETSTATE_REPULSION:
			m_SPChargeEffectRed->SetPosition(m_magPosition);
			if (m_specialShotCount == PLAYER_SPECIALSHOTCOUNT_ZERO)
			{
				m_SPChargeEffectRed->Play();
			}
			break;
		}
		if (m_specialShotCount == PLAYER_SPECIALSHOTCOUNT_ZERO)
		{
			SoundOneShotPlay(L"Assets/sound/�p���[�`���[�W.wav");
		}

		m_specialShotCount++;

		//���ˑO�Ƀ_���[�W���󂯂���L�����Z��
		//if (m_HitOn == true) 
		//{
		//	//�Q�[�W��0�ɁB
		//	m_specialAttackGauge = PLAYER_SPGAUGE_ZERO;
		//	m_specialShotFlag = false;
		//	m_specialShotCount = PLAYER_SPECIALSHOTCOUNT_ZERO;
		//	if (m_playerNum == NUMBER_PLAYER1)
		//	{
		//		m_chargeSPFontRender->SetPosition(FONT_SPGAUGE_POSITION_PLAYER1);
		//		m_chargeSPFontRender->SetScale(FONT_SPGAUGE_SCALE);
		//		m_chargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
		//	}
		//	else
		//	{
		//		m_chargeSPFontRender->SetPosition(FONT_SPGAUGE_POSITION_PLAYER2);
		//		m_chargeSPFontRender->SetScale(FONT_SPGAUGE_SCALE);
		//		m_chargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
		//	}
		//}
	}

	if (m_specialShotCount >= PLAYER_SPECIALSHOTCOUNT_SHOOT)
	{
		//���͂Ȃ�
		if (m_magnetState == MAGNETSTATE_GRAVITY)
		{
			//����炷
			SoundOneShotPlay(L"Assets/sound/���͒e����.wav", SOUND_SE_SHOOTGRAVITY_VOLUME);

			//���˃G�t�F�N�g
			m_SPFireEffect->Init(u"Assets/effect/���͒e����.efk");
			m_SPFireEffect->SetPosition({
				m_position.x + m_cameraFront.x * 50.0f,
				m_position.y + 50.0f,
				m_position.z + m_cameraFront.z * 50.0f
				});
			m_SPFireEffect->Play();

			GravityBullet* gravityBullet = NewGO<GravityBullet>(0, "gravitybullet");
			gravityBullet->SetPosition(m_magPosition);
			gravityBullet->SetParent(this);

			//���̏ꏊ�Ɍ������Č���(GetShootPoint�̒��ł̎Q�Ǝ󂯎��p)
			Vector3 crossPoint;
			//���ː���v�Z�B
			bool hitFlag = GetShootPoint(crossPoint);

			if (hitFlag)
			{
				//�Ə��̎w�������ɔ�΂�
				Vector3 gravityMoveDirection = crossPoint - gravityBullet->GetPosition();
				gravityMoveDirection.Normalize();
				gravityBullet->SetMoveDirection(gravityMoveDirection);
			}
			else
			{
				Vector3 gravityMoveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				gravityMoveDirection.y = 0.0f;
				gravityMoveDirection.Normalize();
				gravityBullet->SetMoveDirection(gravityMoveDirection);
			}

			//�������̂ŕK�E�Z�Q�[�W��0��
			m_specialAttackGauge = PLAYER_SPGAUGE_ZERO;
			m_specialShotFlag = false;
			m_specialShotCount = PLAYER_SPECIALSHOTCOUNT_ZERO;
			if (m_playerNum == NUMBER_PLAYER1)
			{
				m_chargeSPFontRender->SetPosition(FONT_SPGAUGE_POSITION_PLAYER1);
				m_chargeSPFontRender->SetScale(FONT_SPGAUGE_SCALE);
				m_chargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
			}
			else
			{
				m_chargeSPFontRender->SetPosition(FONT_SPGAUGE_POSITION_PLAYER2);
				m_chargeSPFontRender->SetScale(FONT_SPGAUGE_SCALE);
				m_chargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
			}
		}
		else//�˗͂Ȃ�
		{
			//�e��1���ł������Ă�?
			if (m_holdDebrisVector.size() != 0)
			{
				//����炷
				SoundOneShotPlay(L"Assets/sound/�C�e1.wav");

				//���˃G�t�F�N�g
				m_SPFireEffect->Init(u"Assets/effect/�˗͒e����.efk");
				m_SPFireEffect->SetPosition({ 
					m_position.x + m_cameraFront.x * 50.0f,
					m_position.y + 50.0f,
					m_position.z + m_cameraFront.z * 50.0f
					});
				m_SPFireEffect->Play();

				//���̏ꏊ�Ɍ������Č���(GetShootPoint�̒��ł̎Q�Ǝ󂯎��p)
				Vector3 crossPoint;
				//���ː���v�Z�B
				bool hitFlag = GetShootPoint(crossPoint);

				if (hitFlag)
				{
					//�Ə��̎w�������ɔ�΂�
					for (auto debris : m_holdDebrisVector)
					{
						debris->SetDebrisState(Debris::enBullet);
						debris->SetMoveDirection(crossPoint - debris->GetPosition());
					}
				}
				else
				{
					for (auto debris : m_holdDebrisVector)
					{
						debris->SetDebrisState(Debris::enBullet);
						Vector3 moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
						moveDirection.y = 0.0f;
						debris->SetMoveDirection(moveDirection);
					}
				}

				m_holdDebrisVector.clear();

				//�e�L�X�g�X�V
				m_bulletNumFont->SetText(std::to_wstring(m_holdDebrisVector.size()));
				if (m_playerNum == NUMBER_PLAYER1)
				{
					m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_ONESPLACE);
				}
				else
				{
					m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_ONESPLACE);
				}


				//�������̂ŕK�E�Z�Q�[�W��0��
				m_specialAttackGauge = PLAYER_SPGAUGE_ZERO;
				m_specialShotFlag = false;
				m_specialShotCount = PLAYER_SPECIALSHOTCOUNT_ZERO;
				if (m_playerNum == NUMBER_PLAYER1)
				{
					m_chargeSPFontRender->SetPosition(FONT_SPGAUGE_POSITION_PLAYER1);
					m_chargeSPFontRender->SetScale(FONT_SPGAUGE_SCALE);
					m_chargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
				}
				else
				{
					m_chargeSPFontRender->SetPosition(FONT_SPGAUGE_POSITION_PLAYER2);
					m_chargeSPFontRender->SetScale(FONT_SPGAUGE_SCALE);
					m_chargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");
				}
			}
			else
			{
				//1�����e�����ĂȂ�����s���ɂ���B�G���[��?�Q�[�W������Ȃ��Ă���
				m_specialShotFlag = false;
				m_specialShotCount = PLAYER_SPECIALSHOTCOUNT_ZERO;
			}
		}
	}
}

void Player::ThrowBomb()
{
	if (g_pad[m_playerNum]->IsPress(enButtonRB2) && m_attackCount == PLAYER_ATTACKCOUNT_CANATTACK)
	{
		//���e����ł������Ă���Ȃ�
		if (m_holdBombVector.empty() == false)
		{
			//����炷
			SoundOneShotPlay(L"Assets/sound/�����鉹.wav");

			//�I�����Ă��锚�e�𔭎�
			auto debris = m_holdBombVector.begin() + m_selectBombNo;
			//�ێ��������e�𔭎˃��[�h�ɂ���
			(*debris)->SetBombState(Bomb::enBullet);

			//�����n�߂̈ʒu�𒲐�
			Vector3 throwPosition = (*debris)->GetPosition();
			throwPosition.y -= 30.0f;
			(*debris)->SetPosition(throwPosition);

			//�L�����N�^�[�̃X�s�[�h��x������B
			m_characterSpeed = PLAYER_SPEED_WALK;

			//�U�����̃t���O���I��
			m_isAttacking = true;

			//�U���̌��̎�������
			m_attackCount = PLAYER_ATTACKCOUNT_AFTERBOMB;

			Vector3 front = g_camera3D[m_playerNum]->GetForward();
			front.y += 0.5f;
			front.Normalize();
			(*debris)->SetMoveDirection(front);

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
	m_holdDebrisRotateDeg += DEBRIS_ROTATEDEG_PER_FRAME;
	if (m_holdDebrisRotateDeg >= 360.0f)
	{
		m_holdDebrisRotateDeg -= 360.0f;
	}

	//��]�̒��S��ݒ肷��B
	Vector3 centerOfRotation = m_position;
	centerOfRotation.y += BULLETHOLDER_TO_CENTER_OF_BULLET_ROTATION;

	//��]�̒��S���v���C���[�����ɁB
	Vector3 cameraDir = g_camera3D[m_playerNum]->GetForward();
	cameraDir.y = 0.0f;
	cameraDir.Normalize();
	centerOfRotation -= cameraDir * BULLETHOLDER_TO_BEHIND_PLAYER;

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
			if(i == DEBRIS_NEXTSHOOT_INDEX)
			{ 
				debris->SetPosition(m_magPosition + (cameraDir * DEBRIS_NEXTSHOOT_DISTANCE));
			}
			else
			{
				//Y������50�̃x�N�g������]�����ĉ�]�̒��S�_����L�΂�
				Quaternion debrisRot;

				//i�̒l�ɂ���ď��Ԃɕ��ׂ�
				debrisRot.SetRotationDeg(cameraDir, m_holdDebrisRotateDeg + degPerOneDebris * (i-1));

				//�R�s�[���Ă����]��K�p����B
				Vector3 toDebris = CENTER_TO_FIRSTDEBRIS;
				debrisRot.Apply(toDebris);

				//��]�̒��S�_����L�΂�
				debris->SetPosition(centerOfRotation + toDebris);
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
			m_selectBombNo = BOMB_FIRST_OBTAIN_INDEX;
		}
	}
	else if (g_pad[m_playerNum]->IsTrigger(enButtonLeft))
	{
		//�I�����Ă��锚�e�̔ԍ����}�C�i�X�ɂȂ��Ă�����
		if (--m_selectBombNo < BOMB_FIRST_OBTAIN_INDEX)
		{
			//�������ăR���e�i�̃T�C�Y-1�ɂȂ�
			m_selectBombNo = m_holdBombVector.size() - 1;
		}
	}

	//��]�̒��S��ݒ肷��B
	Vector3 centerOfRotation = m_position;
	centerOfRotation.y += BULLETHOLDER_TO_CENTER_OF_BULLET_ROTATION;

	//��]�̒��S���v���C���[�����ɁB
	Vector3 cameraDir = g_camera3D[m_playerNum]->GetForward();
	cameraDir.y = 0;
	cameraDir.Normalize();
	centerOfRotation -= cameraDir * BULLETHOLDER_TO_BEHIND_PLAYER;

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
			Vector3 toBomb = CENTER_TO_FIRSTBOMB;
			debrisRot.Apply(toBomb);

			//��]�̒��S�_����L�΂�
			debris->SetPosition(centerOfRotation + toBomb);

			i++;
		}
	}
}

//���̓o�[�X�g���g�p���Ă��Ȃ����̒ʏ�̓���
void Player::MagneticBehavior()
{
	//���͂̏�Ԃɂ���ē���������B
	switch (m_magnetState)
	{
	case MAGNETSTATE_GRAVITY://����

		break;
	case MAGNETSTATE_REPULSION: //�˗�

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
		m_magnetCharge -= PLAYER_BURST_COST;
		if (m_magnetCharge < PLAYER_MAGNETCHARGE_ZERO)
		{
			m_magnetCharge = PLAYER_MAGNETCHARGE_ZERO;
		}

		m_isBurst = true;
		m_burstCount = PLAYER_BURSTCOUNT_START;

		switch (m_magnetState)
		{
		case MAGNETSTATE_GRAVITY://����
			//�o�[�X�g�����Đ�
			SoundOneShotPlay(L"Assets/sound/���̓o�[�X�g��.wav", SOUND_SE_BURST_VOLUME);
			//�G�t�F�N�g��\��
			m_burstEffect->Init(u"Assets/effect/���̓o�[�X�g.efk");
			m_burstEffect->Play();
			break;

		case MAGNETSTATE_REPULSION://�˗�
			//�o�[�X�g�����Đ�
			SoundOneShotPlay(L"Assets/sound/�˗̓o�[�X�g��.wav", SOUND_SE_BURST_VOLUME);
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
	m_characterSpeed = PLAYER_SPEED_STOP;

	//�G�܂ł̃x�N�g��
	Vector3 toEnemy = m_enemy->m_position - m_position;

	//�G�ɗ^�����
	Vector3 force = toEnemy;
	//�c���͉e�����Ȃ�
	force.y = 0;
	force.Normalize();

	//���͂̏�Ԃɂ���ē���������B
	switch (m_magnetState)
	{
	case MAGNETSTATE_GRAVITY://����

		//��������̂Ń}�C�i�X��
		force *= -10.0f;
		//�߂����鎞�͈�������Ȃ�
		if (toEnemy.Length() > BURST_RANGE_MIN && toEnemy.Length() < BURST_RANGE_MAX)
		{
			m_enemy->ExecuteCharacon(force);

			//�G�̒e���܂��D���Ă��Ȃ���
			if (m_isSteal == false)
			{

				//�G�̕ێ�����e��1���ȏ゠��Ȃ�
				if (m_enemy->m_holdDebrisVector.size() != 0)
				{
					int enemyHoldDebrisIndex = 0;
					//�����Ă�K���L���h���b�v������
					//�S���ł͂Ȃ�3�܂łɂ��Ă݂�B

					//�G�̎����Ă���K���L�̃��X�g�𑖍�
					for (auto iterator = m_enemy->m_holdDebrisVector.begin(); iterator != m_enemy->m_holdDebrisVector.end(); iterator++)
					{
						//�h���b�v��Ԃɂ����Ă����B�����z���̂Ń|�b�v��Ԃł͂Ȃ��B
						(*iterator)->SetOnGroundFlag(false);
						(*iterator)->SetDebrisState(Debris::enDrop);

						//�J�E���g�������߂�
						enemyHoldDebrisIndex++;

						//3�ɂȂ�����3�z�����̂Ńu���C�N�B
						if (enemyHoldDebrisIndex == 3)
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
					if (m_enemy->m_playerNum == NUMBER_PLAYER1)
					{
						if (m_enemy->m_holdDebrisVector.size() >= SIZE_TENSPLACE)
							m_enemy->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_TENSPLACE);

						else
							m_enemy->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_ONESPLACE);
					}
					else
					{
						if (m_enemy->m_holdDebrisVector.size() >= SIZE_TENSPLACE)
							m_enemy->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_TENSPLACE);
						else
							m_enemy->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_ONESPLACE);
					}

					//�����G�̒e��D�����̂Ńt���O�ύX
					m_isSteal = true;
				}

				//�G��1�ł����e�������Ă����
				if (m_enemy->m_holdBombVector.size() != 0)
				{
					//���e���h���b�v������B
					(*m_enemy->m_holdBombVector.begin())->SetBombState(Bomb::enDrop);

					//�h���b�v���������e�𑊎�̃R���e�i����폜
					m_enemy->m_holdBombVector.erase(m_enemy->m_holdBombVector.begin());

					//�����G�̒e��D�����̂Ńt���O�ύX
					m_isSteal = true;
				}

				if (m_isSteal == true)
				{
					//���X�N�̂���s���𐬌��������̂ŕK�E�Z�Q�[�W���v���X�B
					ChargeSpecialAttackGauge(SPGAUGE_BULLETSTEAL_POINT);
				}
			}
		}

		break;
	case 1: //�˗�

		force *= 10.0f;
		if (toEnemy.Length() < BURST_RANGE_MAX)
		{
			m_enemy->ExecuteCharacon(force);
		}

		break;
	default:
		MessageBoxA(nullptr, "���͂̏�Ԃ��ُ�ł��B\n", "�G���[", MB_OK);
		break;
	}

	m_burstCount--;
	//�o�[�X�g�J�E���g��0�ɂȂ�ƃo�[�X�g�I���B
	if (m_burstCount <= PLAYER_BURSTCOUNT_END)
	{
		m_isBurst = false;
		//�G�̒e��D�����t���O�����Z�b�g�B
		m_isSteal = false;

		//�X�s�[�h���߂�
		m_characterSpeed = PLAYER_SPEED_RUN;
	}
}

void Player::ChangeMagnetPower()
{
	//���̓Q�[�W�̎��R����
	m_magnetCharge -= PLAYER_MAGNETCHARGE_DECAY_VALUE;
	if (m_magnetCharge < PLAYER_MAGNETCHARGE_ZERO)
	{
		m_magnetCharge = PLAYER_MAGNETCHARGE_ZERO;
	}

	//���̓Q�[�W��0�ȉ����A�o�[�X�g����K�E�Z�������łȂ����
	if (m_magnetCharge <= PLAYER_MAGNETCHARGE_ZERO &&
		m_isBurst == false &&
		m_specialShotFlag == false	
		)	{
		//���͂̏�Ԃ�-1��1�Ȃ̂ŁA-1���|����Δ��]����B
		//���ʂ�switch���Ă����������B
		m_magnetState *= -1;

		if (m_isAttacking == true)
		{
			m_isAttacking = false;
			m_characterSpeed = PLAYER_SPEED_RUN;
		}

		//�`���[�W���񕜁B
		m_magnetCharge = PLAYER_MAGNETCHARGE_MAX;
	
		//���̓G�t�F�N�g�ύX
		if (m_magnetState == MAGNETSTATE_GRAVITY) {
			m_magEffect[0]->Init(u"Assets/effect/����.efk");
			m_magEffect[1]->Init(u"Assets/effect/����.efk");
		}
		else if (m_magnetState == MAGNETSTATE_REPULSION) {
			m_magEffect[0]->Init(u"Assets/effect/�˗�.efk");
			m_magEffect[1]->Init(u"Assets/effect/�˗�.efk");
		}
	}
}

//�J�����̈ړ�
void Player::Camera()
{
	//�J�����֘A
	if (g_pad[m_playerNum]->IsTrigger(enButtonRB3))
	{
		m_isLockon = !m_isLockon;//���b�N�؂�ւ�
	}

	//�v���C���[�̓���90����Ƃ��āA��������J�����ɐL�т�x�N�g������]������B
	Vector3 targetPos = m_position;
	targetPos.y += PLAYER_POSITION_TO_CAMERATARGET;

	if (m_isLockon)
	{
		//���b�N���Ă��鎞�́A�G�̒��S����v���C���[�̓���ւ̃x�N�g���̉�������ɃJ����������B
		Vector3 toEnemy = m_enemy->m_magPosition - targetPos;
		toEnemy.Normalize();
		m_toCameraDir = toEnemy * -1.0f;

		//�R���g���[���[�̓��͂ŃJ�����̌�����������Ƃ��点��
		m_cameraQRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * 5.0f);
		m_cameraQRotY.Apply(m_toCameraDir);

		Quaternion qRotX;
		Vector3 right = g_camera3D[m_playerNum]->GetRight();
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -5.0f);

		qRotX.Apply(m_toCameraDir);
	}
	else
	{
		//���b�N����Ă��Ȃ����̓J�����ւ̃x�N�g������]������B
		m_cameraQRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * m_sensitivity);
		m_cameraQRotY.Apply(m_toCameraDir);

		Quaternion qRotX;
		Vector3 right = g_camera3D[m_playerNum]->GetRight();
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -m_sensitivity);

		//��]��̊p�x���m�F���ďシ���鎞�܂��͉������鎞�͉�]�����Ȃ��B
		Vector3 checkToCamera = m_toCameraDir;
		qRotX.Apply(checkToCamera);
		checkToCamera.Normalize();
		float t = checkToCamera.Dot(Vector3::Up);
		if (t < 0.999f && t > -0.999f)
		{
			qRotX.Apply(m_toCameraDir);
		}
	}

	//�{���̃J�����ʒu
	m_cameraPos = targetPos + m_toCameraDir * PLAYER_CAMERATARGET_TO_CAMERAPOSITION;

	//�o�l�J�����I�Ȃ��́@�J�����̃^�[�Q�b�g�ʒu����{���̃J�����ʒu�ւ̃x�N�g�����X�e�[�W�ɏՓ˂��Ă��邩����
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(targetPos, m_cameraPos, crossPoint);

	if (isHit == false)
	{
		//�������Ă��Ȃ��Ȃ�{���̃J�����ʒu
		g_camera3D[m_playerNum]->SetPosition(m_cameraPos);
	}
	else
	{
		//�������Ă���Ȃ瓖�������ʒu�ɃJ�������ړ�
		g_camera3D[m_playerNum]->SetPosition(crossPoint);
	}
	g_camera3D[m_playerNum]->SetTarget(targetPos);

	//���C�g
	Vector3 Direction = m_magPosition - m_cameraPos;
	Direction.Normalize();
	m_spotLight->SetDirection(Direction);
	m_spotLight->SetPosition(m_cameraPos);
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
	squarePos1.y += PLAYER_AIMABLE_HEIGHT;
	squarePos1 += side * PLAYER_AIMABLE_WIDTH;

	//�l�p�`�̍���
	Vector3 squarePos2 = m_position;
	squarePos2.y -= PLAYER_AIMABLE_HEIGHT;
	squarePos2 += side * PLAYER_AIMABLE_WIDTH;

	//�l�p�`�̉E��
	Vector3 squarePos3 = m_position;
	squarePos3.y += PLAYER_AIMABLE_HEIGHT;
	squarePos3 -= side * PLAYER_AIMABLE_WIDTH;

	//�l�p�`�̉E��
	Vector3 squarePos4 = m_position;
	squarePos4.y -= PLAYER_AIMABLE_HEIGHT;
	squarePos4 -= side * PLAYER_AIMABLE_WIDTH;

	//1�ڂ̎O�p�`�̒��_�͍���A�����A�E��B
	m_triCollider[0].SetVertex(squarePos1, squarePos2, squarePos3);
	//2�ڂ̎O�p�`�̒��_�͉E���A�E��A�����B
	m_triCollider[1].SetVertex(squarePos4, squarePos3, squarePos2);

	//�J�v�Z���̓����蔻�������B
	Vector3 legPos = m_position;
	Vector3 headPos = m_position;
	headPos.y += PLAYER_COLLIDER_HEIGHT;

	//�J�v�Z���̎n�_�ƏI�_���X�V����B(���a��Start�֐��ōŏ��������߂Ă��悢)
	m_collider.SetStartPoint(legPos);
	m_collider.SetEndPoint(headPos);
	m_collider.SetRadius(PLAYER_COLLIDER_RADIUS);
}

//�����̗̑͂Ƀ_���[�W��^����
void Player::Damage(int damage)
{	
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

		m_gameScene->SetGameState(GameScene::GameState::enResult);
	}

	//�^�����_���[�W�ʂ𑊎�ɕ\������
	DamageDisplay* damagedisplay = NewGO<DamageDisplay>(0, "damagedisplay");
	damagedisplay->SetDamagePosition(m_position);
	damagedisplay->SetEnemyNum(m_enemy->m_playerNum);
	damagedisplay->SetDamage(damage);

	//HP200�ȉ��ŐԂ��Ȃ�
	if (m_hp <= 200 && m_hpBarRedFlag == false) {
		m_HPBarSpriteRender->Init("Assets/Image/HP_Bar_Red.dds", 308, 32);
		m_hpBarRedFlag = true;
	}

	//�_���[�W�G�t�F�N�g
	m_hitEffect->SetPosition({ m_position.x, m_position.y + 50, m_position.z });

	//�J�����̑O����
	m_damageEffectFront.y = 0.0f;
	m_damageEffectFront.Normalize();

	float innerProduct = m_damageEffectFront.Dot(Vector3::AxisZ); //����
	float angle = acosf(innerProduct);//�A�[�N�R�T�C��
	Quaternion rot;
	rot.SetRotation(Vector3::AxisY, angle);

	m_hitEffect->SetRotation(rot);
	m_hitEffect->Play();
	//�����܂ŃG�t�F�N�g����
}

//�K�E�Z�Q�[�W���`���[�W����B
void Player::ChargeSpecialAttackGauge(int charge)
{
	if (m_gameScene->GetGameState() != GameScene::enPlaying)
	{
		return;
	}

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
			SoundOneShotPlay(L"Assets/sound/����[��.wav", 0.5f);

			m_chargeSPFontRender->SetText(L"MAX");
			if (m_playerNum == 0)
			{
				m_chargeSPFontRender->SetPosition({ -585.0f, -215.0f });
			}
			else
			{
				m_chargeSPFontRender->SetPosition({ 465.0f, -215.0f });
			}
			m_chargeSPFontRender->SetScale({1.0f,1.0f});
		}
	}
	else
	{
		m_chargeSPFontRender->SetText(std::to_wstring(m_specialAttackGauge) + L"%");

		if (m_playerNum == 0)
		{
			if (m_specialAttackGauge < 10)
				m_chargeSPFontRender->SetPosition({ -553.0f, -225.0f });
			else
				m_chargeSPFontRender->SetPosition({ -565.0f, -225.0f });
		}
		else
		{
			if (m_specialAttackGauge < 10)
				m_chargeSPFontRender->SetPosition({ 498.0f, -225.0f });
			else											 
				m_chargeSPFontRender->SetPosition({ 485.0f, -225.0f });
		}
	}

	m_oldSpecialAttackGauge = m_specialAttackGauge;
}

//����������
void Player::Win()
{	
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

	m_LastFrontDir = m_cameraFront;
}

//�s�k������
void Player::Lose()
{	
	m_Lose = true;	

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

	m_LastFrontDir = m_cameraFront;
}



//�U����Ԃɐ؂�ւ��ł�����؂�ւ���B
void Player::TryChangeStatusAttack()
{
	if (m_magnetState == 1 && m_holdDebrisVector.empty() == false && g_pad[m_playerNum]->IsPress(enButtonRB1)) {
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
	if (m_moveAmount.LengthSq() > 5.0f) {
		m_animStatus = enStatus_Run;
	}
}

//������Ԃɐ؂�ւ��ł�����؂�ւ���B
void Player::TryChangeStatusWalk()
{
	if (m_moveAmount.LengthSq() <= 5.0f && m_moveAmount.LengthSq() > 0.0f) {
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
	if (m_moveAmount.LengthSq() <= 0.001f) {
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
		break;
	case enStatus_SpecialAttack:
		TryChangeStatusFall();
		TryChangeStatusSpecialAttack();
		if (m_skinModelRender->IsPlayingAnimation() == false)
		{
			m_animStatus = enStatus_Idle;			
		}
		TryChangeStatusHit();		
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
		break;
	case enStatus_Walk:
		TryChangeStatusAttack();
		TryChangeStatusSpecialAttack();
		TryChangeStatusRun();
		TryChangeStatusIdle();
		TryChangeStatusFall();
		TryChangeStatusHit();
		break;
	case enStatus_Idle:
		TryChangeStatusAttack();
		TryChangeStatusSpecialAttack();
		TryChangeStatusRun();
		TryChangeStatusWalk();
		TryChangeStatusFall();
		TryChangeStatusHit();
		TryChangeStatusBurst();
		break;
	case enStatus_Fall:
		TryChangeStatusAttack();
		TryChangeStatusSpecialAttack();
		TryChangeStatusRun();
		TryChangeStatusWalk();
		TryChangeStatusIdle();
		TryChangeStatusFall();
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
		break;	
	}
}

void Player::AnimationSelect()
{	
	m_skinModelRender->SetAnimationSpeed(1.0f);
	switch (m_animStatus)
	{
	case enStatus_Attack:
		m_skinModelRender->SetAnimationSpeed(4.0f);
		m_skinModelRender->PlayAnimation(enAnimationClip_Attack);
		break;
	case enStatus_SpecialAttack:
		m_skinModelRender->SetAnimationSpeed(2.0f);
		m_skinModelRender->PlayAnimation(enAnimationClip_SpecialAttack);
		break;
	case enStatus_Burst:
		m_skinModelRender->SetAnimationSpeed(4.0f);
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
		m_moveAmount.y = 0.0f;
	}
	m_position = m_charaCon.Execute(m_moveAmount, 1.0f);
	m_skinModelRender->SetPosition(m_position);

	m_isknockBackCount++;

	if (m_isknockBackCount >= 7) {
		m_moveAmount = { 0.0f,0.0f,0.0f };
		m_isknockBackCount = 0;
		m_isKnockBack = false;
	}	
}
void Player::OpeningCamera()
{
	m_openingCameraCount++;

	if (g_pad[m_playerNum]->IsTrigger(enButtonA))//�I�[�v�j���O�J�����X�L�b�v
	{
		m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
	}
	
	if (m_openingCameraCount < 250)//250�t���[���o�Ă�
	{
		Vector3 toPos = m_position;
		toPos.y = 400;
		//toPos���񂷁B
		Quaternion qRotY;
		//Y�����ɉ�]����N�H�[�^�j�I��������
		m_openingCameraRotationRad += 0.01;
		qRotY.SetRotation(Vector3::AxisY, m_openingCameraRotationRad);
		//�N�H�[�^�j�I�����g����toPos����
		qRotY.Apply(toPos);
		m_sequenceCameraPos = toPos;
		g_camera3D[m_playerNum]->SetTarget(m_targetPos);
	}
	else//�L�����Ɍ������ăJ�������ړ�������
	{
		Vector3 PlayerPos = m_position;
		PlayerPos.y = m_position.y + 90.0f;//�v���C���[�̓��̈ʒu
		
		
		Vector3 targetVec = PlayerPos - m_sequenceCameraPos;
		if (targetVec.Length() < 250)//�J�������߂Â��΃I�[�v�j���O�J�����I��
		{
			m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
		}
		targetVec.Normalize();
		m_sequenceCameraPos += targetVec*m_openingCameraGainSpeed;
		m_openingCameraGainSpeed += 0.1;
		g_camera3D[m_playerNum]->SetTarget(PlayerPos);
	}
	g_camera3D[m_playerNum]->SetPosition(m_sequenceCameraPos);

	//�L���������������Ɍ�������

	//�L�����̑O������m_cameraFront;

	float dot = m_cameraFront.Dot(Vector3::AxisZ);//����
	float angle = acosf(dot);//�A�[�N�R�T�C��
	if (m_cameraFront.x < 0) {
		angle *= -1;
	}
	m_rot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(m_rot);
}

void Player::FinalHit()//�����������Ƃ��̃J����
{	
	m_gameScene->StopGameBGM();
	
	if (m_FirstTime == true) {//��񂾂�����郋�[�v
		//��ʕ������I��
		GameObjectManager::GetInstance()->Set2ScreenMode(false);
		//��ʂɏo�Ă���������
		//HP�o�[�A��ʕ������A���r�E�X�Q�[�W������
		DeleteGO(m_bulletNumFont);
		m_bulletNumFont = nullptr;
		DeleteGO(m_bulletMaxFont);
		m_bulletMaxFont = nullptr;
		DeleteGO(m_crosshairRender);
		m_crosshairRender = nullptr;
		DeleteGO(m_HPBarSpriteRender);
		m_HPBarSpriteRender = nullptr;
		DeleteGO(m_HPBarDarkSpriteRender);
		m_HPBarDarkSpriteRender = nullptr;
		DeleteGO(m_DamageBarSpriteRender);
		m_DamageBarSpriteRender = nullptr;
		DeleteGO(m_chargeSPFontRender);
		m_chargeSPFontRender = nullptr;
		DeleteGO(m_spotLight);
		m_spotLight = nullptr;
		DeleteGO(m_mobiusGauge);
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
	
	if (m_playerNum == m_loserNum)//�s�҂��ʂ�
	{		
		m_skinModelRender->PlayAnimation(enAnimationClip_Death);//�A�j���[�V�����Đ�
		
		Vector3 winnerFrontPos;//���҂̈ʒu���班���O�ɐL�т�x�N�g��		
		Vector3 winnerHeadPos = m_enemy->m_position;
		winnerHeadPos.y += 50;//���҂̓��̈ʒu
		Vector3 LastRight = Cross(g_vec3AxisY, m_LastFrontDir);//�Ō�Ɍ����Ă��������̉E�x�N�g��
		Vector3 targetPos = m_position;//�^�[�Q�b�g�|�W�V����
		m_sequenceCameraPos = targetPos;//�J�����̃|�W�V����
		targetPos.y += 20;//������ɐݒ肷��
		targetPos += m_LastFrontDir * -30;//�L�����̏������ɐL�΂�	
		m_sequenceCameraPos.y += 100;//�L������菭���キ�炢
		//���[�v�̒l�ɍ��킹�ăX�e�[�^�X��ς���
		if (m_LoseCameraLoop == 0)
		{
			m_LastCameraStatus = 0;
		}
		if(m_LoseCameraLoop == 50)
		{
			m_LastCameraStatus = 1;
		}
		if (m_LoseCameraLoop == 100)
		{
			m_LastCameraStatus = 2;
		}
		if (m_LoseCameraLoop == 150)
		{
			m_LastCameraStatus = 3;
		}
		if (m_LoseCameraLoop == 200)
		{
			m_LastCameraStatus = 4;
		}		
		
		switch (m_LastCameraStatus)
		{
		case 0://�E����̃J����
			m_skinModelRender->SetAnimationSpeed(0.1f);//�A�j���[�V������x������
			m_enemy->m_skinModelRender->SetAnimationSpeed(0.1f);
			m_sequenceCameraPos += LastRight * 200;//�E
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 1://������̃J����
			m_sequenceCameraPos += LastRight * -200;//��
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 2://�O����̃J����
			m_sequenceCameraPos += m_LastFrontDir * 200;//����
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 3://�������ʂ��Ȃ���G���������J����
			m_skinModelRender->SetAnimationSpeed(1.0f);//�A�j���[�V�����X�s�[�h�����Ƃɖ߂�
			m_enemy->m_skinModelRender->SetAnimationSpeed(1.0f);
			//�G�̂�����ƑO�Ǝ��������񂾐��𐳋K�����Č��ɏ����L�΂�
			m_winnerVec=(winnerHeadPos + m_enemy->m_LastFrontDir * 200) - m_position;
			m_winnerVec.Normalize();
			m_sequenceCameraPos += m_winnerVec*-200;//���
			m_winnerWaistPos = m_enemy->m_position;//�G�̍��̈ʒu
			m_winnerWaistPos.y += 20;
			g_camera3D[0]->SetTarget(m_winnerWaistPos);
			break;
		case 4://�G�̑O�܂ňړ�����
			//�J������G�̑O�܂ňړ�������				
			winnerFrontPos = (m_enemy->m_position + m_enemy->m_LastFrontDir * 150 ) - (m_position + m_winnerVec * -200);//	
			if (m_coef < 1.0f)//�x�N�g���Ɋ|����l
				m_coef += 0.01f;
			if (m_coef > 0.5f)//�J�����������̋����܂ōs���ƃA�j���[�V�������Đ�����
			{
				m_enemy->m_WinAnimOn = true;
			}
			m_sequenceCameraPos += (winnerFrontPos * (pow(m_coef,1.5)) );//�w���֐��I�ɃJ�������߂Â�

			m_sequenceCameraPos += m_winnerVec * -200;//case3�̂Ƃ��̃J�����̈ʒu�ƍ��킹�邽��
			
			g_camera3D[0]->SetTarget(m_winnerWaistPos);
			break;
		default:
			break;
		}

		//SE
		if (m_LoseCameraLoop == 0 || m_LoseCameraLoop == 50 || m_LoseCameraLoop == 100)
		{
			//�����Đ�
			SoundOneShotPlay(L"Assets/sound/K.O..wav", 1.5f);
		}
		else if (m_LoseCameraLoop == 250)
		{
			//�W���O�����Đ�
			SoundOneShotPlay(L"Assets/sound/yattaze!1.wav", 1.5f);
		}
		else if (m_LoseCameraLoop == 300)
		{
			if (m_loserNum == 1)	//1P�����������ꍇ
			{
				m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(5);
				m_resultWinnerSprite->Init("Assets/Image/1P.DDS", 148, 120);
				m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
				m_resultWinnerSprite->SetPosition({ -120.0f, -120.0f, 0.0f });
				m_resultWinnerSprite->SetScale({ 1.0f, 1.0f, 1.0f });	

				//�{�C�X�Đ�
				SoundOneShotPlay(L"Assets/sound/Player1Win.wav", 3.0f);
			}
			else if (m_loserNum == 0)	//2P�����������ꍇ
			{
				m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(5);
				m_resultWinnerSprite->Init("Assets/Image/2P.DDS", 180, 128);
				m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
				m_resultWinnerSprite->SetPosition({ -120.0f, -120.0f, 0.0f });
				m_resultWinnerSprite->SetScale({ 1.0f, 1.0f, 1.0f });	

				//�{�C�X�Đ�
				SoundOneShotPlay(L"Assets/sound/Player2Win.wav", 3.0f);
			}
		}
		else if (m_LoseCameraLoop == 350)
		{
			m_resultWinSprite = NewGO<prefab::CSpriteRender>(5);
			m_resultWinSprite->Init("Assets/Image/Win.DDS", 300, 128);
			m_resultWinSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
			m_resultWinSprite->SetPosition({ 150.0f, -120.0f, 0.0f });
			m_resultWinSprite->SetScale({ 1.0f, 1.0f, 1.0f });		
		}

		g_camera3D[0]->SetPosition(m_sequenceCameraPos);
		m_LoseCameraLoop++;
	}
	else//���҂̂Ƃ�
	{
		if (m_WinAnimOn==true)//�J�����������̋����܂ōs���ƃA�j���[�V�������Đ�����
		{			
			m_skinModelRender->PlayAnimation(enAnimationClip_Winner);
		}
		else
		{
			m_skinModelRender->PlayAnimation(enAnimationClip_Idle);
		}
	}
}
