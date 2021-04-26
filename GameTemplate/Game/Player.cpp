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

	//モデルの初期化
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/Mage.tkm", "Assets/modelData/Mage.tks", animationClips, enAnimationClip_num);
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetScale(m_scale);

	//キャラコンの初期化
	m_charaCon.Init(10.0f, 50.0f, m_position);

	//ステータス表示の初期化
	m_statusFontRender = NewGO<prefab::CFontRender>(1);
	m_statusFontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)m_playerNum);
	m_statusFontRender->SetPosition({ -625.0f, 350.0f });

	//照準表示の初期化
	m_crosshairRender = NewGO<prefab::CSpriteRender>(1);
	m_crosshairRender->SetDrawScreen(static_cast<prefab::CSpriteRender::DrawScreen>(m_playerNum));
	m_crosshairRender->Init("Assets/Image/Sight.dds", 32, 32);

	//ステージのクラスを取得
	m_stageModel = FindGO<BackGround>("background");

	return true;
}

void Player::Update()
{
	//プレイヤー番号が与えられていなければ何もしない
	if (m_playerNum == -1)
	{
		return;
	}

	//磁力の変化
	ChangeMagnetPower();

	//体力等ステータスのテキストを表示(後に画像にする。)
	DisplayStatus();

	//座標に応じて三角形の当たり判定の場所をセット。
	Collision();

	//移動
	Move();

	//保持しているガレキの位置を制御する
	HoldDebris();

	//バーストを使用している?
	if (m_isBurst == true)
	{
		MagneticBurst();
	}
	else
	{
		MagneticBehavior();
	}
	
	//状態更新。
	UpdateState();
	//アニメーション選択。
	AnimationSelect();

	//カメラの移動
	Camera();

}

//体力、メビウスゲージの表示
void Player::DisplayStatus()
{
	//体力、チャージ、現在の自分の磁力の状態の表示
	std::wstring powerText;
	switch (m_magPower)
	{
	case -1:
		powerText = L"引力";
		m_statusFontRender->SetColor({ 0.0f,0.0f,1.0f,1.0f });
		break;
	case 1:
		powerText = L"斥力";
		m_statusFontRender->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		break;
	default:
		powerText = L"error";
	}

	wchar_t charge[256];
	swprintf_s(charge, L"%.1f", m_charge / 10.0f);

	m_statusFontRender->SetText(L"HP:" + std::to_wstring(m_hp)
		+ L"\nCharge:" + charge
		+ L"%\n\n\n\n\n\n\n\n\n磁力:" + powerText
	);
}

//移動
void Player::Move()
{
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

//攻撃
void Player::Attack()
{
	m_attackCount--;
	//攻撃のクールタイムが終わると移動速度を戻す
	if (m_attackCount <= 0)
	{
		m_attackCount = 0;

		m_isAttacking = false;

		m_characterSpeed = 6.0f;
	}

	if (g_pad[m_playerNum]->IsPress(enButtonRB1) && m_attackCount == 0)
	{
		//ガレキを一つでも持っているなら
		if (m_holdDebrisVector.empty() == false)
		{
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
			
			//発射先の決定
			if (m_isLock)
			{
				Vector3 dir = m_enemy->m_magPosition - debris->m_position;
				dir.Normalize();
				debris->m_moveDirection = dir;
			}
			else
			{
				//ロックオンしていないので、発射先を決める必要がある。
				//カメラの位置から向いている方向に飛ばすレイを作成。
				//キャラクターの位置からじゃないことに注意。
				//レイの向き
				Vector3 testRayDir = g_camera3D[m_playerNum]->GetForward();
				//レイの始点
				Vector3 testRayStart = g_camera3D[m_playerNum]->GetPosition();
				//レイの始点と向きから求めたレイの終点(10000以上の距離狙うことはないと思うので距離は10000に設定)
				Vector3 testRayEnd = testRayStart + testRayDir * 10000.0f;

				//交点(キャラクターの位置からこの位置に向かって発射されることになる)
				Vector3 crossPoint;

				//交差したかフラグ。
				bool hitFlag = false;

				//まず敵キャラクター付近の板ポリ当たり判定を検索する。
				for (auto tricollider : m_enemy->m_triCollider)
				{
					hitFlag = tricollider.isHit(testRayStart, testRayEnd, crossPoint);
					if (hitFlag == true)
					{
						//1回でもヒットしていたらbreak
						break;
					}
				}

				//敵キャラクター付近にヒットしなかったらステージのモデルを検索する。
				if (hitFlag == false)
				{
					hitFlag = m_stageModel->isLineHitModel(testRayStart, testRayEnd, crossPoint);
				}

				if (hitFlag)
				{
					//照準の指す方向に飛ばす
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

			//発射したガレキを保持リストから削除
			m_holdDebrisVector.erase(m_holdDebrisVector.begin());
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
		//磁力ゲージを300消費。
		m_charge -= 300.0f;
		if (m_charge < 0)
		{
			m_charge = 0;
		}

		m_isBurst = true;
		m_burstCount = 60;
	}
}

//磁力バースト
void Player::MagneticBurst()
{
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

			//敵の弾をまだ奪っていないかつ、敵の保持する弾が1発以上ある時
			if (m_isSteal == false && m_enemy->m_holdDebrisVector.size() != 0)
			{
				int i = 0;
				//持ってるガレキをドロップさせる
				//全部ではなく3つまでにしてみる。

				//敵の持っているガレキのリストを走査
				for (auto iterator = m_enemy->m_holdDebrisVector.begin();iterator != m_enemy->m_holdDebrisVector.end();iterator++)
				{
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

				//もう敵の弾を奪ったのでフラグ変更
				m_isSteal = true;
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
	m_charge -= 1.0f;
	if (m_charge < 0)
	{
		m_charge = 0;
	}

	//磁力ゲージが0以下かつ、バースト中でなければ
	if (m_charge <= 0 && m_isBurst == false)
	{
		//磁力の状態が-1か1なので、-1を掛ければ反転する。
		//普通にswitchしてもいいかも。
		m_magPower *= -1;

		if (m_isAttacking == true)
		{
			m_isAttacking = false;
			m_characterSpeed = 6.0f;
		}

		//チャージを回復。
		m_charge = 1000;
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
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;

		//体力の更新。
		DisplayStatus();

		Lose();

		m_enemy->Win();
	}


	//与えたダメージ量を相手に表示する
	DamageDisplay* damagedisplay = NewGO<DamageDisplay>(0, "damagedisplay");
	damagedisplay->m_damagePos = m_position;
	damagedisplay->m_enemyNum = m_enemy->m_playerNum;
	damagedisplay->m_damage = damage;
}

//勝利した時
void Player::Win()
{
	m_resultSpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_resultSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);
	m_resultSpriteRender->Init("Assets/Image/Syouri.dds", 256, 256);
}

//敗北した時
void Player::Lose()
{
	m_resultSpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_resultSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)m_playerNum);
	m_resultSpriteRender->Init("Assets/Image/Haiboku.dds", 256, 256);
}

//攻撃状態に切り替えできたら切り替える。
void Player::TryChangeStatusAttack()
{
	if (g_pad[m_playerNum]->IsPress(enButtonX) || g_pad[m_playerNum]->IsPress(enButtonRB1)) {
		m_animStatus = enStatus_Attack;
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
	}
}

//待機状態に切り替えできたら切り替える。
void Player::TryChangeStatusIdle()
{
	if (m_moveSpeed.LengthSq() <= 0.001f) {
		m_animStatus = enStatus_Idle;
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