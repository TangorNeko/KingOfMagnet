#include "stdafx.h"
#include "GravityBullet.h"

#include "Player.h"
#include "BackGround.h"
#include "GameScene.h"
#include "Debris.h"

namespace
{
	const Vector3 MODEL_GRAVITY_SCALE = { 0.05f,0.05f,0.05f, };	//引力弾のモデルの拡大率
	const float MODEL_GRAVITY_ROTATE_ANGLE = 0.2f;				//引力弾のモデルの回転量
	const float SOUND_SE_GRAVITY_VOLUME = 1.5f;					//引力の音のボリューム
	const Vector3 EFFECT_GRAVITY_SCALE = { 25.0f,25.0f,25.0f };	//引力のエフェクトの拡大率
	const float GRAVITYBULLET_COLLISION_RADIUS = 30.0f;			//引力弾の当たり判定の半径
	const float GRAVITYBULLET_GRAVITY_RANGE_MIN = 60.0f;		//引力弾の引力の最小範囲
	const float GRAVITYBULLET_GRAVITY_RANGE_MAX = 400.0f;		//引力弾の引力の最大範囲
	const float GRAVITYBULLET_FLOAT_DEBRIS_SPEED = 11.0f;		//引力弾がガレキを浮かせる速度
	const float GRAVITYBULLET_SPEED = 30.0f;					//引力弾の速度
	const float GRAVITYBULLET_BACUUMSPEED = 6.5f;				//引力弾の引き寄せ速度
	const int GRAVITYATTACKCOUNT_ATTACK = 180;					//引力弾がガレキを発射するカウント
	const int GRAVITYATTACKCOUNT_BACUUM = 30;					//引力弾がプレイヤーを引き寄せる吸うんと
	const int EFFECT_GRAVITY1_INTERVAL = 30;					//引力エフェクト1の再生間隔
	const int EFFECT_GRAVITY2_INTERVAL = 60;					//引力エフェクト2の再生間隔
	const int EFFECT_GRAVITY_PLAY = 0;							//引力エフェクトの再生タイマー
}

GravityBullet::~GravityBullet()
{
	DeleteGO(m_gravityEffect);
	DeleteGO(m_gravityEffect2);
	DeleteGO(m_wearingEffect);
	DeleteGO(m_inflateEffect);
}

bool GravityBullet::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Gravity2.tkm");
	m_skinModelRender->SetScale(MODEL_GRAVITY_SCALE);

	m_stageModel = FindGO<BackGround>("background");

	m_gameScene = FindGO<GameScene>("gamescene");

	//エフェクト
	m_gravityEffect = NewGO<prefab::CEffect>(0);
	m_gravityEffect->Init(u"Assets/effect/GravityBullet1.efk");
	m_gravityEffect->SetScale(EFFECT_GRAVITY_SCALE);

	m_gravityEffect2 = NewGO<prefab::CEffect>(0);
	m_gravityEffect2->Init(u"Assets/effect/GravityBullet1.efk");
	m_gravityEffect2->SetScale(EFFECT_GRAVITY_SCALE);

	m_inflateEffect = NewGO<prefab::CEffect>(0);
	m_inflateEffect->Init(u"Assets/effect/GravityBullet2.efk");
	m_inflateEffect->SetScale(EFFECT_GRAVITY_SCALE);

	m_wearingEffect = NewGO<prefab::CEffect>(0);
	m_wearingEffect->Init(u"Assets/effect/Blackhole3.efk");
	m_wearingEffect->SetScale(EFFECT_GRAVITY_SCALE);
	m_wearingEffect->SetPosition(m_position);
	m_wearingEffect->Play();

	return true;
}

void GravityBullet::Update()
{
	//ポーズ中かリザルトシーンならスキップ。
	if (m_gameScene->GetGameState() == GameScene::GameState::enPause || m_gameScene->GetGameState() == GameScene::GameState::enResult)
	{
		return;
	}

	//前フレームの座標を記録
	m_oldPosition = m_position;

	switch (m_gravityBulletState)
	{
	case enBullet:
		//発射されている途中の挙動
		AsBulletBehave();
		break;
	case enExplode:
		//着弾した瞬間の挙動
		AsExplodeBehave();
		break;
	case enGravity:
		//着弾した後の挙動
		AsGravityBehave();
		break;
	case enFinish:
		//フィニッシュ攻撃中の挙動
		AsFinishBehave();
		break;
	}
}

void GravityBullet::AsBulletBehave()
{
	//移動処理。
	m_position += m_moveDirection * GRAVITYBULLET_SPEED;

	//プレイヤーとの当たり判定用
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetEndPoint(m_position);
	m_bulletCollider.SetRadius(GRAVITYBULLET_COLLISION_RADIUS);

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//発射したプレイヤーと違う時
			if (player->GetPlayerNum() != m_parent->GetPlayerNum())
			{
				//当たり判定にヒットしているなら爆発。
				if (player->IsBulletHitCollider(m_bulletCollider))
				{
					m_gravityBulletState = enExplode;
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

		//ステージのどこかに当たったので爆発。
		m_gravityBulletState = enExplode;
	}

	//弾が纏うエフェクトのポジション
	m_wearingEffect->SetPosition(m_position);

	//弾を回転させる。
	angle += MODEL_GRAVITY_ROTATE_ANGLE;
	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}

	Quaternion qRot;
	qRot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(qRot);
	m_skinModelRender->SetPosition(m_position);
}

void GravityBullet::AsExplodeBehave()
{
	//周囲のガレキを浮かせるモードに。
	QueryGOs<Debris>("debris", [this](Debris* debris)->bool
		{
			Vector3 diff = m_position - debris->GetPosition();

			if (diff.Length() < GRAVITYBULLET_GRAVITY_RANGE_MAX && debris->GetDebrisState() == Debris::enDrop)
			{
				//HACK:浮いてる途中に拾われないかつダメージを直で受けないようにPopにしている。
				//正直あんまり良いとは思わない DebrisのStateを増やす?
				debris->SetDebrisState(Debris::enPop);

				//浮かせたガレキを後ほど攻撃状態に移行させるためにコンテナに格納。
				m_controlDebrisVector.push_back(debris);
			}

			return true;
		});

	//爆発の瞬間にエフェクトを発生させる。
	m_gravityEffect->SetPosition(m_position);
	m_gravityEffect->Play();
	m_inflateEffect->SetPosition(m_position);
	m_inflateEffect->Play();

	//弾が纏うエフェクトをストップ
	m_wearingEffect->Stop();
	//弾のモデルは削除
	DeleteGO(m_skinModelRender);

	//音を鳴らす
	SoundOneShotPlay(L"Assets/sound/GravityExplosion.wav", SOUND_SE_GRAVITY_VOLUME);

	//爆発したので引力を発生させる状態へ
	m_gravityBulletState = enGravity;
}

void GravityBullet::AsGravityBehave()
{
	//HACK:Pop状態では常にm_position.yがマイナス10され続けているので
	//毎フレーム+11することで1ずつ浮く形になっている。
	//またenPop状態でm_position.yがマイナス10された時地形に当たっていたら
	//Drop状態に移行するため毎フレームPopにしている。処理が汚い。
	for (auto debris : m_controlDebrisVector)
	{
		debris->SetDebrisState(Debris::enPop);
		Vector3 position = debris->GetPosition();
		position.y += GRAVITYBULLET_FLOAT_DEBRIS_SPEED;
		debris->SetPosition(position);
	}

	//引力状態のカウンターを進める
	m_gravityTimeCount++;

	//カウンターが180以上か、プレイヤーから攻撃指示が出たらフィニッシュ状態へ移行。
	if (m_gravityTimeCount >= GRAVITYATTACKCOUNT_ATTACK || m_parent->GetGravityAttackFlag() == true)
	{
		m_gravityBulletState = enFinish;
	}
	//爆発してからプレイヤーを吸い寄せ始めるまでに少しタイムラグを設ける。
	else if (m_gravityTimeCount >= GRAVITYATTACKCOUNT_BACUUM)
	{
		QueryGOs<Player>("Player", [this](Player* player)->bool
			{
				Vector3 diff = m_position - player->GetPosition();
				if (diff.Length() > GRAVITYBULLET_GRAVITY_RANGE_MIN &&	//近すぎてもダメ
					diff.Length() < GRAVITYBULLET_GRAVITY_RANGE_MAX && player != m_parent)
				{
					//Y軸も吸い寄せると床抜けすることがあるのでy軸を除く。
					Vector3 toGravity = diff;
					toGravity.y = 0.0f;
					toGravity.Normalize();

					//吸い寄せスピード分乗算
					toGravity *= GRAVITYBULLET_BACUUMSPEED;

					//敵プレイヤーのキャラコンに実行させる
					//正直このやり方も良いといえるのか分からない。
					player->ExecuteCharacon(toGravity);
				}
				return true;
			});
	}
	if (m_gravityTimeCount % EFFECT_GRAVITY2_INTERVAL == EFFECT_GRAVITY_PLAY)
	{
		m_gravityEffect2->SetPosition(m_position);
		m_gravityEffect2->Play();
	}
	else if (m_gravityTimeCount % EFFECT_GRAVITY1_INTERVAL == EFFECT_GRAVITY_PLAY)
	{
		m_gravityEffect->SetPosition(m_position);
		m_gravityEffect->Play();
	}
}

void GravityBullet::AsFinishBehave()
{
	//浮かせたガレキに攻撃を指示。
	for (auto debris : m_controlDebrisVector)
	{
		//ガレキの状態をBulletに
		debris->SetDebrisState(Debris::enBullet);

		//どちらのプレイヤーの攻撃かをガレキに渡す。
		debris->SetParent(m_parent);

		//攻撃先は引力弾の中心(敵プレイヤーの位置に飛ぶわけではない)
		Vector3 toGravity = m_position - debris->GetPosition();
		toGravity.Normalize();

		//ガレキに移動方向を指定。
		debris->SetMoveDirection(toGravity);
	}

	//フィニッシュ攻撃を指示したので引力弾としての役目は終わり。
	DeleteGO(this);
}
