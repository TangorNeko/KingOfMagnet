#include "stdafx.h"
#include "Bomb.h"

#include "Player.h"
#include "BackGround.h"
#include "GameScene.h"

#include "Explosion.h"
#include "Flash.h"
#include "Incendia.h"

namespace
{
	const float BOMB_SPEED = 20.0f;										//爆弾の速度
	const Vector3 BOMB_GRENADE_SCALE = { 0.5f,0.5f,0.5f };				//グレネードのモデルの拡大率
	const Vector3 BOMB_FLASHGRENADE_SCALE = { 0.35f, 0.35f, 0.35f };	//閃光弾のモデルの拡大率
	const Vector3 BOMB_INCENDIARY_SCALE = { 0.25f, 0.25f, 0.25f };		//焼夷弾のモデルの拡大率
	const float BOMB_DESTROY_HEIGHT = -1000.0f;							//爆弾が消滅する高さ
	const float BOMB_BIRST_MOVE_SPEED_X = 30.0f;						//バースト時の移動速度_X
	const float BOMB_BIRST_MOVE_SPEED_Y = 10.0f;						//バースト時の移動速度_Y
	const float BOMB_BIRST_MOVE_SPEED_Z = 30.0f;						//バースト時の移動速度_Z
	const float BOMB_OBTAINABLE_RANGE = 100.0f;							//プレイヤーに拾われる距離
	const float BOMB_GROUND_GRAVITY_SPEED = 10.0f;						//落ちている時の重力
	const float BOMB_COLLISION_RADIUS = 60.0f;							//爆弾の当たり判定の半径
	const float BOMB_FALLING_DIRECTION_MINUS_VALUE = 0.035f;			//放物線を描くためYの速度にマイナスしていく値
	const float BOMB_POP_POSITION_SPACE = 30.0f;						//ステージに当たった時壁に当たらないようにするためずらす距離
	const int BOMB_EXPLOSION_COUNT_EXPLOSION = 20;						//爆弾が爆発するカウント


	//他のクラスでも使いそうな定数
	const int PLAYER_HOLD_BOMB_SIZE_MAX = 3;							//プレイヤーが持てる爆弾の最大数
}
Bomb::~Bomb()
{
	DeleteGO(m_skinModelRender);

	//ゲームシーンの弾の数を減らす。
	if (m_gameScene != nullptr)
	{
		m_gameScene->BulletDecrease();
	}
}

bool Bomb::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->SetShadowCasterFlag(true);

	//爆弾の形状で読み込むモデルを分岐
	switch (m_bombShape)
	{
	case enGrenade:
		m_skinModelRender->Init("Assets/modelData/Grenade.tkm");
		m_skinModelRender->SetScale(BOMB_GRENADE_SCALE);
		m_moveDirection.Normalize();
		break;
	case enFlashGrenade:
		m_skinModelRender->Init("Assets/modelData/FlashGrenade.tkm");
		m_skinModelRender->SetScale(BOMB_FLASHGRENADE_SCALE);
		m_moveDirection.Normalize();
		break;
	case enIncendiaryGrenade:
		m_skinModelRender->Init("Assets/modelData/SmokeGrenade.tkm");
		m_skinModelRender->SetScale(BOMB_INCENDIARY_SCALE);
		m_moveDirection.Normalize();
		break;
	default:
		MessageBoxA(nullptr, "存在しない爆弾の形状です。\n", "エラー", MB_OK);
		break;
	}

	//ステージとの当たり判定用
	m_stageModel = FindGO<BackGround>("background");

	//ゲームシーンを取得
	m_gameScene = FindGO<GameScene>("gamescene");

	return true;
}

void Bomb::Update()
{
	//ポーズ中かリザルトシーンならスキップ。
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause || m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		return;
	}

	//前フレームの座標を記録
	m_oldPosition = m_position;

	//爆弾の状態で分岐
	switch (m_bombState)
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
		MessageBoxA(nullptr, "存在しない爆弾の状態です。", "エラー", MB_OK);
		break;
	}

	m_skinModelRender->SetPosition(m_position);

	//Bombが穴に落ちた時、消す。
	if (m_position.y <= BOMB_DESTROY_HEIGHT) {
		DeleteGO(this);
	}
}


//地面に落ちている時の挙動
void Bomb::AsDropBehave()
{
	QueryGOs<Player>("Player", [this](Player* player)->bool {

		Vector3 toPlayer = player->GetPosition() - m_position;

		//引力の時のみ
		if (player->GetMagnetState() == MAGNETSTATE_GRAVITY)
		{
			//バーストしてたら引っ張ってくる
			if (player->IsBurst() == true && toPlayer.Length() > BURST_AFFECT_RANGE_MIN && toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
			{
				m_isOnGround = false;

				Vector3 moveDir = toPlayer;
				moveDir.Normalize();

				//x、z、yそれぞれ別々で測る
				m_position.x += moveDir.x * BOMB_BIRST_MOVE_SPEED_X;
				//壁にぶつかったとき
				Vector3 crossPoint;
				bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.z += moveDir.z * BOMB_BIRST_MOVE_SPEED_Z;
				//壁にぶつかったとき
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.y += moveDir.y * BOMB_BIRST_MOVE_SPEED_Y;
				//地面にぶつかったとき
				crossPoint;
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;
			}

			//近くに弾があれば3発以内なら拾える
			if (toPlayer.Length() < BOMB_OBTAINABLE_RANGE && player->m_holdBombVector.size() < PLAYER_HOLD_BOMB_SIZE_MAX)
			{
				m_parent = player;
				m_bombState = enHold;

				//プレイヤーの保持する爆弾コンテナに格納
				player->m_holdBombVector.push_back(this);
				//プレイヤーが選択している爆弾をリセット。
				player->m_selectBombNo = 0;
			}
		}

		//斥力の時
		else if (player->GetMagnetState() == MAGNETSTATE_REPULSION)
		{
			//バーストしてたら弾き飛ばす
			if (player->IsBurst() == true && toPlayer.Length() > BURST_AFFECT_RANGE_MIN && toPlayer.Length() < BURST_AFFECT_RANGE_MAX)
			{
				//弾き飛ばすのでプレイヤーへの向きとは反対側
				Vector3 moveDir = toPlayer * -1;

				//y方向にはプレイヤーの位置依存では移動させない(床を抜ける可能性があるので)
				moveDir.y = 0.0f;
				moveDir.Normalize();

				//x、zそれぞれ別々で測る
				m_position.x += moveDir.x * BOMB_BIRST_MOVE_SPEED_X;
				//壁にぶつかったとき
				Vector3 crossPoint;
				bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.z += moveDir.z * BOMB_BIRST_MOVE_SPEED_Z;
				//壁にぶつかったとき
				isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
				if (isHit == true) {
					m_position = m_oldPosition;
				}
				else
					m_oldPosition = m_position;

				m_position.y += BOMB_BIRST_MOVE_SPEED_Y;
			}

		}
		return true;
		});
	//重力処理
	if (m_isOnGround == false)
	{
		m_position.y -= BOMB_GROUND_GRAVITY_SPEED;
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
void Bomb::AsBulletBehave()
{
	//プレイヤーとの当たり判定用
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetRadius(BOMB_COLLISION_RADIUS);

	QueryGOs<Player>("Player", [this](Player* player)->bool
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
				m_position += m_moveDirection * BOMB_SPEED;
				m_moveDirection.y -= BOMB_FALLING_DIRECTION_MINUS_VALUE;
				m_moveDirection.Normalize();

				//移動先の当たり判定を更新
				m_bulletCollider.SetEndPoint(m_position);

				//当たり判定にヒットしているなら起爆。
				if (player->IsBulletHitCollider(m_bulletCollider))
				{
					//当たった所からポップさせる
					m_bombState = enPop;
					m_isOnGround = false;
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
		m_position -= moveDir * BOMB_POP_POSITION_SPACE;

		m_bombState = enPop;
	}
}


//プレイヤーに保持されている時の挙動
void Bomb::AsHoldBehave()
{
	//プレイヤーの向きに爆弾のモデルも向ける。
	//爆弾の回転クォータニオン
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
void Bomb::AsPopBehave()
{
	m_position.y -= BOMB_GROUND_GRAVITY_SPEED;

	//ステージとの当たり判定
	Vector3 crossPoint;
	bool isHit = m_stageModel->isLineHitModel(m_oldPosition, m_position, crossPoint);
	if (isHit == true)
	{
		//当たった所にドロップさせる
		m_position = m_oldPosition;

		//爆弾の種類によって分岐。
		switch(m_bombShape)
		{
			case enGrenade:
			m_explosionCount++;
			if (m_explosionCount >= BOMB_EXPLOSION_COUNT_EXPLOSION) {
				Explosion* explosion = NewGO<Explosion>(0);
				explosion->SetPosition(crossPoint);
				DeleteGO(this);
			}
			break;
			case enFlashGrenade:
			m_explosionCount++;
			if (m_explosionCount >= BOMB_EXPLOSION_COUNT_EXPLOSION) {
				Flash* flash = NewGO<Flash>(0);
				flash->SetPosition(crossPoint);
				flash->SetParentNum(m_parent->GetPlayerNum());
				DeleteGO(this);
			}
			break;
			case enIncendiaryGrenade:
			m_explosionCount++;
			if (m_explosionCount >= BOMB_EXPLOSION_COUNT_EXPLOSION) {
				Incendia* incendia = NewGO<Incendia>(0);
				incendia->SetPosition(crossPoint);
				DeleteGO(this);
			}
			break;
		}
	}
}