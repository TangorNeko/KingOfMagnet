#include "stdafx.h"
#include "Character_base.h"
#include "Bullet.h"
#include "ChargeShot.h"
#include "BackGround.h"
#include <string>
#include"Mage.h"


Character_base::~Character_base()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_pointLight);
	DeleteGO(m_fontRender);
	DeleteGO(m_spriteRender);
}

bool Mage::Start()
{
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

	//マシンガン用
	animationClips[enAnimationClip_Gun_Idle].Load("Assets/animData/Gun_Idle.tka");
	animationClips[enAnimationClip_Gun_Idle].SetLoopFlag(true);	//ループモーションにする。
	animationClips[enAnimationClip_Gun_Run].Load("Assets/animData/Gun_Run.tka");
	animationClips[enAnimationClip_Gun_Run].SetLoopFlag(true);	//ループモーションにする。
	animationClips[enAnimationClip_Gun_Walk].Load("Assets/animData/Gun_Walk.tka");
	animationClips[enAnimationClip_Gun_Walk].SetLoopFlag(true);	//ループモーションにする。

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Mage.tkm", "Assets/modelData/Mage.tks",animationClips,enAnimationClip_num);

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
	m_weaponModel->Init("Assets/modelData/Mage_Weapon.tkm");

	m_stageModel = FindGO<BackGround>("background");
	
	
	return true;
}
void Mage::Update()
{
	//m_skinModelRender->PlayAnimation(enAnimationClip_Run);
	
	if (g_pad[0]->IsTrigger(enButtonStart) || g_pad[1]->IsTrigger(enButtonStart))
	{
		m_isSceneStop = !m_isSceneStop;
	}
	//プレイヤー番号が与えられていなければ何もしない
	if (m_playerNum == -1)
	{
		return;
	}

	if (m_isSceneStop == false)
	{

		//磁力の変化
		ChangeMagnetPower();

		//座標に応じて三角形の当たり判定の場所をセット。
		Collision();

		//体力等ステータスのテキストを表示(後に画像にする。)
		DisplayStatus();

		//移動関連

		//カメラの前方向
		front = m_position - g_camera3D[m_playerNum]->GetPosition();
		front.y = 0.0f;
		front.Normalize();

		//カメラの右方向
		right = Cross(g_vec3AxisY, front);

		n = front.Dot(Vector3::AxisZ);//内積
		float angle = acosf(n);//アークコサイン
		if (front.x < 0) {
			angle *= -1;
		}
		rot.SetRotation(Vector3::AxisY, angle);
		m_skinModelRender->SetRotation(rot);		
		m_moveSpeed = front * g_pad[m_playerNum]->GetLStickYF() * m_Speed + right * g_pad[m_playerNum]->GetLStickXF() * m_Speed + m_Yspeed0 + m_Yspeed1;
		if (m_charaCon.IsOnGround() == false)
		{
			m_loop++;
			if (m_moveSpeed.y > -0.001)
			{
				m_moveSpeed.y -= 0.005f * (m_loop * m_loop);
			}
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

		//移動アクション
		MoveAction();

		//攻撃関連
		//通常攻撃
		NormalAttack();

		//チャージ
		Charge();

		//固有攻撃
		SpecialAttack();
		
		//移動関連2
		
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f);
		m_magPosition = m_position;
		m_magPosition.y += 50.0f;
		m_skinModelRender->SetPosition(m_position);

		//キャラクターにかかる磁力の影響
		PlayerMagneticMove();
		//手のボーンのワールド行列を取得
		Matrix handmatrix = m_skinModelRender->GetWorldMatrixFromBoneName(L"B_R_Hand");
		//手のボーンの行列
		//handmatrix.Multiply(mScale, handmatrix);

		m_weaponModel->SetMatrix(handmatrix);

		//マシンガンを持ったとき
		HaveMachinegun();
		
		//状態更新。
		UpdateState();
		//アニメーション選択。
		AnimationSelect();

		//カメラ関連
		Camera();
	}
}
void Mage::DisplayStatus()
{
	//体力、チャージ、現在の自分の磁力の状態の表示
	std::wstring powerText;
	switch (m_magPower)
	{
	case -2:
		powerText = L"引力Lv2";
		m_fontRender->SetColor({ 0.0f,0.0f,1.0f,1.0f });
		break;
	case -1:
		powerText = L"引力Lv1";
		m_fontRender->SetColor({ 0.0f,0.5f,1.0f,1.0f });
		break;
	case 0:
		powerText = L"磁力なし";
		m_fontRender->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		break;
	case 1:
		powerText = L"斥力Lv1";
		m_fontRender->SetColor({ 1.0f,0.4f,0.4f,1.0f });
		break;
	case 2:
		powerText = L"斥力Lv2";
		m_fontRender->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		break;
	default:
		powerText = L"error";
	}

	wchar_t charge[256];
	swprintf_s(charge, L"%.1f", m_charge / 10.0f);

	m_fontRender->SetText(L"HP:" + std::to_wstring(m_hp)
		+ L"\nCharge:" + charge
		+ L"%\n\n\n\n\n\n\n磁力:" + powerText
		+ L"\n磁力の変化まで:" + std::to_wstring((600 - m_timer) / 60)
		+ L"\n移動アクション:" + std::to_wstring(m_moveActionCount / 60));
}
void Mage::MoveAction()
{
	//移動アクション
	if (g_pad[m_playerNum]->IsTrigger(enButtonA) && m_moveActionCount == 0 && !(g_pad[m_playerNum]->IsPress(enButtonLB2)))
	{
		m_skinModelRender->SetScale({ 0.0f,0.0f,0.0f });
		m_moveActionCount = 600;
		m_moveSpeed += m_characterDirection * 500.0f;
	}
	else
	{
		if (--m_moveActionCount <= 550)
		{
			m_skinModelRender->SetScale({ Scale });
		}

		if (m_moveActionCount < 0)
		{
			m_moveActionCount = 0;
		}
	}
}

void Mage::NormalAttack()
{
	//通常攻撃
	if (--m_normalAttackCount < 0)
	{
		m_normalAttackCount = 0;
	}

	if (g_pad[m_playerNum]->IsTrigger(enButtonRB1) && m_normalAttackCount == 0)
	{
		if (m_isLock)
		{
			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_CharaNum = 1;
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
			Vector3 testRayDir = g_camera3D[m_playerNum]->GetForward();
			Vector3 testRayStart = g_camera3D[m_playerNum]->GetPosition();
			Vector3 testRayEnd = testRayStart + testRayDir * 10000.0f;

			Vector3 crossPoint;

			bool hitFlag = m_stageModel->isLineHitModel(testRayStart, testRayEnd, crossPoint);

			Bullet* bullet = NewGO<Bullet>(0, "bullet");
			bullet->m_CharaNum = 1;
			bullet->m_position = m_position;
			bullet->m_position.y += 50;

			if (hitFlag)
			{
				//照準の指す方向に飛ばす
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
void Mage::Charge()
{
	//チャージ
	if (g_pad[m_playerNum]->IsPress(enButtonLB2) && g_pad[m_playerNum]->IsPress(enButtonRB2))
	{
		
		if (m_Psycho_on == false)
		{
			m_Psycho_on = true;
			psychokinesis = NewGO<Psychokinesis>(0,"psychokinesis");
			psychokinesis->m_playerNum = m_playerNum;
			psychokinesis->m_rot = rot;
		}
		else
		{
			psychokinesis->m_charge = m_charge;
			psychokinesis->m_chargelevel = m_chargelevel;
		}
		m_charge += 10.0f - m_magPower * 2.5f;
		if (m_charge < 333.3f) {
			m_chargelevel = 1;
		}
		else if (m_charge < 666.6) {
			m_chargelevel = 2;
			if(psychokinesis->m_level2 == false)
			psychokinesis->m_level2 = true;
		}
		else if (m_charge < 1000.0f) {
			m_chargelevel = 3;
			if (psychokinesis->m_level3 == false)
				psychokinesis->m_level3 = true;
		}
		else if (m_charge >= 1000.0f)
		{
			m_chargelevel = 4;
			m_charge = 1000.0f;
		}
		//音の再生
		//音がずっと出る
		if (m_chargeSound == nullptr)
		{
			m_chargeSound = NewGO<prefab::CSoundSource>(0);
			m_chargeSound->Init(L"Assets/sound/ビーム砲チャージ.wav");
			m_chargeSoundVolume = 0.6f;
			m_chargeSound->SetVolume(m_chargeSoundVolume);
			m_chargeSound->Play(true);
		}
		else
		{
			m_chargeSoundVolume += 0.1f;
			if (m_chargeSoundVolume >= 0.6f)
			{
				m_chargeSoundVolume = 0.6f;
			}
			m_chargeSound->SetVolume(m_chargeSoundVolume);
		}
		//速度０
		m_Speed = 1.0f;
	}
	else
	{
		//チャージをやめた時の音がゆるやかに減衰する。
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
		//速度０
		m_Speed = 4.0f;
	}

	//チャージ確認用
	if (m_charge < 1000.0f) {
		m_pointLight->SetColor({ 0.0f,m_charge / 100,0.0f });
	}
	else
	{
		m_pointLight->SetColor({ 10.0f,0.0f,0.0f });
	}
	m_pointLight->SetPosition(m_position);
	//サイコキネシスの岩にプレイヤーの座標を送る
	if (m_Psycho_on == true)
	{
		psychokinesis->m_playerpos = m_position;
		//psychokinesis->m_Dir = cameraPos;
	}

}
void Mage::SpecialAttack()
{
	//固有攻撃
	if (g_pad[m_playerNum]->IsPress(enButtonX) && m_Psycho_on == true)
	{
		if (m_isLock)
		{			
			Vector3 dir = m_enemy->m_magPosition - m_magPosition;
			dir.Normalize();
			psychokinesis->m_moveDirection = dir;
			psychokinesis->m_velocity = 50.0f;
			psychokinesis->m_parentNo = m_playerNum;
			m_charge = 0;
		}
		else
		{
			Vector3 testRayDir = g_camera3D[m_playerNum]->GetForward();
			Vector3 testRayStart = g_camera3D[m_playerNum]->GetPosition();
			Vector3 testRayEnd = testRayStart + testRayDir * 10000.0f;

			Vector3 crossPoint;

			bool hitFlag = m_stageModel->isLineHitModel(testRayStart, testRayEnd, crossPoint);

			
			if(hitFlag)
			{ 
				psychokinesis->m_moveDirection = crossPoint - m_magPosition;
				psychokinesis->m_moveDirection.Normalize();
			}
			else
			{
				psychokinesis->m_moveDirection = m_position - g_camera3D[m_playerNum]->GetPosition();
				psychokinesis->m_moveDirection.y = 0.0f;
				psychokinesis->m_moveDirection.Normalize();
			}
			psychokinesis->m_velocity = 50.0f;
			psychokinesis->m_parentNo = m_playerNum;
		}
		m_charge = 0;
		
	}
}

void Mage::TryChangeStatusAttack()
{
	if (g_pad[m_playerNum]->IsPress(enButtonX)|| g_pad[m_playerNum]->IsPress(enButtonRB1)) {
		status = enStatus_Attack;
	}
}
void Mage::TryChangeStatusRun()
{
	if (m_moveSpeed.LengthSq() > 5.0f) {
		status = enStatus_Run;
	}
}
void Mage::TryChangeStatusWalk()
{
	if (m_moveSpeed.LengthSq() <= 5.0f&& m_moveSpeed.LengthSq() > 0.0f) {
		status = enStatus_Walk;
	}
}
void Mage::TryChangeStatusIdle()
{
	if (m_moveSpeed.LengthSq() <= 0.001f) {
		status = enStatus_Idle;
	}
}
void Mage::TryChangeStatusFall()
{
	if (m_charaCon.IsOnGround() == false) 
	{
		status = enStatus_Fall;		
	}	
}
void Mage::UpdateState()
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

void Mage::AnimationSelect()
{
	m_skinModelRender->m_animation_speed = 1.0;
	
	if (m_MachinegunHave == false) {
		switch (status) {
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
		case enStatus_Fall:
			m_skinModelRender->PlayAnimation(enAnimationClip_Fall);

		}
	}	
}
void Mage::HaveMachinegun()
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
			m_Machinegun_loopcount = 0;
			m_Machinegun_deletetime = 500;
			m_Machinegun_bulletNum = 100;
			m_weaponModel->Init("Assets/modelData/Mage_Weapon.tkm");
		}
	}
}
