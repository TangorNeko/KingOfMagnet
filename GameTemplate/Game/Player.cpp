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
	//アニメーションの初期化
	animationClips[enAnimationClip_Run].Load("Assets/animData/Mage_Run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);	//ループモーションにする。
	animationClips[enAnimationClip_Idle].Load("Assets/animData/Mage_Idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);	//ループモーションにする。
	animationClips[enAnimationClip_Attack].Load("Assets/animData/Mage_Attack.tka");
	animationClips[enAnimationClip_Attack].SetLoopFlag(false);	//ループモーションにする。
	animationClips[enAnimationClip_Walk].Load("Assets/animData/Mage_Walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);	//ループモーションにする。
	animationClips[enAnimationClip_Fall].Load("Assets/animData/Mage_Fall.tka");
	animationClips[enAnimationClip_Fall].SetLoopFlag(true);	//ループモーションにする。
	animationClips[enAnimationClip_SpecialAttack].Load("Assets/animData/Mage_RangeAttack.tka");
	animationClips[enAnimationClip_SpecialAttack].SetLoopFlag(false);
	animationClips[enAnimationClip_Burst].Load("Assets/animData/Mage_SpecialAttack2.tka");
	animationClips[enAnimationClip_Burst].SetLoopFlag(false);
	animationClips[enAnimationClip_Hit].Load("Assets/animData/Mage_Hit.tka");
	animationClips[enAnimationClip_Hit].SetLoopFlag(false);
	animationClips[enAnimationClip_Death].Load("Assets/animData/Mage_Death.tka");
	animationClips[enAnimationClip_Death].SetLoopFlag(false);
	animationClips[enAnimationClip_Winner].Load("Assets/animData/Mage_Winner.tka");
	animationClips[enAnimationClip_Winner].SetLoopFlag(false);	//ループモーションにする。
	
	//モデルの初期化
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	if (m_playerNum == NUMBER_PLAYER1)//プレイヤーの色を変える
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

	//キャラコンの初期化
	m_charaCon.Init(PLAYER_COLLIDER_RADIUS, PLAYER_COLLIDER_HEIGHT, m_position);

	//残弾数表示の初期化
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

	//必殺ゲージの溜まり具合を表示するフォント
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

	//照準表示の初期化
	m_crosshairRender = NewGO<prefab::CSpriteRender>(1);
	m_crosshairRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));
	m_crosshairRender->Init("Assets/Image/Sight.dds", SPRITE_SIGHT_WIDTH, SPRITE_SIGHT_HEIGHT);

	//ステージのクラスを取得
	m_stageModel = FindGO<BackGround>("background");

	//HPバー
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
	//レッド
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

	//プレイヤーのライト
	m_spotLight = NewGO<prefab::CSpotLight>(0);
	m_spotLight->SetColor(LIGHT_PLAYER_SPOTLIGHT_COLOR);
	m_spotLight->SetRange(LIGHT_PLAYER_SPOTLIGHT_RANGE);
	m_spotLight->SetAngleDeg(LIGHT_PLAYER_SPOTLIGHT_ANGLE);
	
	
	//エフェクト関連
	m_magEffect[0] = NewGO<prefab::CEffect>(0);
	m_magEffect[1] = NewGO<prefab::CEffect>(0);
	m_magEffect[0]->SetScale(EFFECT_MAGNET_SCALE);
	m_magEffect[1]->SetScale(EFFECT_MAGNET_SCALE);
	if (m_magnetState == MAGNETSTATE_REPULSION) {
		m_magEffect[0]->Init(u"Assets/effect/斥力.efk");
		m_magEffect[1]->Init(u"Assets/effect/斥力.efk");
	}
	else if (m_magnetState == MAGNETSTATE_GRAVITY) {
		m_magEffect[0]->Init(u"Assets/effect/引力.efk");
		m_magEffect[1]->Init(u"Assets/effect/引力.efk");
	}

	m_burstEffect = NewGO<prefab::CEffect>(0);
	m_burstEffect->SetScale(EFFECT_BURST_SCALE);

	m_hitEffect = NewGO<prefab::CEffect>(0);
	m_hitEffect->Init(u"Assets/effect/ダメージ.efk");
	m_hitEffect->SetScale(EFFECT_HIT_SCALE);

	m_SPFireEffect = NewGO<prefab::CEffect>(0);
	m_SPFireEffect->SetScale(EFFECT_SPFIRE_SCALE);

	m_SPGaugeMaxEffect = NewGO<prefab::CEffect>(0);
	m_SPGaugeMaxEffect->Init(u"Assets/effect/キュピーン.efk");
	m_SPGaugeMaxEffect->SetScale(EFFECT_SPGAUGEMAX_SCALE);

	m_SPChargeEffectRed = NewGO<prefab::CEffect>(0);
	m_SPChargeEffectRed->SetScale(EFFECT_SPCHARGE_SCALE);
	m_SPChargeEffectRed->Init(u"Assets/effect/斥力チャージ.efk");

	m_SPChargeEffectBlue = NewGO<prefab::CEffect>(0);
	m_SPChargeEffectBlue->SetScale(EFFECT_SPCHARGE_SCALE);
	m_SPChargeEffectBlue->Init(u"Assets/effect/引力チャージ.efk");
	//ここまでエフェクト

	m_gameScene = FindGO<GameScene>("gamescene");
	return true;
}

void Player::Update()
{
	//ポーズ中ならスキップ。
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause)
	{
		return;
	}

	//オープニングカメラ。
	if (m_gameScene->GetGameState() == GameScene::GameState::enBirdseye)
	{
		OpeningCamera();
	}
	//スタートダウンカウントダウン中
	else if (m_gameScene->GetGameState() == GameScene::GameState::enStartCountDown)
	{
		//カメラの移動。
		Camera();

		//キャラを前に向かせる

		//カメラの前方向
		m_cameraFront = m_position - g_camera3D[m_playerNum]->GetPosition();
		m_cameraFront.y = 0.0f;
		m_cameraFront.Normalize();

		//カメラの右方向
		m_cameraRight = Cross(g_vec3AxisY, m_cameraFront);

		float dot = m_cameraFront.Dot(Vector3::AxisZ);//内積
		float angle = acosf(dot);//アークコサイン
		if (m_cameraFront.x < 0) {
			angle *= -1;
		}
		m_rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(m_rot);

		//状態更新。
		UpdateState();
		//アニメーション選択。
		AnimationSelect();
	}
	else if(m_gameScene->GetGameState() == GameScene::GameState::enPlaying)
	{		
		//体力等ステータスのテキストを表示(後に画像にする。)
		DisplayStatus();

		//座標に応じて三角形の当たり判定の場所をセット。
		Collision();

		//磁力の変化
		ChangeMagnetPower();
		if (m_isKnockBack == true)
		{
			KnockBack();
		}
		else if (m_isKnockBack == false)
		{
			//移動
			Move();
			//必殺技
			SpecialAttack();
			//保持しているガレキの位置を制御する
			HoldDebris();
			//保持している爆弾の位置を制御する
			HoldBomb();
			//バーストを使用している?
			if (m_isBurst == true)
			{
				MagneticBurst();
			}
			else
			{
				MagneticBehavior();
			}

			//爆弾を投げる
			ThrowBomb();
		}

		//攻撃後の隙のタイマーを減らしていく
		m_attackCount--;
		//攻撃のクールタイムが終わると移動速度を戻す
		if (m_attackCount <= PLAYER_ATTACKCOUNT_CANATTACK && m_isBurst == false)
		{
			m_attackCount = PLAYER_ATTACKCOUNT_CANATTACK;
			m_isAttacking = false;
			m_characterSpeed = PLAYER_SPEED_RUN;
		}
		//状態更新。
		UpdateState();
		//アニメーション選択。
		AnimationSelect();

		//カメラの移動
		Camera();

		//斥力・引力エフェクト			

		//磁力エフェクトを再生
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
			
		//必殺技ゲージが溜まった瞬間のエフェクトが再生されていれば
		if (m_SPGaugeMaxEffect->IsPlay())
		{
			m_SPGaugeMaxEffect->SetPosition(m_magPosition);
		}
	
	}
	else if(m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		//ファイナルヒットカメラ。
		FinalHit();
		//状態更新。
		//UpdateState();
		//アニメーション選択。
		//AnimationSelect();
	}
	else if (m_gameScene->GetGameState() == GameScene::GameState::enDraw)
	{
		//保持しているガレキを浮遊させる。
		HoldDebris();

		//保持している爆弾を浮遊させる。
		HoldBomb();

		//カメラの移動。
		Camera();

		//キャラを前に向かせる

		//カメラの前方向
		m_cameraFront = m_position - g_camera3D[m_playerNum]->GetPosition();
		m_cameraFront.y = 0.0f;
		m_cameraFront.Normalize();

		//カメラの右方向
		m_cameraRight = Cross(g_vec3AxisY, m_cameraFront);

		float dot = m_cameraFront.Dot(Vector3::AxisZ);//内積
		float angle = acosf(dot);//アークコサイン
		if (m_cameraFront.x < 0) {
			angle *= -1;
		}
		m_rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(m_rot);

		m_animStatus = enStatus_Idle;
		//アニメーション選択。
		AnimationSelect();
	}
	
}
//体力、メビウスゲージの表示
void Player::DisplayStatus()
{
	//体力、チャージ、現在の自分の磁力の状態の表示
	
	//HPバー更新
	if (m_gameScene->GetGameState() == GameScene::GameState::enPlaying)
	{
		if (m_playerNum == NUMBER_PLAYER1) {
			m_DamageBarSpriteRender->SetPosition({ -9.0f + m_hp / 1000.0f * 299, 325.0f,0.0f });
			if (m_HPBarDarkSpriteRender->GetPosition().x > m_DamageBarSpriteRender->GetPosition().x) {
				Vector3 DarkPos = m_HPBarDarkSpriteRender->GetPosition();
				DarkPos.x -= 2.0f;
				m_HPBarDarkSpriteRender->SetPosition(DarkPos);
				if (m_HPBarDarkSpriteRender->GetPosition().x < m_DamageBarSpriteRender->GetPosition().x) {	//オーバー修正
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
				if (m_HPBarDarkSpriteRender->GetPosition().x > m_DamageBarSpriteRender->GetPosition().x) {	//オーバー修正
					DarkPos.x = m_DamageBarSpriteRender->GetPosition().x;
					m_HPBarDarkSpriteRender->SetPosition(DarkPos);
				}
			}
		}
	}

	//メビウスゲージの色を磁力から決定
	if (m_magnetState == MAGNETSTATE_REPULSION)
	{
		m_mobiusGauge->m_isRed = true;
	}
	else
	{
		m_mobiusGauge->m_isRed = false;
	}

	//メビウスゲージに現在の必殺技のチャージ量を渡す
	m_mobiusGauge->m_magnetCharge = m_magnetCharge;

	m_mobiusGauge->m_spCharge = m_specialAttackGauge;
}

//移動
void Player::Move()
{
	Vector3 oldPos = m_position;

	//カメラの前方向
	m_cameraFront = m_position - g_camera3D[m_playerNum]->GetPosition();
	m_cameraFront.y = 0.0f;
	m_cameraFront.Normalize();

	//カメラの右方向
	m_cameraRight = Cross(g_vec3AxisY, m_cameraFront);

	float dot = m_cameraFront.Dot(Vector3::AxisZ);//内積
	float angle = acosf(dot);//アークコサイン
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

	//穴に落ちた時の処理
	if (m_position.y <= HEIGHT_PLAYER_FALL) {
		Damage(DAMAGE_FALL);

		//敵から最も遠いリスポーン地点に移動する。
		Vector3 respawnPoint = m_stageModel->GetRespawnPoint(m_enemy->m_position);
		m_position = respawnPoint;
		m_charaCon.SetPosition(m_position);
		m_skinModelRender->SetPosition(m_position);

		//リスポーンしたので落下加速用のカウントをリセット。
		m_fallLoop = PLAYER_FALLLOOP_ZERO;
	}

	//足音
	if (g_pad[m_playerNum]->GetLStickXF() || g_pad[m_playerNum]->GetLStickYF())
	{
		m_footstepsTimer++;
		
		if (m_animStatus == enStatus_Walk) {
			if (m_footstepsTimer >= FOOTSTEPTIMER_WALK)
			{
				SoundOneShotPlay(L"Assets/sound/足音.wav", SOUND_SE_FOOTSTEP_VOLUME);

				m_footstepsTimer = FOOTSTEPTIMER_RESET;
			}
		}
		if (m_animStatus == enStatus_Run) {
			if (m_footstepsTimer >= FOOTSTEPTIMER_RUN)
			{
				SoundOneShotPlay(L"Assets/sound/足音.wav", SOUND_SE_FOOTSTEP_VOLUME);
				
				m_footstepsTimer = FOOTSTEPTIMER_RESET;
			}
		}
	}
	else
		m_footstepsTimer = FOOTSTEPTIMER_RESET;

	if (oldPos.y >= HEIGHT_PLAYER_FALLSOUND_PLAY && m_position.y < HEIGHT_PLAYER_FALLSOUND_PLAY)
	{
		SoundOneShotPlay(L"Assets/sound/落下音.wav", SOUND_SE_FALL_VOLUME);
	}	
}

//攻撃
void Player::Attack()
{
	if (g_pad[m_playerNum]->IsPress(enButtonRB1) && m_attackCount == PLAYER_ATTACKCOUNT_CANATTACK)
	{
		//ガレキを一つでも持っているなら
		if (m_holdDebrisVector.empty() == false)
		{
			//音を鳴らす
			SoundOneShotPlay(L"Assets/sound/シュート音.wav", SOUND_SE_DEBRISSHOOT_VOLUME);

			//一番最初に保持したガレキを発射
			auto debris = m_holdDebrisVector.front();
			//保持したガレキを発射モードにする
			debris->SetDebrisState(Debris::enBullet);

			//キャラクターのスピードを遅くする。
			m_characterSpeed = PLAYER_SPEED_WALK;

			//攻撃中のフラグをオン
			m_isAttacking = true;

			//攻撃の隙の持続時間
			m_attackCount = PLAYER_ATTACKCOUNT_AFTERSHOOT;
			
			//この場所に向かって撃つ(GetShootPointの中での参照受け取り用)
			Vector3 crossPoint;
			//発射先を計算。
			bool hitFlag = GetShootPoint(crossPoint);

			//発射先に敵もしくはステージのモデルがある。
			if (hitFlag)
			{
				//照準の指す方向に飛ばす
				debris->SetMoveDirection(crossPoint - debris->GetPosition());
			}
			else //ない。
			{
				Vector3 moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				moveDirection.y = 0.0f;
				debris->SetMoveDirection(moveDirection);
			}

			//発射したガレキを保持リストから削除
			m_holdDebrisVector.erase(m_holdDebrisVector.begin());

			//テキスト更新
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

//必殺技
void Player::SpecialAttack()
{
	//引力モードの必殺技のタイミングを指示できる。
	if (m_isGravityBulletAttack == true)
	{
		m_isGravityBulletAttack = false;
	}
	else if (g_pad[m_playerNum]->IsTrigger(enButtonLB3))
	{
		m_isGravityBulletAttack = true;
	}

	//必殺技ポイントが溜まっていてボタンを押したら
	//アニメーションに発射タイミングを合わせる。
	if (m_specialAttackGauge >= PLAYER_SPGAUGE_MAX && g_pad[m_playerNum]->IsTrigger(enButtonLB3))
	{
		m_SpecialAttackOn = true;		//アニメーションを必殺技にする。
		m_specialShotFlag = true;
	}
	if (m_specialShotFlag == true)
	{
		//発射する前に、チャージするようなエフェクト
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
			SoundOneShotPlay(L"Assets/sound/パワーチャージ.wav");
		}

		m_specialShotCount++;

		//発射前にダメージを受けたらキャンセル
		//if (m_HitOn == true) 
		//{
		//	//ゲージも0に。
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
		//引力なら
		if (m_magnetState == MAGNETSTATE_GRAVITY)
		{
			//音を鳴らす
			SoundOneShotPlay(L"Assets/sound/引力弾発射.wav", SOUND_SE_SHOOTGRAVITY_VOLUME);

			//発射エフェクト
			m_SPFireEffect->Init(u"Assets/effect/引力弾発射.efk");
			m_SPFireEffect->SetPosition({
				m_position.x + m_cameraFront.x * 50.0f,
				m_position.y + 50.0f,
				m_position.z + m_cameraFront.z * 50.0f
				});
			m_SPFireEffect->Play();

			GravityBullet* gravityBullet = NewGO<GravityBullet>(0, "gravitybullet");
			gravityBullet->SetPosition(m_magPosition);
			gravityBullet->SetParent(this);

			//この場所に向かって撃つ(GetShootPointの中での参照受け取り用)
			Vector3 crossPoint;
			//発射先を計算。
			bool hitFlag = GetShootPoint(crossPoint);

			if (hitFlag)
			{
				//照準の指す方向に飛ばす
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

			//撃ったので必殺技ゲージを0に
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
		else//斥力なら
		{
			//弾を1発でも持ってる?
			if (m_holdDebrisVector.size() != 0)
			{
				//音を鳴らす
				SoundOneShotPlay(L"Assets/sound/気弾1.wav");

				//発射エフェクト
				m_SPFireEffect->Init(u"Assets/effect/斥力弾発射.efk");
				m_SPFireEffect->SetPosition({ 
					m_position.x + m_cameraFront.x * 50.0f,
					m_position.y + 50.0f,
					m_position.z + m_cameraFront.z * 50.0f
					});
				m_SPFireEffect->Play();

				//この場所に向かって撃つ(GetShootPointの中での参照受け取り用)
				Vector3 crossPoint;
				//発射先を計算。
				bool hitFlag = GetShootPoint(crossPoint);

				if (hitFlag)
				{
					//照準の指す方向に飛ばす
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

				//テキスト更新
				m_bulletNumFont->SetText(std::to_wstring(m_holdDebrisVector.size()));
				if (m_playerNum == NUMBER_PLAYER1)
				{
					m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_ONESPLACE);
				}
				else
				{
					m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_ONESPLACE);
				}


				//撃ったので必殺技ゲージを0に
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
				//1発も弾持ってないから不発にする。エラー音?ゲージも消費しなくていい
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
		//爆弾を一つでも持っているなら
		if (m_holdBombVector.empty() == false)
		{
			//音を鳴らす
			SoundOneShotPlay(L"Assets/sound/投げる音.wav");

			//選択している爆弾を発射
			auto debris = m_holdBombVector.begin() + m_selectBombNo;
			//保持した爆弾を発射モードにする
			(*debris)->SetBombState(Bomb::enBullet);

			//投げ始めの位置を調整
			Vector3 throwPosition = (*debris)->GetPosition();
			throwPosition.y -= 30.0f;
			(*debris)->SetPosition(throwPosition);

			//キャラクターのスピードを遅くする。
			m_characterSpeed = PLAYER_SPEED_WALK;

			//攻撃中のフラグをオン
			m_isAttacking = true;

			//攻撃の隙の持続時間
			m_attackCount = PLAYER_ATTACKCOUNT_AFTERBOMB;

			Vector3 front = g_camera3D[m_playerNum]->GetForward();
			front.y += 0.5f;
			front.Normalize();
			(*debris)->SetMoveDirection(front);

			//発射した爆弾を保持リストから削除
			m_holdBombVector.erase(m_holdBombVector.begin() + m_selectBombNo);

			//選択している爆弾のナンバーをリセット。
			m_selectBombNo = 0;
		}

	}
}

//保持しているガレキの動きを制御する
void Player::HoldDebris()
{
	//回転角度を増加させていく。
	m_holdDebrisRotateDeg += DEBRIS_ROTATEDEG_PER_FRAME;
	if (m_holdDebrisRotateDeg >= 360.0f)
	{
		m_holdDebrisRotateDeg -= 360.0f;
	}

	//回転の中心を設定する。
	Vector3 centerOfRotation = m_position;
	centerOfRotation.y += BULLETHOLDER_TO_CENTER_OF_BULLET_ROTATION;

	//回転の中心をプレイヤーより後ろに。
	Vector3 cameraDir = g_camera3D[m_playerNum]->GetForward();
	cameraDir.y = 0.0f;
	cameraDir.Normalize();
	centerOfRotation -= cameraDir * BULLETHOLDER_TO_BEHIND_PLAYER;

	//保持しているガレキが1個以上あれば
	if (m_holdDebrisVector.empty() == false)
	{
		//360度をガレキの数-1の数で割って1個あたりの角度を求める
		//次に発射する1個は身体の前に置くので-1
		int vectorSize = m_holdDebrisVector.size();
		float degPerOneDebris = 360.0f / (vectorSize - 1);

		//autoFor文の中でも回数をカウントするための変数
		int i = 0;
		for (auto debris : m_holdDebrisVector)
		{
			//次に発射するガレキのみ自分の前に。
			if(i == DEBRIS_NEXTSHOOT_INDEX)
			{ 
				debris->SetPosition(m_magPosition + (cameraDir * DEBRIS_NEXTSHOOT_DISTANCE));
			}
			else
			{
				//Y方向に50のベクトルを回転させて回転の中心点から伸ばす
				Quaternion debrisRot;

				//iの値によって順番に並べる
				debrisRot.SetRotationDeg(cameraDir, m_holdDebrisRotateDeg + degPerOneDebris * (i-1));

				//コピーしてから回転を適用する。
				Vector3 toDebris = CENTER_TO_FIRSTDEBRIS;
				debrisRot.Apply(toDebris);

				//回転の中心点から伸ばす
				debris->SetPosition(centerOfRotation + toDebris);
			}

			i++;
		}
	}
}

//保持している爆弾を浮遊させる。
void Player::HoldBomb()
{
	//選択する爆弾を決定
	if (g_pad[m_playerNum]->IsTrigger(enButtonRight))
	{
		//選択している爆弾の番号がコンテナのサイズを超えていたら
		if (++m_selectBombNo >= m_holdBombVector.size())
		{
			//一周回って0になる
			m_selectBombNo = BOMB_FIRST_OBTAIN_INDEX;
		}
	}
	else if (g_pad[m_playerNum]->IsTrigger(enButtonLeft))
	{
		//選択している爆弾の番号がマイナスになっていたら
		if (--m_selectBombNo < BOMB_FIRST_OBTAIN_INDEX)
		{
			//一周回ってコンテナのサイズ-1になる
			m_selectBombNo = m_holdBombVector.size() - 1;
		}
	}

	//回転の中心を設定する。
	Vector3 centerOfRotation = m_position;
	centerOfRotation.y += BULLETHOLDER_TO_CENTER_OF_BULLET_ROTATION;

	//回転の中心をプレイヤーより後ろに。
	Vector3 cameraDir = g_camera3D[m_playerNum]->GetForward();
	cameraDir.y = 0;
	cameraDir.Normalize();
	centerOfRotation -= cameraDir * BULLETHOLDER_TO_BEHIND_PLAYER;

	//保持している爆弾が1個以上あれば
	if (m_holdBombVector.empty() == false)
	{
		//360度をガレキの数-1の数で割って1個あたりの角度を求める
		//次に発射する1個は身体の前に置くので-1
		int vectorSize = m_holdBombVector.size();
		float degPerOneDebris = 360.0f / vectorSize;

		//autoFor文の中でも回数をカウントするための変数
		int i = 0;
		for (auto debris : m_holdBombVector)
		{
			//Y方向に30のベクトルを回転させて回転の中心点から伸ばす
			Quaternion debrisRot;

			//選んでいる爆弾が常に一番上になるようにする。
			debrisRot.SetRotationDeg(cameraDir, degPerOneDebris * (i - m_selectBombNo));

			//toDebris本体に回転を適用すると他の場所にも影響が出るのでコピーしてから回転を適用する。
			Vector3 toBomb = CENTER_TO_FIRSTBOMB;
			debrisRot.Apply(toBomb);

			//回転の中心点から伸ばす
			debris->SetPosition(centerOfRotation + toBomb);

			i++;
		}
	}
}

//磁力バーストを使用していない時の通常の動き
void Player::MagneticBehavior()
{
	//磁力の状態によって動きが分岐。
	switch (m_magnetState)
	{
	case MAGNETSTATE_GRAVITY://引力

		break;
	case MAGNETSTATE_REPULSION: //斥力

		//斥力モードでのみガレキを発射できる
		Attack();

		break;
	default:
		MessageBoxA(nullptr, "磁力の状態が異常です。\n", "エラー", MB_OK);
		break;
	}

	//LB1を押して攻撃中でなかった場合バースト状態に移行
	if (g_pad[m_playerNum]->IsTrigger(enButtonLB1) && m_isAttacking == false)
	{
		//磁力ゲージを300消費。
		m_magnetCharge -= PLAYER_BURST_COST;
		if (m_magnetCharge < PLAYER_MAGNETCHARGE_ZERO)
		{
			m_magnetCharge = PLAYER_MAGNETCHARGE_ZERO;
		}

		m_isBurst = true;
		m_burstCount = PLAYER_BURSTCOUNT_START;

		switch (m_magnetState)
		{
		case MAGNETSTATE_GRAVITY://引力
			//バースト音を再生
			SoundOneShotPlay(L"Assets/sound/引力バースト音.wav", SOUND_SE_BURST_VOLUME);
			//エフェクトを表示
			m_burstEffect->Init(u"Assets/effect/引力バースト.efk");
			m_burstEffect->Play();
			break;

		case MAGNETSTATE_REPULSION://斥力
			//バースト音を再生
			SoundOneShotPlay(L"Assets/sound/斥力バースト音.wav", SOUND_SE_BURST_VOLUME);
			//エフェクトを表示
			m_burstEffect->Init(u"Assets/effect/斥力バースト.efk");
			m_burstEffect->Play();
			break;
		}
	}
}

//磁力バースト
void Player::MagneticBurst()
{
	//バーストエフェクトの位置を設定
	m_burstEffect->SetPosition(m_position);

	//バースト中は移動速度は0に
	m_characterSpeed = PLAYER_SPEED_STOP;

	//敵までのベクトル
	Vector3 toEnemy = m_enemy->m_position - m_position;

	//敵に与える力
	Vector3 force = toEnemy;
	//縦軸は影響しない
	force.y = 0;
	force.Normalize();

	//磁力の状態によって動きが分岐。
	switch (m_magnetState)
	{
	case MAGNETSTATE_GRAVITY://引力

		//引っ張るのでマイナスに
		force *= -10.0f;
		//近すぎる時は引っ張らない
		if (toEnemy.Length() > BURST_RANGE_MIN && toEnemy.Length() < BURST_RANGE_MAX)
		{
			m_enemy->ExecuteCharacon(force);

			//敵の弾をまだ奪っていない時
			if (m_isSteal == false)
			{

				//敵の保持する弾が1発以上あるなら
				if (m_enemy->m_holdDebrisVector.size() != 0)
				{
					int enemyHoldDebrisIndex = 0;
					//持ってるガレキをドロップさせる
					//全部ではなく3つまでにしてみる。

					//敵の持っているガレキのリストを走査
					for (auto iterator = m_enemy->m_holdDebrisVector.begin(); iterator != m_enemy->m_holdDebrisVector.end(); iterator++)
					{
						//ドロップ状態にさせていく。すぐ吸うのでポップ状態ではない。
						(*iterator)->SetOnGroundFlag(false);
						(*iterator)->SetDebrisState(Debris::enDrop);

						//カウントをすすめる
						enemyHoldDebrisIndex++;

						//3になったら3つ吸ったのでブレイク。
						if (enemyHoldDebrisIndex == 3)
						{
							break;
						}
					}

					//コンテナのサイズが3以下なら削除
					if (m_enemy->m_holdDebrisVector.size() < 3)
					{
						m_enemy->m_holdDebrisVector.clear();
					}
					else//3以上なら3個分削除
					{
						m_enemy->m_holdDebrisVector.erase(m_enemy->m_holdDebrisVector.begin(), m_enemy->m_holdDebrisVector.begin() + 3);
					}

					//テキスト更新
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

					//もう敵の弾を奪ったのでフラグ変更
					m_isSteal = true;
				}

				//敵が1つでも爆弾を持っていれば
				if (m_enemy->m_holdBombVector.size() != 0)
				{
					//爆弾をドロップさせる。
					(*m_enemy->m_holdBombVector.begin())->SetBombState(Bomb::enDrop);

					//ドロップさせた爆弾を相手のコンテナから削除
					m_enemy->m_holdBombVector.erase(m_enemy->m_holdBombVector.begin());

					//もう敵の弾を奪ったのでフラグ変更
					m_isSteal = true;
				}

				if (m_isSteal == true)
				{
					//リスクのある行動を成功させたので必殺技ゲージをプラス。
					ChargeSpecialAttackGauge(SPGAUGE_BULLETSTEAL_POINT);
				}
			}
		}

		break;
	case 1: //斥力

		force *= 10.0f;
		if (toEnemy.Length() < BURST_RANGE_MAX)
		{
			m_enemy->ExecuteCharacon(force);
		}

		break;
	default:
		MessageBoxA(nullptr, "磁力の状態が異常です。\n", "エラー", MB_OK);
		break;
	}

	m_burstCount--;
	//バーストカウントが0になるとバースト終了。
	if (m_burstCount <= PLAYER_BURSTCOUNT_END)
	{
		m_isBurst = false;
		//敵の弾を奪ったフラグをリセット。
		m_isSteal = false;

		//スピードも戻す
		m_characterSpeed = PLAYER_SPEED_RUN;
	}
}

void Player::ChangeMagnetPower()
{
	//磁力ゲージの自然減少
	m_magnetCharge -= PLAYER_MAGNETCHARGE_DECAY_VALUE;
	if (m_magnetCharge < PLAYER_MAGNETCHARGE_ZERO)
	{
		m_magnetCharge = PLAYER_MAGNETCHARGE_ZERO;
	}

	//磁力ゲージが0以下かつ、バースト中や必殺技発動中でなければ
	if (m_magnetCharge <= PLAYER_MAGNETCHARGE_ZERO &&
		m_isBurst == false &&
		m_specialShotFlag == false	
		)	{
		//磁力の状態が-1か1なので、-1を掛ければ反転する。
		//普通にswitchしてもいいかも。
		m_magnetState *= -1;

		if (m_isAttacking == true)
		{
			m_isAttacking = false;
			m_characterSpeed = PLAYER_SPEED_RUN;
		}

		//チャージを回復。
		m_magnetCharge = PLAYER_MAGNETCHARGE_MAX;
	
		//磁力エフェクト変更
		if (m_magnetState == MAGNETSTATE_GRAVITY) {
			m_magEffect[0]->Init(u"Assets/effect/引力.efk");
			m_magEffect[1]->Init(u"Assets/effect/引力.efk");
		}
		else if (m_magnetState == MAGNETSTATE_REPULSION) {
			m_magEffect[0]->Init(u"Assets/effect/斥力.efk");
			m_magEffect[1]->Init(u"Assets/effect/斥力.efk");
		}
	}
}

//カメラの移動
void Player::Camera()
{
	//カメラ関連
	if (g_pad[m_playerNum]->IsTrigger(enButtonRB3))
	{
		m_isLockon = !m_isLockon;//ロック切り替え
	}

	//プレイヤーの頭上90を基準として、そこからカメラに伸びるベクトルを回転させる。
	Vector3 targetPos = m_position;
	targetPos.y += PLAYER_POSITION_TO_CAMERATARGET;

	if (m_isLockon)
	{
		//ロックしている時は、敵の中心からプレイヤーの頭上へのベクトルの延長線上にカメラがある。
		Vector3 toEnemy = m_enemy->m_magPosition - targetPos;
		toEnemy.Normalize();
		m_toCameraDir = toEnemy * -1.0f;

		//コントローラーの入力でカメラの向きをちょっとずらせる
		m_cameraQRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * 5.0f);
		m_cameraQRotY.Apply(m_toCameraDir);

		Quaternion qRotX;
		Vector3 right = g_camera3D[m_playerNum]->GetRight();
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -5.0f);

		qRotX.Apply(m_toCameraDir);
	}
	else
	{
		//ロックされていない時はカメラへのベクトルを回転させる。
		m_cameraQRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * m_sensitivity);
		m_cameraQRotY.Apply(m_toCameraDir);

		Quaternion qRotX;
		Vector3 right = g_camera3D[m_playerNum]->GetRight();
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -m_sensitivity);

		//回転先の角度を確認して上すぎる時または下すぎる時は回転させない。
		Vector3 checkToCamera = m_toCameraDir;
		qRotX.Apply(checkToCamera);
		checkToCamera.Normalize();
		float t = checkToCamera.Dot(Vector3::Up);
		if (t < 0.999f && t > -0.999f)
		{
			qRotX.Apply(m_toCameraDir);
		}
	}

	//本来のカメラ位置
	m_cameraPos = targetPos + m_toCameraDir * PLAYER_CAMERATARGET_TO_CAMERAPOSITION;

	//バネカメラ的なもの　カメラのターゲット位置から本来のカメラ位置へのベクトルがステージに衝突しているか判定
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(targetPos, m_cameraPos, crossPoint);

	if (isHit == false)
	{
		//当たっていないなら本来のカメラ位置
		g_camera3D[m_playerNum]->SetPosition(m_cameraPos);
	}
	else
	{
		//当たっているなら当たった位置にカメラを移動
		g_camera3D[m_playerNum]->SetPosition(crossPoint);
	}
	g_camera3D[m_playerNum]->SetTarget(targetPos);

	//ライト
	Vector3 Direction = m_magPosition - m_cameraPos;
	Direction.Normalize();
	m_spotLight->SetDirection(Direction);
	m_spotLight->SetPosition(m_cameraPos);
}

//当たり判定
void Player::Collision()
{
	//三角形の当たり判定×2で四角形の当たり判定をつくる
	//敵への向きとY軸の外積で、敵から見た横側を取得。
	Vector3 diff = m_enemy->m_position - m_position;
	diff.Normalize();
	Vector3 side = Cross(diff, Vector3::AxisY);

	//四角形の左上
	Vector3 squarePos1 = m_position;
	squarePos1.y += PLAYER_AIMABLE_HEIGHT;
	squarePos1 += side * PLAYER_AIMABLE_WIDTH;

	//四角形の左下
	Vector3 squarePos2 = m_position;
	squarePos2.y -= PLAYER_AIMABLE_HEIGHT;
	squarePos2 += side * PLAYER_AIMABLE_WIDTH;

	//四角形の右上
	Vector3 squarePos3 = m_position;
	squarePos3.y += PLAYER_AIMABLE_HEIGHT;
	squarePos3 -= side * PLAYER_AIMABLE_WIDTH;

	//四角形の右下
	Vector3 squarePos4 = m_position;
	squarePos4.y -= PLAYER_AIMABLE_HEIGHT;
	squarePos4 -= side * PLAYER_AIMABLE_WIDTH;

	//1つ目の三角形の頂点は左上、左下、右上。
	m_triCollider[0].SetVertex(squarePos1, squarePos2, squarePos3);
	//2つ目の三角形の頂点は右下、右上、左下。
	m_triCollider[1].SetVertex(squarePos4, squarePos3, squarePos2);

	//カプセルの当たり判定をつくる。
	Vector3 legPos = m_position;
	Vector3 headPos = m_position;
	headPos.y += PLAYER_COLLIDER_HEIGHT;

	//カプセルの始点と終点を更新する。(半径はStart関数で最初だけ決めてもよい)
	m_collider.SetStartPoint(legPos);
	m_collider.SetEndPoint(headPos);
	m_collider.SetRadius(PLAYER_COLLIDER_RADIUS);
}

//自分の体力にダメージを与える
void Player::Damage(int damage)
{	
	m_hp -= damage;
	m_HitOn = true;//アニメーションフラグ
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

	//与えたダメージ量を相手に表示する
	DamageDisplay* damagedisplay = NewGO<DamageDisplay>(0, "damagedisplay");
	damagedisplay->SetDamagePosition(m_position);
	damagedisplay->SetEnemyNum(m_enemy->m_playerNum);
	damagedisplay->SetDamage(damage);

	//HP200以下で赤くなる
	if (m_hp <= 200 && m_hpBarRedFlag == false) {
		m_HPBarSpriteRender->Init("Assets/Image/HP_Bar_Red.dds", 308, 32);
		m_hpBarRedFlag = true;
	}

	//ダメージエフェクト
	m_hitEffect->SetPosition({ m_position.x, m_position.y + 50, m_position.z });

	//カメラの前方向
	m_damageEffectFront.y = 0.0f;
	m_damageEffectFront.Normalize();

	float innerProduct = m_damageEffectFront.Dot(Vector3::AxisZ); //内積
	float angle = acosf(innerProduct);//アークコサイン
	Quaternion rot;
	rot.SetRotation(Vector3::AxisY, angle);

	m_hitEffect->SetRotation(rot);
	m_hitEffect->Play();
	//ここまでエフェクト処理
}

//必殺技ゲージをチャージする。
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
			//エフェクト
			m_SPGaugeMaxEffect->SetPosition({ m_position.x,m_position.y + 50.0f, m_position.z });
			m_SPGaugeMaxEffect->Play();
			//SE
			SoundOneShotPlay(L"Assets/sound/きらーん.wav", 0.5f);

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

//勝利した時
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

//敗北した時
void Player::Lose()
{	
	m_Lose = true;	

	//空中で倒れた時用に、現在位置から下方向に向かってレイを飛ばす。
	Vector3 crossPoint;
	bool hitflag = false;

	Vector3 linestart = m_position;
	Vector3 lineend = m_position;
	linestart.y += 50.0f;
	lineend.y -= 1000.0f;

	hitflag = m_stageModel->isLineHitModel(linestart, lineend, crossPoint);

	if (hitflag == true)
	{
		//レイがヒットしているならヒットした位置に移動。
		m_position = crossPoint;
	}
	else
	{
		//ヒットしないということは穴の上なのでリスポーン位置にプレイヤーの位置を移動。
		m_position = m_stageModel->GetRespawnPoint(m_enemy->m_position);
	}

	m_skinModelRender->SetPosition(m_position);

	m_LastFrontDir = m_cameraFront;
}



//攻撃状態に切り替えできたら切り替える。
void Player::TryChangeStatusAttack()
{
	if (m_magnetState == 1 && m_holdDebrisVector.empty() == false && g_pad[m_playerNum]->IsPress(enButtonRB1)) {
		m_animStatus = enStatus_Attack;		
	}
}

//特殊攻撃状態に切り替える
void Player::TryChangeStatusSpecialAttack()
{
	if (m_SpecialAttackOn==true)
	{
		m_animStatus = enStatus_SpecialAttack;	
		m_SpecialAttackOn = false;	
	}
}

//バースト状態に切り替える
void Player::TryChangeStatusBurst()
{
	if (m_isBurst == true)
	{
		m_animStatus = enStatus_Burst;
	}
}

//走り状態に切り替えできたら切り替える。
void Player::TryChangeStatusRun()
{
	if (m_moveAmount.LengthSq() > 5.0f) {
		m_animStatus = enStatus_Run;
	}
}

//歩き状態に切り替えできたら切り替える。
void Player::TryChangeStatusWalk()
{
	if (m_moveAmount.LengthSq() <= 5.0f && m_moveAmount.LengthSq() > 0.0f) {
		m_animStatus = enStatus_Walk;
	}
}

//落下状態に切り替える
void Player::TryChangeStatusFall()
{
	if (m_charaCon.IsOnGround() == false)
	{
		m_animStatus = enStatus_Fall;
		m_HitOn = false;//被弾状態リセット
	}
}

//待機状態に切り替えできたら切り替える。
void Player::TryChangeStatusIdle()
{
	if (m_moveAmount.LengthSq() <= 0.001f) {
		m_animStatus = enStatus_Idle;
	}
}

//被弾状態に切り替える
void Player::TryChangeStatusHit()
{
	if (m_HitOn == true )
	{		
		m_animStatus = enStatus_Hit;
	}	
}

//アニメーションの状態更新
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

//カメラが向いている方向に敵のモデルもしくはステージのモデルがあるかを計算する。
bool Player::GetShootPoint(Vector3& crossPoint)
{
	//発射先の決定

	//カメラの位置から向いている方向に飛ばすレイを作成。
	//キャラクターの位置からじゃないことに注意。
	//レイの向き
	Vector3 testRayDir = g_camera3D[m_playerNum]->GetForward();
	//レイの始点はプレイヤーの頭上(カメラのTargetは頭上にある)
	Vector3 testRayStart = g_camera3D[m_playerNum]->GetTarget();
	//レイの始点と向きから求めたレイの終点(10000以上の距離狙うことはないと思うので距離は10000に設定)
	Vector3 testRayEnd = testRayStart + testRayDir * 10000.0f;

	//交差したかフラグ。
	bool hitFlag = false;

	//まず敵キャラクター付近の板ポリ当たり判定を検索する。
	for (auto tricollider : m_enemy->m_triCollider)
	{
		hitFlag = tricollider.isHit(testRayStart, testRayEnd, crossPoint);
		if (hitFlag == true)
		{
			//1回でもヒットしていたらreturn
			return hitFlag;
		}
	}

	//敵キャラクター付近にヒットしなかったらステージのモデルを検索する。
	if (hitFlag == false)
	{
		hitFlag = m_stageModel->isLineHitModel(testRayStart, testRayEnd, crossPoint);
	}

	return hitFlag;
}
void Player::KnockBack() {
	//ノックバックする向きを設定
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

	if (g_pad[m_playerNum]->IsTrigger(enButtonA))//オープニングカメラスキップ
	{
		m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
	}
	
	if (m_openingCameraCount < 250)//250フレーム経てば
	{
		Vector3 toPos = m_position;
		toPos.y = 400;
		//toPosを回す。
		Quaternion qRotY;
		//Y軸回りに回転するクォータニオンをつくる
		m_openingCameraRotationRad += 0.01;
		qRotY.SetRotation(Vector3::AxisY, m_openingCameraRotationRad);
		//クォータニオンを使ってtoPosを回す
		qRotY.Apply(toPos);
		m_sequenceCameraPos = toPos;
		g_camera3D[m_playerNum]->SetTarget(m_targetPos);
	}
	else//キャラに向かってカメラを移動させる
	{
		Vector3 PlayerPos = m_position;
		PlayerPos.y = m_position.y + 90.0f;//プレイヤーの頭の位置
		
		
		Vector3 targetVec = PlayerPos - m_sequenceCameraPos;
		if (targetVec.Length() < 250)//カメラが近づけばオープニングカメラ終了
		{
			m_gameScene->SetGameState(GameScene::GameState::enStartCountDown);
		}
		targetVec.Normalize();
		m_sequenceCameraPos += targetVec*m_openingCameraGainSpeed;
		m_openingCameraGainSpeed += 0.1;
		g_camera3D[m_playerNum]->SetTarget(PlayerPos);
	}
	g_camera3D[m_playerNum]->SetPosition(m_sequenceCameraPos);

	//キャラを初期方向に向かせる

	//キャラの前方向はm_cameraFront;

	float dot = m_cameraFront.Dot(Vector3::AxisZ);//内積
	float angle = acosf(dot);//アークコサイン
	if (m_cameraFront.x < 0) {
		angle *= -1;
	}
	m_rot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(m_rot);
}

void Player::FinalHit()//決着がついたときのカメラ
{	
	m_gameScene->StopGameBGM();
	
	if (m_FirstTime == true) {//一回だけ流れるループ
		//画面分割を終了
		GameObjectManager::GetInstance()->Set2ScreenMode(false);
		//画面に出ているやつを消す
		//HPバー、画面分割線、メビウスゲージを消す
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
		//弾も消す
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
	
	if (m_playerNum == m_loserNum)//敗者を写す
	{		
		m_skinModelRender->PlayAnimation(enAnimationClip_Death);//アニメーション再生
		
		Vector3 winnerFrontPos;//勝者の位置から少し前に伸びるベクトル		
		Vector3 winnerHeadPos = m_enemy->m_position;
		winnerHeadPos.y += 50;//勝者の頭の位置
		Vector3 LastRight = Cross(g_vec3AxisY, m_LastFrontDir);//最後に向いていた向きの右ベクトル
		Vector3 targetPos = m_position;//ターゲットポジション
		m_sequenceCameraPos = targetPos;//カメラのポジション
		targetPos.y += 20;//少し上に設定する
		targetPos += m_LastFrontDir * -30;//キャラの少し後ろに伸ばす	
		m_sequenceCameraPos.y += 100;//キャラより少し上くらい
		//ループの値に合わせてステータスを変える
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
		case 0://右からのカメラ
			m_skinModelRender->SetAnimationSpeed(0.1f);//アニメーションを遅くする
			m_enemy->m_skinModelRender->SetAnimationSpeed(0.1f);
			m_sequenceCameraPos += LastRight * 200;//右
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 1://左からのカメラ
			m_sequenceCameraPos += LastRight * -200;//左
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 2://前からのカメラ
			m_sequenceCameraPos += m_LastFrontDir * 200;//正面
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 3://自分を写しながら敵を向いたカメラ
			m_skinModelRender->SetAnimationSpeed(1.0f);//アニメーションスピードをもとに戻す
			m_enemy->m_skinModelRender->SetAnimationSpeed(1.0f);
			//敵のちょっと前と自分を結んだ線を正規化して後ろに少し伸ばす
			m_winnerVec=(winnerHeadPos + m_enemy->m_LastFrontDir * 200) - m_position;
			m_winnerVec.Normalize();
			m_sequenceCameraPos += m_winnerVec*-200;//後ろ
			m_winnerWaistPos = m_enemy->m_position;//敵の腰の位置
			m_winnerWaistPos.y += 20;
			g_camera3D[0]->SetTarget(m_winnerWaistPos);
			break;
		case 4://敵の前まで移動する
			//カメラを敵の前まで移動させる				
			winnerFrontPos = (m_enemy->m_position + m_enemy->m_LastFrontDir * 150 ) - (m_position + m_winnerVec * -200);//	
			if (m_coef < 1.0f)//ベクトルに掛ける値
				m_coef += 0.01f;
			if (m_coef > 0.5f)//カメラが半分の距離まで行くとアニメーションを再生する
			{
				m_enemy->m_WinAnimOn = true;
			}
			m_sequenceCameraPos += (winnerFrontPos * (pow(m_coef,1.5)) );//指数関数的にカメラが近づく

			m_sequenceCameraPos += m_winnerVec * -200;//case3のときのカメラの位置と合わせるため
			
			g_camera3D[0]->SetTarget(m_winnerWaistPos);
			break;
		default:
			break;
		}

		//SE
		if (m_LoseCameraLoop == 0 || m_LoseCameraLoop == 50 || m_LoseCameraLoop == 100)
		{
			//音を再生
			SoundOneShotPlay(L"Assets/sound/K.O..wav", 1.5f);
		}
		else if (m_LoseCameraLoop == 250)
		{
			//ジングルを再生
			SoundOneShotPlay(L"Assets/sound/yattaze!1.wav", 1.5f);
		}
		else if (m_LoseCameraLoop == 300)
		{
			if (m_loserNum == 1)	//1Pが勝利した場合
			{
				m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(5);
				m_resultWinnerSprite->Init("Assets/Image/1P.DDS", 148, 120);
				m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
				m_resultWinnerSprite->SetPosition({ -120.0f, -120.0f, 0.0f });
				m_resultWinnerSprite->SetScale({ 1.0f, 1.0f, 1.0f });	

				//ボイス再生
				SoundOneShotPlay(L"Assets/sound/Player1Win.wav", 3.0f);
			}
			else if (m_loserNum == 0)	//2Pが勝利した場合
			{
				m_resultWinnerSprite = NewGO<prefab::CSpriteRender>(5);
				m_resultWinnerSprite->Init("Assets/Image/2P.DDS", 180, 128);
				m_resultWinnerSprite->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
				m_resultWinnerSprite->SetPosition({ -120.0f, -120.0f, 0.0f });
				m_resultWinnerSprite->SetScale({ 1.0f, 1.0f, 1.0f });	

				//ボイス再生
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
	else//勝者のとき
	{
		if (m_WinAnimOn==true)//カメラが半分の距離まで行くとアニメーションを再生する
		{			
			m_skinModelRender->PlayAnimation(enAnimationClip_Winner);
		}
		else
		{
			m_skinModelRender->PlayAnimation(enAnimationClip_Idle);
		}
	}
}
