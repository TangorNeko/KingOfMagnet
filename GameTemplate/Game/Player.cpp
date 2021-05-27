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
	if (m_playerNum == 0)//プレイヤーの色を変える
		m_skinModelRender->Init("Assets/modelData/Player1.tkm", "Assets/modelData/Mage.tks", animationClips, enAnimationClip_num);
	if (m_playerNum == 1)
		m_skinModelRender->Init("Assets/modelData/Player2.tkm", "Assets/modelData/Mage.tks", animationClips, enAnimationClip_num);

	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetPosition(m_position);

	//キャラコンの初期化
	m_charaCon.Init(10.0f, 50.0f, m_position);

	//残弾数表示の初期化
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

	//必殺ゲージの溜まり具合を表示するフォント
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

	//照準表示の初期化
	m_crosshairRender = NewGO<prefab::CSpriteRender>(1);
	m_crosshairRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));
	m_crosshairRender->Init("Assets/Image/Sight.dds", 32, 32);

	//ステージのクラスを取得
	m_stageModel = FindGO<BackGround>("background");

	//HPバー
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

	//プレイヤーのライト
	m_spotLight = NewGO<prefab::CSpotLight>(0);
	m_spotLight->SetColor({ 10.0f,10.0f,10.0f });
	m_spotLight->SetRange(250.0f);
	m_spotLight->SetAngleDeg(30.0f);
	
	
	//エフェクト関連
	m_magEffect[0] = NewGO<prefab::CEffect>(0);
	m_magEffect[1] = NewGO<prefab::CEffect>(0);
	m_magEffect[0]->SetScale({ 25.0f, 25.0f, 25.0f });
	m_magEffect[1]->SetScale({ 25.0f, 25.0f, 25.0f });
	if (m_magPower == 1) {
		m_magEffect[0]->Init(u"Assets/effect/斥力.efk");
		m_magEffect[1]->Init(u"Assets/effect/斥力.efk");
	}
	else if (m_magPower == -1) {
		m_magEffect[0]->Init(u"Assets/effect/引力.efk");
		m_magEffect[1]->Init(u"Assets/effect/引力.efk");
	}

	m_burstEffect = NewGO<prefab::CEffect>(0);
	m_burstEffect->SetScale({ 50.0f, 50.0f, 50.0f });

	m_hitEffect = NewGO<prefab::CEffect>(0);
	m_hitEffect->Init(u"Assets/effect/ダメージ.efk");
	m_hitEffect->SetScale({ 10.0f, 10.0f, 10.0f });

	m_SPEffect = NewGO<prefab::CEffect>(0);
	m_SPEffect->SetScale({ 20.0f, 20.0f, 20.0f });

	m_SPGaugeMaxEffect = NewGO<prefab::CEffect>(0);
	m_SPGaugeMaxEffect->Init(u"Assets/effect/キュピーン.efk");
	m_SPGaugeMaxEffect->SetScale({ 8.0f, 8.0f, 8.0f });

	m_SPFirstEffectRed = NewGO<prefab::CEffect>(0);
	m_SPFirstEffectRed->SetScale({ 70.0f, 70.0f, 70.0f });
	m_SPFirstEffectRed->Init(u"Assets/effect/斥力チャージ.efk");

	m_SPFirstEffectBlue = NewGO<prefab::CEffect>(0);
	m_SPFirstEffectBlue->SetScale({ 70.0f, 70.0f, 70.0f });
	m_SPFirstEffectBlue->Init(u"Assets/effect/引力チャージ.efk");
	//ここまでエフェクト

	m_gameScene = FindGO<SampleScene>("gamescene");
	return true;
}

void Player::Update()
{
	//ポーズ中ならスキップ。
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPause)
	{
		return;
	}

	//オープニングカメラ。
	if (m_gameScene->GetGameState() == SampleScene::GameState::enBirdseye)
	{
		OpeningCamera();
	}
	//スタートダウンカウントダウン中
	else if (m_gameScene->GetGameState() == SampleScene::GameState::enStartCountDown)
	{
		//カメラの移動。
		Camera();

		//キャラを前に向かせる

		//カメラの前方向
		m_front = m_position - g_camera3D[m_playerNum]->GetPosition();
		m_front.y = 0.0f;
		m_front.Normalize();

		//カメラの右方向
		right = Cross(g_vec3AxisY, m_front);

		n = m_front.Dot(Vector3::AxisZ);//内積
		float angle = acosf(n);//アークコサイン
		if (m_front.x < 0) {
			angle *= -1;
		}
		m_rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(m_rot);

		//状態更新。
		UpdateState();
		//アニメーション選択。
		AnimationSelect();
	}
	else if(m_gameScene->GetGameState() == SampleScene::GameState::enPlaying)
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

			//グレネード用。仮です。
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

		//攻撃後の隙のタイマーを減らしていく
		m_attackCount--;
		//攻撃のクールタイムが終わると移動速度を戻す
		if (m_attackCount <= 0 && m_isBurst == false)
		{
			m_attackCount = 0;
			m_isAttacking = false;
			m_characterSpeed = 6.0;
		}
		//状態更新。
		UpdateState();
		//アニメーション選択。
		AnimationSelect();

		//カメラの移動
		Camera();

		//斥力・引力エフェクト			

		//磁力エフェクトを再生
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
		//ファイナルヒットカメラ。
		FinalHit();
		//状態更新。
		UpdateState();
		//アニメーション選択。
		AnimationSelect();
	}
	else if (m_gameScene->GetGameState() == SampleScene::GameState::enDraw)
	{
		//保持しているガレキを浮遊させる。
		HoldDebris();

		//保持している爆弾を浮遊させる。
		HoldBomb();

		//カメラの移動。
		Camera();

		//キャラを前に向かせる

		//カメラの前方向
		m_front = m_position - g_camera3D[m_playerNum]->GetPosition();
		m_front.y = 0.0f;
		m_front.Normalize();

		//カメラの右方向
		right = Cross(g_vec3AxisY, m_front);

		n = m_front.Dot(Vector3::AxisZ);//内積
		float angle = acosf(n);//アークコサイン
		if (m_front.x < 0) {
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

	//メビウスゲージの色を磁力から決定
	if (m_magPower == 1)
	{
		m_mobiusGauge->m_isRed = true;
	}
	else
	{
		m_mobiusGauge->m_isRed = false;
	}

	//メビウスゲージに現在の必殺技のチャージ量を渡す
	m_mobiusGauge->m_charge = m_charge;

	m_mobiusGauge->m_spCharge = m_specialAttackGauge;
}

//移動
void Player::Move()
{
	Vector3 oldPos = m_position;

	//カメラの前方向
	m_front = m_position - g_camera3D[m_playerNum]->GetPosition();
	m_front.y = 0.0f;
	m_front.Normalize();

	//カメラの右方向
	right = Cross(g_vec3AxisY, m_front);

	n = m_front.Dot(Vector3::AxisZ);//内積
	float angle = acosf(n);//アークコサイン
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

	//穴に落ちた時の処理
	if (m_position.y <= -750.0f) {
		m_LandingNum++;//落ちた回数
		Damage(100);

		//敵から最も遠いリスポーン地点に移動する。
		Vector3 respawnPoint = m_stageModel->GetRespawnPoint(m_enemy->m_position);
		m_position = respawnPoint;
		m_charaCon.SetPosition(m_position);
		m_skinModelRender->SetPosition(m_position);

		//リスポーンしたので落下加速用のカウントをリセット。
		m_fallLoop = 0;
	}

	//足音
	if (g_pad[m_playerNum]->GetLStickXF() || g_pad[m_playerNum]->GetLStickYF())
	{
		m_footstepsTimer++;
		
		if (m_animStatus == enStatus_Walk) {
			if (m_footstepsTimer >= 25)
			{
				prefab::CSoundSource* ssShoot = NewGO<prefab::CSoundSource>(0);;
				ssShoot->Init(L"Assets/sound/足音.wav");
				ssShoot->SetVolume(0.2f);
				ssShoot->Play(false);

				m_footstepsTimer = 0;
			}
		}
		if (m_animStatus == enStatus_Run) {
			if (m_footstepsTimer >= 17)
			{
				prefab::CSoundSource* ssShoot = NewGO<prefab::CSoundSource>(0);;
				ssShoot->Init(L"Assets/sound/足音.wav");
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
		ssShoot->Init(L"Assets/sound/落下音.wav");
		ssShoot->SetVolume(0.5f);
		ssShoot->Play(false);
	}	
}

//攻撃
void Player::Attack()
{
	if (g_pad[m_playerNum]->IsPress(enButtonRB1) && m_attackCount == 0)
	{
		//ガレキを一つでも持っているなら
		if (m_holdDebrisVector.empty() == false)
		{
			m_AttackNum++;//攻撃回数
			//音を鳴らす
			prefab::CSoundSource* ssShoot = NewGO<prefab::CSoundSource>(0);;
			ssShoot->Init(L"Assets/sound/シュート音.wav");
			ssShoot->SetVolume(0.5f);
			ssShoot->Play(false);

			//一番最初に保持したガレキを発射
			auto debris = m_holdDebrisVector.front();
			//保持したガレキを発射モードにする
			debris->m_debrisState = Debris::enBullet;

			//キャラクターのスピードを遅くする。
			m_characterSpeed = 0.5f;

			//攻撃中のフラグをオン
			m_isAttacking = true;

			//攻撃の隙の持続時間
			m_attackCount = 10;
			
			//この場所に向かって撃つ(GetShootPointの中での参照受け取り用)
			Vector3 crossPoint;
			//発射先を計算。
			bool hitFlag = GetShootPoint(crossPoint);

			//発射先に敵もしくはステージのモデルがある。
			if (hitFlag)
			{
				//照準の指す方向に飛ばす
				debris->m_moveDirection = crossPoint - debris->m_position;
				debris->m_moveDirection.Normalize();
			}
			else //ない。
			{
				debris->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				debris->m_moveDirection.y = 0.0f;
				debris->m_moveDirection.Normalize();
			}

			//発射したガレキを保持リストから削除
			m_holdDebrisVector.erase(m_holdDebrisVector.begin());

			//テキスト更新
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
	if (m_specialAttackGauge >= 100 && g_pad[m_playerNum]->IsTrigger(enButtonLB3))
	{
		m_SpecialAttackOn = true;		//アニメーションを必殺技にする。
		m_specialShotFlag = true;
	}
	if (m_specialShotFlag == true)
	{
		//発射する前に、チャージするようなエフェクト
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
			ssSPCharge->Init(L"Assets/sound/パワーチャージ.wav");
			ssSPCharge->Play(false);
		}

		m_specialShotCount += 1;

		//発射前にダメージを受けたらキャンセル
		if (m_HitOn == true) 
		{
			//ゲージも0に。
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
		//引力なら
		if (m_magPower == -1)
		{
			//m_SPFirstEffectBlue->Stop();
			//音を鳴らす
			prefab::CSoundSource* ssSPShot = NewGO<prefab::CSoundSource>(0);
			ssSPShot->Init(L"Assets/sound/引力弾発射.wav");
			ssSPShot->SetVolume(0.8f);
			ssSPShot->Play(false);

			//発射エフェクト
			m_SPEffect->Init(u"Assets/effect/引力弾発射.efk");
			m_SPEffect->SetPosition({
				m_position.x + m_front.x * 50.0f,
				m_position.y + 50.0f,
				m_position.z + m_front.z * 50.0f
				});
			m_SPEffect->Play();

			GravityBullet* gravityBullet = NewGO<GravityBullet>(0, "gravitybullet");
			gravityBullet->m_position = m_magPosition;
			gravityBullet->m_parent = this;

			//この場所に向かって撃つ(GetShootPointの中での参照受け取り用)
			Vector3 crossPoint;
			//発射先を計算。
			bool hitFlag = GetShootPoint(crossPoint);

			if (hitFlag)
			{
				//照準の指す方向に飛ばす
				gravityBullet->m_moveDirection = crossPoint - gravityBullet->m_position;
				gravityBullet->m_moveDirection.Normalize();
			}
			else
			{
				gravityBullet->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				gravityBullet->m_moveDirection.y = 0.0f;
				gravityBullet->m_moveDirection.Normalize();
			}

			//撃ったので必殺技ゲージを0に
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
		else//斥力なら
		{
			//m_SPFirstEffectRed->Stop();
			//弾を1発でも持ってる?
			if (m_holdDebrisVector.size() != 0)
			{
				//音を鳴らす
				prefab::CSoundSource* ssSPShot = NewGO<prefab::CSoundSource>(0);;
				ssSPShot->Init(L"Assets/sound/気弾1.wav");
				ssSPShot->Play(false);

				//発射エフェクト
				m_SPEffect->Init(u"Assets/effect/斥力弾発射.efk");
				m_SPEffect->SetPosition({ 
					m_position.x + m_front.x * 50.0f,
					m_position.y + 50.0f,
					m_position.z + m_front.z * 50.0f
					});
				m_SPEffect->Play();

				//この場所に向かって撃つ(GetShootPointの中での参照受け取り用)
				Vector3 crossPoint;
				//発射先を計算。
				bool hitFlag = GetShootPoint(crossPoint);

				if (hitFlag)
				{
					//照準の指す方向に飛ばす
					for (auto debris : m_holdDebrisVector)
					{
						m_AttackNum++;//攻撃回数
						debris->m_debrisState = Debris::enBullet;
						debris->m_moveDirection = crossPoint - debris->m_position;
						debris->m_moveDirection.Normalize();
					}
				}
				else
				{
					for (auto debris : m_holdDebrisVector)
					{
						m_AttackNum++;//攻撃回数
						debris->m_debrisState = Debris::enBullet;
						debris->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
						debris->m_moveDirection.y = 0.0f;
						debris->m_moveDirection.Normalize();
					}
				}

				m_holdDebrisVector.clear();

				//テキスト更新
				m_bulletNumFont->SetText(std::to_wstring(m_holdDebrisVector.size()));
				if (m_playerNum == 0)					
					m_bulletNumFont->SetPosition({ -170.0f, -270.0f });
				
				else
					m_bulletNumFont->SetPosition({ 60.0f, -270.0f });
				


				//撃ったので必殺技ゲージを0に
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
				//1発も弾持ってないから不発にする。エラー音?ゲージも消費しなくていい
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
		//爆弾を一つでも持っているなら
		if (m_holdBombVector.empty() == false)
		{
			m_AttackNum++;//攻撃回数
			//音を鳴らす
			prefab::CSoundSource* ssThrow = NewGO<prefab::CSoundSource>(0);;
			ssThrow->Init(L"Assets/sound/投げる音.wav");
			ssThrow->Play(false);

			//選択している爆弾を発射
			auto debris = m_holdBombVector.begin() + m_selectBombNo;
			//保持した爆弾を発射モードにする
			(*debris)->m_bombState = Bomb::enBullet;

			//キャラクターのスピードを遅くする。
			m_characterSpeed = 0.5f;

			//攻撃中のフラグをオン
			m_isAttacking = true;

			//攻撃の隙の持続時間
			m_attackCount = 60;

			Vector3 front = g_camera3D[m_playerNum]->GetForward();
			front.y = 0;
			front.Normalize();
			(*debris)->m_moveDirection = front;

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
	m_holdDebrisRotateDeg += 0.5f;
	if (m_holdDebrisRotateDeg >= 360.0f)
	{
		m_holdDebrisRotateDeg -= 360.0f;
	}

	//回転の中心を設定する。
	Vector3 centerOfRotation = m_position;
	centerOfRotation.y += 100.0f;

	//回転の中心をプレイヤーより後ろに。
	Vector3 cameraDir = g_camera3D[m_playerNum]->GetForward();
	cameraDir.y = 0;
	cameraDir.Normalize();
	centerOfRotation -= cameraDir * 30.0f;
	
	Vector3 toDebris = { 0.0f,50.0f,0.0f };

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
			if(i == 0)
			{ 
				debris->m_position = m_magPosition;
				debris->m_position += cameraDir * 100.0f;
			}
			else
			{
				//Y方向に50のベクトルを回転させて回転の中心点から伸ばす
				Quaternion debrisRot;

				//iの値によって順番に並べる
				debrisRot.SetRotationDeg(cameraDir, m_holdDebrisRotateDeg + degPerOneDebris * (i-1));

				//toDebris本体に回転を適用すると他の場所にも影響が出るのでコピーしてから回転を適用する。
				Vector3 tmp = toDebris;
				debrisRot.Apply(tmp);

				//回転の中心点から伸ばす
				debris->m_position = centerOfRotation + tmp;
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
			m_selectBombNo = 0;
		}
	}
	else if (g_pad[m_playerNum]->IsTrigger(enButtonLeft))
	{
		//選択している爆弾の番号がマイナスになっていたら
		if (--m_selectBombNo < 0)
		{
			//一周回ってコンテナのサイズ-1になる
			m_selectBombNo = m_holdBombVector.size() - 1;
		}
	}

	//回転の中心を設定する。
	Vector3 centerOfRotation = m_position;
	centerOfRotation.y += 100.0f;

	//回転の中心をプレイヤーより後ろに。
	Vector3 cameraDir = g_camera3D[m_playerNum]->GetForward();
	cameraDir.y = 0;
	cameraDir.Normalize();
	centerOfRotation -= cameraDir * 30.0f;

	Vector3 toDebris = { 0.0f,30.0f,0.0f };

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
			Vector3 tmp = toDebris;
			debrisRot.Apply(tmp);

			//回転の中心点から伸ばす
			debris->m_position = centerOfRotation + tmp;

			i++;
		}
	}
}

//磁力バーストを使用していない時の通常の動き
void Player::MagneticBehavior()
{
	//磁力の状態によって動きが分岐。
	switch (m_magPower)
	{
	case -1://引力

		break;
	case 1: //斥力

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
		m_BurstNum++;//バーストを使った回数
		//磁力ゲージを300消費。
		m_charge -= 300.0f;
		if (m_charge < 0)
		{
			m_charge = 0;
		}

		m_isBurst = true;
		m_burstCount = 60;

		//バースト音
		prefab::CSoundSource* ssBurst = NewGO<prefab::CSoundSource>(0);;

		switch (m_magPower)
		{
		case -1://引力
			//バースト音を再生
			ssBurst->Init(L"Assets/sound/引力バースト音.wav");
			ssBurst->SetVolume(1.5);
			ssBurst->Play(false);		
			//エフェクトを表示
			m_burstEffect->Init(u"Assets/effect/引力バースト.efk");
			m_burstEffect->Play();
			break;

		case 1://斥力
			//バースト音を再生
			ssBurst->Init(L"Assets/sound/斥力バースト音.wav");
			ssBurst->SetVolume(1.5);
			ssBurst->Play(false);
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
	m_characterSpeed = 0.0f;

	//敵までのベクトル
	Vector3 toEnemy = m_enemy->m_position - m_position;

	//敵に与える力
	Vector3 force = toEnemy;
	//縦軸は影響しない
	force.y = 0;
	force.Normalize();

	//磁力の状態によって動きが分岐。
	switch (m_magPower)
	{
	case -1://引力

		//引っ張るのでマイナスに
		force *= -10.0f;
		//近すぎる時は引っ張らない
		if (toEnemy.Length() > 100.0f && toEnemy.Length() < 750.0f)
		{
			m_enemy->m_charaCon.Execute(force, 1.0f);

			//敵の弾をまだ奪っていない時
			if (m_isSteal == false)
			{

				//敵の保持する弾が1発以上あるなら
				if (m_enemy->m_holdDebrisVector.size() != 0)
				{
					int i = 0;
					//持ってるガレキをドロップさせる
					//全部ではなく3つまでにしてみる。

					//敵の持っているガレキのリストを走査
					for (auto iterator = m_enemy->m_holdDebrisVector.begin(); iterator != m_enemy->m_holdDebrisVector.end(); iterator++)
					{
						m_StealNum++;//敵の弾を奪った回数
						//ドロップ状態にさせていく。すぐ吸うのでポップ状態ではない。
						(*iterator)->m_debrisState = Debris::enDrop;

						//カウントをすすめる
						i++;

						//3になったら3つ吸ったのでブレイク。
						if (i == 3)
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

					//もう敵の弾を奪ったのでフラグ変更
					m_isSteal = true;
				}

				//敵が1つでも爆弾を持っていれば
				if (m_enemy->m_holdBombVector.size() != 0)
				{
					//爆弾をドロップさせる。
					(*m_enemy->m_holdBombVector.begin())->m_bombState = Bomb::enDrop;

					//ドロップさせた爆弾を相手のコンテナから削除
					m_enemy->m_holdBombVector.erase(m_enemy->m_holdBombVector.begin());

					//もう敵の弾を奪ったのでフラグ変更
					m_isSteal = true;
				}

				if (m_isSteal == true)
				{
					//リスクのある行動を成功させたので必殺技ゲージをプラス。
					ChargeSpecialAttackGauge(25);
				}
			}
		}

		break;
	case 1: //斥力

		force *= 10.0f;
		if (toEnemy.Length() < 750.0f)
		{
			m_enemy->m_charaCon.Execute(force, 1.0f);
		}

		break;
	default:
		MessageBoxA(nullptr, "磁力の状態が異常です。\n", "エラー", MB_OK);
		break;
	}

	m_burstCount--;
	//バーストカウントが0になるとバースト終了。
	if (m_burstCount <= 0)
	{
		m_isBurst = false;
		//敵の弾を奪ったフラグをリセット。
		m_isSteal = false;

		//スピードも戻す
		m_characterSpeed = 6.0f;
	}
}

void Player::ChangeMagnetPower()
{
	//磁力ゲージの自然減少
	m_charge -= 2.0f;
	if (m_charge < 0)
	{
		m_charge = 0;
	}

	//磁力ゲージが0以下かつ、バースト中や必殺技発動中でなければ
	if (m_charge <= 0 && 
		m_isBurst == false &&
		m_specialShotFlag == false	
		)	{
		//磁力の状態が-1か1なので、-1を掛ければ反転する。
		//普通にswitchしてもいいかも。
		m_magPower *= -1;

		if (m_isAttacking == true)
		{
			m_isAttacking = false;
			m_characterSpeed = 6.0f;
		}

		//チャージを回復。
		m_charge = 1000.0f;
	
		//磁力エフェクト変更
		if (m_magPower == 1) {
			m_magEffect[0]->Init(u"Assets/effect/斥力.efk");
			m_magEffect[1]->Init(u"Assets/effect/斥力.efk");
		}
		else if (m_magPower == -1) {
			m_magEffect[0]->Init(u"Assets/effect/引力.efk");
			m_magEffect[1]->Init(u"Assets/effect/引力.efk");
		}
	}
}

//カメラの移動
void Player::Camera()
{
	//カメラ関連
	if (g_pad[m_playerNum]->IsTrigger(enButtonRB3))
	{
		m_isLock = !m_isLock;//ロック切り替え
	}

	//プレイヤーの頭上90を基準として、そこからカメラに伸びるベクトルを回転させる。
	Vector3 targetPos = m_position;
	targetPos.y += 90.0f;

	if (m_isLock)
	{
		//ロックしている時は、敵の中心からプレイヤーの頭上へのベクトルの延長線上にカメラがある。
		Vector3 toEnemy = m_enemy->m_magPosition - targetPos;
		toEnemy.Normalize();
		m_toCameraDir = toEnemy * -1.0f;

		//コントローラーの入力でカメラの向きをちょっとずらせる
		qRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * 5.0f);
		qRotY.Apply(m_toCameraDir);

		Quaternion qRotX;
		Vector3 right = g_camera3D[m_playerNum]->GetRight();
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -5.0f);

		qRotX.Apply(m_toCameraDir);
	}
	else
	{
		//ロックされていない時はカメラへのベクトルを回転させる。
		qRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * 1.5);
		qRotY.Apply(m_toCameraDir);

		Quaternion qRotX;
		Vector3 right = g_camera3D[m_playerNum]->GetRight();
		qRotX.SetRotationDeg(right, g_pad[m_playerNum]->GetRStickYF() * -1.5);

		//回転先の角度を確認して上すぎる時または下すぎる時は回転させない。
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

	//本来のカメラ位置
	cameraPos = targetPos + m_toCameraDir * 125.0f;

	//バネカメラ的なもの　カメラのターゲット位置から本来のカメラ位置へのベクトルがステージに衝突しているか判定
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(targetPos, cameraPos, crossPoint);

	if (isHit == false)
	{
		//当たっていないなら本来のカメラ位置
		g_camera3D[m_playerNum]->SetPosition(cameraPos);
	}
	else
	{
		//当たっているなら当たった位置にカメラを移動
		g_camera3D[m_playerNum]->SetPosition(crossPoint);
	}
	g_camera3D[m_playerNum]->SetTarget(targetPos);

	//ライト
	Vector3 PlayerWaistPos = m_position;
	PlayerWaistPos.y += 40;
	Vector3 Direction = PlayerWaistPos - cameraPos;
	Direction.Normalize();
	m_spotLight->SetDirection(Direction);
	m_spotLight->SetPosition(cameraPos);
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
	squarePos1.y += 250.0f;
	squarePos1 += side * 250.0f;

	//四角形の左下
	Vector3 squarePos2 = m_position;
	squarePos2.y -= 250.0f;
	squarePos2 += side * 250.0f;

	//四角形の右上
	Vector3 squarePos3 = m_position;
	squarePos3.y += 250.0f;
	squarePos3 -= side * 250.0f;

	//四角形の右下
	Vector3 squarePos4 = m_position;
	squarePos4.y -= 250.0f;
	squarePos4 -= side * 250.0f;

	//1つ目の三角形の頂点は左上、左下、右上。
	m_triCollider[0].SetVertex(squarePos1, squarePos2, squarePos3);
	//2つ目の三角形の頂点は右下、右上、左下。
	m_triCollider[1].SetVertex(squarePos4, squarePos3, squarePos2);

	//カプセルの当たり判定をつくる。
	Vector3 legPos = m_position;
	Vector3 headPos = m_position;
	headPos.y += 100.0f;

	//カプセルの始点と終点を更新する。(半径はStart関数で最初だけ決めてもよい)
	m_collider.SetStartPoint(legPos);
	m_collider.SetEndPoint(headPos);
	m_collider.SetRadius(20.0f);
}

//自分の体力にダメージを与える
void Player::Damage(int damage)
{	
	m_ReceivedDamage += damage;//受けたダメージ
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

		m_gameScene->SetGameState(SampleScene::GameState::enResult);
	}

	//与えたダメージ量を相手に表示する
	DamageDisplay* damagedisplay = NewGO<DamageDisplay>(0, "damagedisplay");
	damagedisplay->m_damagePos = m_position;
	damagedisplay->m_enemyNum = m_enemy->m_playerNum;
	damagedisplay->m_damage = damage;

	//HPバー更新
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPlaying)
	{
		if (m_playerNum == 0) {
			m_HPBarDarkSpriteRender->SetPosition({ -9.0f + m_hp / 1000.0f * 299, 325.0f,0.0f });
		}
		else if (m_playerNum == 1) {
			m_HPBarDarkSpriteRender->SetPosition({ 9.0f + m_hp / 1000.0f * -299, 325.0f,0.0f });
		}
	}

	//ダメージエフェクト
	m_hitEffect->SetPosition({ m_position.x, m_position.y + 50, m_position.z });

	//カメラの前方向
	m_damegeEffectFront.y = 0.0f;
	m_damegeEffectFront.Normalize();

	float innerProduct = m_damegeEffectFront.Dot(Vector3::AxisZ); //内積
	float angle = acosf(innerProduct);//アークコサイン
	/*if (m_damegeEffectFront.x < 0) {
		angle *= -1;
	}*/
	Quaternion rot;
	rot.SetRotation(Vector3::AxisY, angle);

	m_hitEffect->SetRotation(rot);
	m_hitEffect->Play();
	//ここまでエフェクト処理
}

//必殺技ゲージをチャージする。
void Player::ChargeSpecialAttackGauge(int charge)
{
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
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/きらーん.wav");
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
		m_SaveSP += charge;//溜まった必殺技ポイント

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

//勝利した時
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

//敗北した時
void Player::Lose()
{	
	m_Lose = true;
	/*m_resultSpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_resultSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);
	m_resultSpriteRender->Init("Assets/Image/Haiboku.dds", 256, 256);*/
	//m_loserNum = m_playerNum;


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

	m_LastFront = m_front;
}



//攻撃状態に切り替えできたら切り替える。
void Player::TryChangeStatusAttack()
{
	if (m_magPower == 1 && m_holdDebrisVector.empty() == false && g_pad[m_playerNum]->IsPress(enButtonRB1)) {
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
	if (m_moveSpeed.LengthSq() > 5.0f) {
		m_animStatus = enStatus_Run;
	}
}

//歩き状態に切り替えできたら切り替える。
void Player::TryChangeStatusWalk()
{
	if (m_moveSpeed.LengthSq() <= 5.0f && m_moveSpeed.LengthSq() > 0.0f) {
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
	if (m_moveSpeed.LengthSq() <= 0.001f) {
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

//死亡状態に切り替える
void Player::TryChangeStatusDeath()
{
	if (m_Lose==true)
	{
		m_animStatus = enStatus_Death;				
	}
}

//勝利状態に切り替える
void Player::TryChangeStatusWin()
{
	if(m_WinAnimOn==true)
	m_animStatus = enStatus_Winner;
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
		//toPosを回す。
		Quaternion qRotY;
		//Y軸回りにちょっとだけ回転するクォータニオンをつくる。
		m_addY += 0.01;
		qRotY.SetRotation(Vector3::AxisY, m_addY);
		//クォータニオンを使ってtoPosを回す。
		qRotY.Apply(toPos);
		m_cameraPos = toPos;
		g_camera3D[m_playerNum]->SetTarget(m_targetPos);
	}
	else
	{
		Vector3 PlayerPos = m_position;
		PlayerPos.y = m_position.y + 90.0f;//プレイヤーの頭の位置
		
		
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


	//キャラを初期方向に向かせる

	//キャラの前方向はm_characterDirection;

	n = m_characterDirection.Dot(Vector3::AxisZ);//内積
	float angle = acosf(n);//アークコサイン
	if (m_characterDirection.x < 0) {
		angle *= -1;
	}
	m_rot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(m_rot);
}

void Player::FinalHit()//決着がついたときのカメラ
{	
	m_gameScene->ssBGM->Stop();
	if (m_FirstTime == true) {//一回だけ流れるループ
		//画面分割を終了
		GameObjectManager::GetInstance()->Set2ScreenMode(false);
		//画面に出ているやつを消す
		//HPバー、画面分割線、メビウスゲージを消す
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
	Vector3 LastRight;
	Vector3 winnerFrontPos;
	Vector3 winnerHeadPos;
	if (m_playerNum == m_loserNum)//敗者を写す
	{	
		LastRight = Cross(g_vec3AxisY, m_LastFront);//最後に向いていた向きの右ベクトル
		Vector3 targetPos = m_position;//ターゲットポジション
		m_cameraPos = targetPos;//カメラのポジション
		targetPos.y += 20;//少し上に設定する
		targetPos += m_LastFront * -30;//キャラの少し後ろに伸ばす	
		m_cameraPos.y += 100;//キャラより少し上くらい
		//ループの値に合わせてステータスを変える
		if (m_LoseCameraLoop > 0)
		{
			m_LoseCameraFlag = false;//アニメーションの再生速度を変える
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
			m_LoseCameraFlag = true;//アニメーションの再生速度を戻す
		}
		if (m_LoseCameraLoop > 200)
		{
			m_LastCameraStatus = 4;
		}		
		
		switch (m_LastCameraStatus)
		{
		case 0://右からのカメラ
			m_cameraPos += LastRight * 200;//右
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 1://左からのカメラ
			m_cameraPos += LastRight * -200;//左
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 2://前からのカメラ
			m_cameraPos += m_LastFront * 200;//正面
			g_camera3D[0]->SetTarget(targetPos);
			break;
		case 3://自分を写しながら敵を向いたカメラ
			winnerHeadPos = m_enemy->m_position;
			winnerHeadPos.y += 50;//勝者の頭の位置
			//敵のちょっと前と自分を結んだ線を正規化して後ろに少し伸ばす
			m_winnerVec=(winnerHeadPos + m_enemy->m_LastFront * 200) - m_position;
			m_winnerVec.Normalize();
			m_cameraPos += m_winnerVec*-200;//後ろ
			m_enemyWaistPos = m_enemy->m_position;//敵の腰の位置
			m_enemyWaistPos.y += 20;
			g_camera3D[0]->SetTarget(m_enemyWaistPos);
			break;
		case 4://敵の前まで移動する
			//カメラを敵の前まで移動させる				
			winnerFrontPos = (m_enemy->m_position + m_enemy->m_LastFront * 150 ) - (m_position + m_winnerVec * -200);			
			if (m_coef < 1.0f)//ベクトルに掛ける値
				m_coef += 0.01f;
			if (m_coef > 0.5f)//カメラが半分の距離まで行くとアニメーションを再生する
				m_enemy->m_WinAnimOn = true;
			m_cameraPos += (winnerFrontPos * (pow(m_coef,1.5)) );//+ (winnerVec * 200)
			m_cameraPos += m_winnerVec * -200;//後ろ
			
			g_camera3D[0]->SetTarget(m_enemyWaistPos);
			break;
		default:
			break;
		}

		//SE
		if (m_LoseCameraLoop == 0)
		{
			//音を再生
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/K.O..wav");
			ss->SetVolume(1.5f);
			ss->Play(false);
		}
		if (m_LoseCameraLoop == 50)
		{
			//音を再生
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/K.O..wav");
			ss->SetVolume(1.5f);
			ss->Play(false);
		}
		if (m_LoseCameraLoop == 100)
		{
			//音を再生
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/K.O..wav");
			ss->SetVolume(1.5f);
			ss->Play(false);
		}
		if (m_LoseCameraLoop == 250)
		{
			//ジングルを再生
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
			//命中率を計算(0で割るとバグるため＋１をしておく)
			m_HitRate = ((m_enemy->m_TakeAttackNum + 1) / (m_AttackNum + 1)) * 100;

			//体力、チャージ、現在の自分の磁力の状態の表示

		}
		if (m_playerNum == 1)
		{
			//命中率を計算(0で割るとバグるため＋１をしておく)
			m_HitRate = ((m_enemy->m_TakeAttackNum + 1) / (m_AttackNum + 1)) * 100;

		}
	}
}
