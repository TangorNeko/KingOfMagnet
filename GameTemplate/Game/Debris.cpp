#include "stdafx.h"
#include "Debris.h"

#include "Player.h"
#include "BackGround.h"
#include "GameScene.h"
#include "Explosion.h"
#include <random>

namespace
{
	const float DEBRIS_SPEED = 50.0f;													//ガレキの弾速
	const int SCRAP_SHAPE_LOTTERY_MOD_VALUE = 3;										//通常弾の形をランダムに決めるための余剰演算値
	const int SCRAP_SHAPE_LOTTERY_PIPE = 0;												//通常弾の形状:パイプ
	const int SCRAP_SHAPE_LOTTERY_GEAR = 1;												//通常弾の形状:歯車
	const int SCRAP_SHAPE_LOTTERY_SCREW = 2;											//通常弾の形状:ネジ
	const Vector3 MODEL_SCRAP_SCALE = { 1.0f,1.0f,1.0f };								//通常弾の拡大率
	const Vector3 MODEL_SWORD_SCALE = { 0.5f,0.5f,0.5f };								//剣の拡大率
	const Vector3 MODEL_SPECIALCHARGER_SCALE = { 0.1f, 0.1f, 0.1f };					//スペシャルチャージャーの拡大率
	const float DEBRIS_DESTROY_HEIGHT = -1000.0f;										//ガレキが消滅する高さ
	const float DEBRIS_BIRST_MOVE_SPEED_X = 30.0f;										//バースト時の移動速度_X
	const float DEBRIS_BIRST_MOVE_SPEED_Y = 10.0f;										//バースト時の移動速度_Y
	const float DEBRIS_BIRST_MOVE_SPEED_Z = 30.0f;										//バースト時の移動速度_Z
	const float DEBRIS_OBTAINABLE_RANGE = 100.0f;										//プレイヤーに拾われる距離
	const float DEBRIS_GROUND_GRAVITY_SPEED = 10.0f;									//落ちている時の重力
	const int FONT_BULLETNUM_DOUBLEDIGIT_BORDER = 10;									//弾の所持数の2桁の境界
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER1_DOUBLEDIGIT = { -207.0f, -270.0f };	//プレイヤー1の弾所持数が2桁だった時のフォントの座標
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER1_SINGLEDIGIT = { -170.0f, -270.0f };	//プレイヤー1の弾所持数が1桁だった時のフォントの座標
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER2_DOUBLEDIGIT = { 23.0f, -270.0f };		//プレイヤー2の弾所持数が2桁だった時のフォントの座標
	const Vector2 FONT_BULLETNUM_POSITION_PLAYER2_SINGLEDIGIT = { 60.0f, -270.0f };		//プレイヤー2の弾所持数が1桁だった時のフォントの座標
	const float DEBRIS_COLLISION_RADIUS = 60.0f;										//ガレキの当たり判定の半径
	const float SOUND_SE_SCRAP_HIT_VOLUME = 1.2f;										//通常弾が当たった時の効果音のボリューム
	const float SOUND_SE_SWORD_HIT_VOLUME = 1.5f;										//剣が当たった時の効果音のボリューム
	const float SOUND_SE_SPECIALCHARGER_HIT_VOLUME = 1.2f;								//スペシャルチャージャーが当たった時の効果音のボリューム
	const int DAMAGE_SCRAP = 80;														//通常弾のダメージ
	const int DAMAGE_SWORD = 120;														//剣のダメージ
	const int DAMAGE_SPECIALCHARGER = 40;												//スペシャルチャージャーのダメージ
	const float DEBRIS_POP_POSITION_SPACE = 30.0f;										//ステージに当たった時壁に埋まらないようにするためずらす距離
	const int SPECIALCHARGER_COUNT_CHARGE = 50;											//スペシャルチャージャーがプレイヤーの必殺技ゲージをチャージする間隔
	const int SPECIALCHARGER_CHARGEVALUE = 2;											//スペシャルチャージャーがプレイヤーの必殺技ゲージをチャージする値

	//他のクラスでも使いそうな定数
	const int PLAYER_HOLD_DEBRIS_SIZE_MAX = 10;											//プレイヤーが持てるガレキの最大数
	const float PLAYER_KNOCKBACK_SPEED = 10.0f;											//プレイヤーのノックバック速度
}
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
	//ガレキのモデルを作成
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->SetShadowCasterFlag(true);
	int random;
	//ガレキの形状で読み込むモデルを分岐
	std::random_device device;
	std::mt19937_64 rnd(device());
	switch (m_debrisShape)
	{
	case enScrap:
		//スクラップのモデル
		random = rnd() % SCRAP_SHAPE_LOTTERY_MOD_VALUE;
		if (random == SCRAP_SHAPE_LOTTERY_PIPE)
		{
			m_skinModelRender->Init("Assets/modelData/pipe.tkm");
		}
		if (random == SCRAP_SHAPE_LOTTERY_GEAR)
		{
			m_skinModelRender->Init("Assets/modelData/gear.tkm");
		}
		if (random == SCRAP_SHAPE_LOTTERY_SCREW)
		{
			m_skinModelRender->Init("Assets/modelData/neji.tkm");
		}
		m_skinModelRender->SetScale(MODEL_SCRAP_SCALE);
		break;
	case enSword:
		//剣のモデル
		m_skinModelRender->Init("Assets/modelData/KnightBullet.tkm");
		m_skinModelRender->SetScale(MODEL_SWORD_SCALE);
		break;
	case enSpecialCharger:
		//スペシャルチャージャーのモデル
		m_skinModelRender->Init("Assets/modelData/Gravity.tkm");
		m_skinModelRender->SetScale(MODEL_SPECIALCHARGER_SCALE);
		break;
	default:
		MessageBoxA(nullptr, "存在しないガレキの形状です。\n", "エラー", MB_OK);
		break;
	}

	//ステージとの当たり判定用
	m_stageModel = FindGO<BackGround>("background");

	//ゲームシーンを取得
	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}

void Debris::Update()
{
	//ポーズ中かリザルトシーンならスキップ。
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause || m_gameScene->GetGameState() == GameScene::GameState::enResult)
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
	if (m_position.y <= DEBRIS_DESTROY_HEIGHT) {
		DeleteGO(this);
	}
}


//地面に落ちている時の挙動
void Debris::AsDropBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool {

		Vector3 toPlayer = player->GetPosition() - m_position;
			//引力の時のみ
			if (player->GetMagnetState() == MAGNETSTATE_GRAVITY)
			{
				//バーストしてたら引っ張ってくる
				if (player->IsBurst() == true && BURST_AFFECT_RANGE_MIN < toPlayer.Length() && toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
				{
					m_isOnGround = false;

					Vector3 moveDir = toPlayer;
					moveDir.Normalize();

					//x、z、yそれぞれ別々で測る
					m_position.x += moveDir.x * DEBRIS_BIRST_MOVE_SPEED_X;
					//壁にぶつかったとき
					Vector3 crossPoint;
					bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;

					m_position.z += moveDir.z * DEBRIS_BIRST_MOVE_SPEED_Z;
					//壁にぶつかったとき
					isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;

					m_position.y += moveDir.y * DEBRIS_BIRST_MOVE_SPEED_Y;
					//地面にぶつかったとき
					crossPoint;
					isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;
				}

				//近くに弾があれば10発以内なら拾える
				if (toPlayer.Length() < DEBRIS_OBTAINABLE_RANGE && player->m_holdDebrisVector.size() < PLAYER_HOLD_DEBRIS_SIZE_MAX)
				{
					m_parent = player;
					m_debrisState = enHold;

					//プレイヤーの保持するガレキコンテナに格納
					player->m_holdDebrisVector.push_back(this);
					//テキスト更新
					if (player->m_bulletNumFont != nullptr)
					{
						player->m_bulletNumFont->SetText(std::to_wstring(player->m_holdDebrisVector.size()));
						if (player->GetPlayerNum() == NUMBER_PLAYER1)
						{
							if (player->m_holdDebrisVector.size() >= FONT_BULLETNUM_DOUBLEDIGIT_BORDER)
								player->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_DOUBLEDIGIT);

							else
								player->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER1_SINGLEDIGIT);
						}
						else
						{
							if (player->m_holdDebrisVector.size() >= FONT_BULLETNUM_DOUBLEDIGIT_BORDER)
								player->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_DOUBLEDIGIT);
							else
								player->m_bulletNumFont->SetPosition(FONT_BULLETNUM_POSITION_PLAYER2_SINGLEDIGIT);
						}
					}
				}
			}

			//斥力の時
			else if (player->GetMagnetState() == MAGNETSTATE_REPULSION)
			{
				//バーストしてたら弾き飛ばす
				if (player->IsBurst() == true && BURST_AFFECT_RANGE_MIN < toPlayer.Length() && toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
				{
					//弾き飛ばすのでプレイヤーへの向きとは反対側
					Vector3 moveDir = toPlayer * -1;

					//y方向にはプレイヤーの位置依存では移動させない(床を抜ける可能性があるので)
					moveDir.y = 0.0f;
					moveDir.Normalize();

					//x、zそれぞれ別々で測る
					m_position.x += moveDir.x * DEBRIS_BIRST_MOVE_SPEED_X;
					//壁にぶつかったとき
					Vector3 crossPoint;
					bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;

					m_position.z += moveDir.z * DEBRIS_BIRST_MOVE_SPEED_Z;
					//壁にぶつかったとき
					isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
					if (isHit == true) {
						m_position = m_oldPosition;
					}
					else
						m_oldPosition = m_position;

					m_position.y += DEBRIS_BIRST_MOVE_SPEED_Y;
				}

			}
		return true;
		});

	//重力処理
	if (m_isOnGround == false)
	{
		m_position.y -= DEBRIS_GROUND_GRAVITY_SPEED;
		Vector3 crossPoint;
		bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
		if (isHit == true)
		{
			m_position = m_oldPosition;
			m_isOnGround = true;
		}
	}
	if (m_position.y != m_oldPosition.y)
	{
		m_isOnGround = false;
	}
}

//弾として発射されている時の挙動
void Debris::AsBulletBehave()
{
	//プレイヤーとの当たり判定用
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetRadius(DEBRIS_COLLISION_RADIUS);

	bool isPlayerHit = false;
	bool isStageHit = false;
	Player* hitPlayer = nullptr;

	QueryGOs<Player>("Player", [this,&isPlayerHit,&hitPlayer](Player* player)->bool
		{
			//発射したプレイヤーと違う時
			if (player->GetPlayerNum() != m_parent->GetPlayerNum())
			{
				//敵プレイヤーが磁力バーストしている時
				if (player->IsBurst() == true)
				{
					Vector3 toPlayer = player->GetMagPosition() - m_position;

					//敵との距離が500未満なら
					if (toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
					{
						//引力なら
						if (player->GetMagnetState() == MAGNETSTATE_GRAVITY)
						{
							//プレイヤーに向かうベクトルと現在の移動方向の平均が新しい移動方向になる
							toPlayer.Normalize();
							Vector3 newDirection = m_moveDirection + toPlayer;
							newDirection /= 2;
							newDirection.Normalize();
							m_moveDirection = newDirection;
						}
						else //斥力なら
						{
							//プレイヤーから離れていくベクトルと現在の移動方向の平均が新しい移動方向になる
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
				m_position += m_moveDirection * DEBRIS_SPEED;

				//移動先の当たり判定を更新
				Vector3 dir = m_position - m_oldPosition;
				dir.Normalize();

				//現在の座標をカプセルの端とする
				Vector3 CapsuleEnd = m_position - dir * DEBRIS_COLLISION_RADIUS;
				m_bulletCollider.SetEndPoint(CapsuleEnd);

				//プレイヤーの当たり判定にヒットしているならヒットフラグ
				if (player->IsBulletHitCollider(m_bulletCollider))
				{
					isPlayerHit = true;

					hitPlayer = player;
				}
			}
			return true;
		});


	//ステージとの当たり判定
	Vector3 stageCrossPoint;
	isStageHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, stageCrossPoint);

	//ステージとプレイヤーに同時ヒットしている時
	if (isPlayerHit == true && isStageHit == true)
	{
		//高さを除いた距離のみ測定
		Vector3 toPlayer = hitPlayer->GetPosition() - m_oldPosition;
		toPlayer.y = 0;
		Vector3 toStage = stageCrossPoint - m_oldPosition;
		toStage.y = 0;

		//距離が近い方にヒット
		if (toPlayer.Length() > toStage.Length())
		{
			StageHitAsBullet(stageCrossPoint);
		}
		else
		{
			PlayerHitAsBullet(hitPlayer);
		}
	}
	else if (isStageHit == true)
	{
		//ステージにのみヒット
		StageHitAsBullet(stageCrossPoint);
	}
	else if (isPlayerHit == true)
	{
		//プレイヤーにのみヒット
		PlayerHitAsBullet(hitPlayer);
	}
}

void Debris::PlayerHitAsBullet(Player* player)
{
	//ガレキの形状でダメージが分岐
	switch (m_debrisShape)
	{
	case enScrap:
		//音を再生
		if (player->GetHP() > 0) {
			SoundOneShotPlay(L"Assets/sound/DamageSE.wav", SOUND_SE_SCRAP_HIT_VOLUME);
		}
		player->Damage(DAMAGE_SCRAP);
		break;
	case enSword:
		//音を再生
		if (player->GetHP() > 0) {
			SoundOneShotPlay(L"Assets/sound/剣が当たる.wav", SOUND_SE_SWORD_HIT_VOLUME);
		}
		player->Damage(DAMAGE_SWORD);
		break;
	case enSpecialCharger:
		//音を再生(仮)
		if (player->GetHP() > 0) {
			SoundOneShotPlay(L"Assets/sound/DamageSE.wav", SOUND_SE_SPECIALCHARGER_HIT_VOLUME);
		}
		player->Damage(DAMAGE_SPECIALCHARGER);
		break;
	}

	//ダメージのエフェクトの正面を設定
	player->SetDamageEffectFront(m_moveDirection * -1);

	//当たった所からポップさせる
	m_debrisState = enPop;
	m_isOnGround = false;

	//プレイヤーをノックバックさせる。
	player->SetKnockBackFlag(true);
	player->SetMoveAmount(m_moveDirection * PLAYER_KNOCKBACK_SPEED);
}

void Debris::StageHitAsBullet(const Vector3& crossPoint)
{
	Vector3 moveDir = m_position - m_oldPosition;
	moveDir.Normalize();

	//当たった所より少し手前からポップさせる
	m_position = crossPoint - (moveDir * DEBRIS_POP_POSITION_SPACE);

	m_debrisState = enPop;
}


//プレイヤーに保持されている時の挙動
void Debris::AsHoldBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//スペシャルチャージャーを持っていると、ゲージが少しずつ溜まる。
			if (m_debrisShape == enSpecialCharger) {
				if (player->GetPlayerNum() == m_parent->GetPlayerNum()) {

					m_specialChargeCount++;

					//カウントが貯まるとプレイヤーの必殺技ゲージ量を増やす。
					if (m_specialChargeCount == SPECIALCHARGER_COUNT_CHARGE) {
						player->ChargeSpecialAttackGauge(SPECIALCHARGER_CHARGEVALUE);
						m_specialChargeCount = 0;
					}
				}
			}
		return true;
	});

	//プレイヤーの向きにガレキのモデルも向ける。
	//ガレキの回転クォータニオン
	Quaternion DebrisRot;
	//キャラの向きを取得
	Vector3 CharacterDirection = m_parent->GetCameraFront();
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
	m_position.y -= DEBRIS_GROUND_GRAVITY_SPEED;

	//ステージとの当たり判定
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		//当たった場合、移動前のフレームにいた場所にドロップさせる
		m_position = m_oldPosition;
		m_debrisState = enDrop;
	}
}
