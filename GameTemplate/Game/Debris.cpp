#include "stdafx.h"
#include "Debris.h"

#include "Player.h"
#include "BackGround.h"
#include "SampleScene.h"

#include "Explosion.h"

Debris::~Debris()
{
	DeleteGO(m_skinModelRender);
	
	//ゲームシーンの弾の数を減らす。
	if (m_gameScene != nullptr)
	{
		m_gameScene->BulletDecrease();
	}
}

bool Debris::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->SetShadowCasterFlag(true);
	int randam;
	//ガレキの形状で読み込むモデルを分岐
	switch (m_debrisShape)
	{
	case enStone:
		//石のモデル
		randam = rand() % 3;
		if(randam == 0)
		m_skinModelRender->Init("Assets/modelData/pipe.tkm");
		if (randam == 1)
		m_skinModelRender->Init("Assets/modelData/gear.tkm");
		if (randam == 2)
		m_skinModelRender->Init("Assets/modelData/neji.tkm");
		m_skinModelRender->SetScale({ 1.0f,1.0f,1.0f });
		break;
	case enSword:
		//剣のモデル
		m_skinModelRender->Init("Assets/modelData/KnightBullet.tkm");
		m_skinModelRender->SetScale({ 0.5f, 0.5f, 0.5f });
		break;
	case enSpecialCharger:
		m_skinModelRender->Init("Assets/modelData/Gravity.tkm");
		m_skinModelRender->SetScale({ 0.1f, 0.1f, 0.1f });
		break;
	default:
		MessageBoxA(nullptr, "存在しないガレキの形状です。\n", "エラー", MB_OK);
		break;
	}

	//ステージとの当たり判定用
	m_stageModel = FindGO<BackGround>("background");

	m_gameScene = FindGO<SampleScene>("gamescene");

	return true;
}

void Debris::Update()
{
	//ポーズ中ならスキップ。
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPause)
	{
		return;
	}

	//前フレームの座標を記録
	m_oldPosition = m_position;

	//ガレキの状態で分岐
	switch (m_debrisState)
	{
	case enDrop:
		//地面に落ちている時の挙動
		AsDropBehave();
		break;
	case enBullet:
		//弾として発射されている時の挙動
		AsBulletBehave();
		break;
	case enHold:
		//プレイヤーに保持されている時の挙動
		AsHoldBehave();
		break;
	case enPop:
		//何かに当たった直後の挙動
		AsPopBehave();
		break;
	default:
		MessageBoxA(nullptr, "存在しないガレキの状態です。", "エラー", MB_OK);
		break;
	}

	m_skinModelRender->SetPosition(m_position);

	//Debrisが穴に落ちた時、消す。
	if (m_position.y <= -1000.0f) {
		DeleteGO(this);
	}
}


//地面に落ちている時の挙動
void Debris::AsDropBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool {

		Vector3 toPlayer = player->m_position - m_position;

		//引力の時のみ
		if (player->m_magPower == -1)
		{
			//バーストしてたら引っ張ってくる
			if (toPlayer.Length() > 50 && toPlayer.Length() < 500.0f && player->m_isBurst == true)
			{
				toPlayer.y += 10.0f;
				Vector3 moveDir = toPlayer;
				moveDir.Normalize();

				//x、zそれぞれ別々で測る
				m_position.x += moveDir.x *= 30.0f;
				//壁にぶつかったとき
				Vector3 crossPoint;
				bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.z += moveDir.z *= 30.0f;
				//壁にぶつかったとき
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;
			}

			//近くに弾があれば10発以内なら拾える
			if (toPlayer.Length() < 100.0f && player->m_holdDebrisVector.size() < 10)
			{
				m_parent = player;
				m_debrisState = enHold;

				//プレイヤーの保持するガレキコンテナに格納
				player->m_holdDebrisVector.push_back(this);
				//テキスト更新
				player->m_bulletNumFont->SetText(std::to_wstring(player->m_holdDebrisVector.size()));
				if (player->m_playerNum == 0)
				{
					if (player->m_holdDebrisVector.size() >= 10)
						player->m_bulletNumFont->SetPosition({ -207.0f, -270.0f });

					else
						player->m_bulletNumFont->SetPosition({ -170.0f, -270.0f });
				}
				else
				{
					if (player->m_holdDebrisVector.size() >= 10)
						player->m_bulletNumFont->SetPosition({ 23.0f, -270.0f });
					else
						player->m_bulletNumFont->SetPosition({ 60.0f, -270.0f });
				}
			}
		}

		//斥力の時
		if (player->m_magPower == 1)
		{
			//バーストしてたら引っ張ってくる
			if (toPlayer.Length() > 50 && toPlayer.Length() < 500.0f && player->m_isBurst == true)
			{
				Vector3 moveDir = toPlayer;
				moveDir.y = 0.0f;
				moveDir.Normalize();

				//x、zそれぞれ別々で測る
				m_position.x += moveDir.x *= -30.0f;
				//壁にぶつかったとき
				Vector3 crossPoint;
				bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.z += moveDir.z *= -30.0f;
				//壁にぶつかったとき
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.y += 10.0f;
			}
			
		}
		return true;
		});
	//重力処理
	m_position.y -= 5.0f;
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		m_position = m_oldPosition;
	}
}

//弾として発射されている時の挙動
void Debris::AsBulletBehave()
{
	//プレイヤーとの当たり判定用
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetRadius(60.0f);

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//発射したプレイヤーと違う時
			if (player->m_playerNum != m_parent->m_playerNum)
			{
				//敵プレイヤーが磁力バーストしている時
				if (player->m_isBurst == true)
				{
					Vector3 toPlayer = player->m_magPosition - m_position;

					//敵との距離が500未満なら
					if (toPlayer.Length() < 500.0f)
					{
						//引力なら
						if (player->m_magPower == -1)
						{
							toPlayer.Normalize();
							Vector3 newDirection = m_moveDirection + toPlayer;
							newDirection /= 2;
							newDirection.Normalize();
							m_moveDirection = newDirection;
						}
						else //斥力なら
						{
							toPlayer.Normalize();
							Vector3 newDirection = m_moveDirection - toPlayer;
							newDirection /= 2;
							newDirection.Normalize();
							m_moveDirection = newDirection;
						}
					}
				}

				//移動処理(TODO:撃った弾と違うプレイヤーは1人しかいないので1回しか呼ばれないので大丈夫だが、場所の移動は検討する、
				//その場合、QueryGOsを移動処理と当たり判定処理の2回に分けてすることになるかも)
				m_position += m_moveDirection * m_velocity;

				//移動先の当たり判定を更新
				m_bulletCollider.SetEndPoint(m_position);

				//当たり判定にヒットしているならダメージ。
				if (player->m_collider.isHitCapsule(m_bulletCollider))
				{
					//ヒット音
					prefab::CSoundSource* ssHit = NewGO<prefab::CSoundSource>(0);

					//ガレキの形状でダメージが分岐
					switch (m_debrisShape)
					{
					case enStone:
						//音を再生
						if (player->m_hp > 0) {
							ssHit->Init(L"Assets/sound/ダメージ音.wav");
							ssHit->SetVolume(1.2f);
							ssHit->Play(false);
						}
						player->m_TakeAttackNum++;//攻撃を受けた回数
						player->Damage(50.0f);
						break;
					case enSword:
						//音を再生
						if (player->m_hp > 0) {
							ssHit->Init(L"Assets/sound/剣が当たる.wav");
							ssHit->SetVolume(1.5f);
							ssHit->Play(false);
						}
						player->m_TakeAttackNum++;//攻撃を受けた回数
						player->Damage(100.0f);
						break;
					case enSpecialCharger:
						//音を再生(仮)
						if (player->m_hp > 0) {
							ssHit->Init(L"Assets/sound/剣が当たる.wav");
							ssHit->SetVolume(1.5f);
							ssHit->Play(false);
						}
						player->m_TakeAttackNum++;//攻撃を受けた回数
						player->Damage(20.0f);
						break;
					}
					player->m_damegeEffectFront = m_moveDirection * -1.0f;
					//当たった所からポップさせる
					m_debrisState = enPop;

					//プレイヤーをノックバックさせる。
					player->m_isKnockBack = true;
					player->m_moveSpeed = m_moveDirection * 10.0f;
				}
			}
			return true;
		});


	//ステージとの当たり判定
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		m_position = crossPoint;

		Vector3 moveDir = m_position - m_oldPosition;
		moveDir.Normalize();

		//当たった所より少し手前からポップさせる
		m_position -= moveDir * 30.0f;

		m_debrisState = enPop;
	}
}


//プレイヤーに保持されている時の挙動
void Debris::AsHoldBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//スペシャルチャージャーを持っていると、ゲージが少しずつ溜まる。
			if (m_debrisShape == enSpecialCharger) {
				if (player->m_playerNum == m_parent->m_playerNum) {

					m_specialChargeCount += 1.0f;

					if (m_specialChargeCount == 50.0f) {
						player->ChargeSpecialAttackGauge(1);
						m_specialChargeCount = 0.0f;
					}
				}
			}
		return true;
	});

	//プレイヤーの向きにガレキのモデルも向ける。
	//ガレキの回転クォータニオン
	Quaternion DebrisRot;
	//キャラの向きを取得
	Vector3 CharacterDirection = m_parent->m_toCameraDir * -1.0f;
	//上下方向の向きは無視する。
	CharacterDirection.y = 0.0f;
	CharacterDirection.Normalize();

	//キャラの向きと基準方向AxisZとの角度を求める
	float dot = CharacterDirection.Dot(Vector3::AxisZ);
	float angle = acosf(dot);//アークコサイン
	//角度しか求まらないのでキャラの向きでどちら回りかを決める
	//反時計回りなら正の数　時計回りなら負の数
	if (CharacterDirection.x < 0)
	{
		angle *= -1;
	}
	//Y軸まわりに回転をセット
	DebrisRot.SetRotation(Vector3::AxisY, angle);
	//モデルに回転を適用。
	m_skinModelRender->SetRotation(DebrisRot);
}

//何かに当たった直後の挙動
void Debris::AsPopBehave()
{
	m_position.y -= 10.0f;

	//ステージとの当たり判定
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		//当たった所にドロップさせる
		m_position = m_oldPosition;

		m_debrisState = enDrop;
	}
}