#include "stdafx.h"
#include "GravityBullet.h"

#include "Player.h"
#include "BackGround.h"
#include "SampleScene.h"
#include "Debris.h"

GravityBullet::~GravityBullet()
{
	DeleteGO(m_effect);

	DeleteGO(m_effect2);
	DeleteGO(m_wearingEffect);
	DeleteGO(m_inflateEffect);
}

bool GravityBullet::Start()
{
	OutputDebugStringA("beforeStart\n");

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Gravity2.tkm");
	m_skinModelRender->SetScale({ 0.05f,0.05f,0.05f, });

	m_stageModel = FindGO<BackGround>("background");

	m_gameScene = FindGO<SampleScene>("gamescene");

	//エフェクト
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Init(u"Assets/effect/引力弾.efk");
	m_effect->SetScale({ 25.0f, 25.0f, 25.0f });

	m_effect2 = NewGO<prefab::CEffect>(0);
	m_effect2->Init(u"Assets/effect/引力弾.efk");
	m_effect2->SetScale({ 25.0f, 25.0f, 25.0f });

	m_inflateEffect = NewGO<prefab::CEffect>(0);
	m_inflateEffect->Init(u"Assets/effect/引力弾2.efk");
	m_inflateEffect->SetScale({ 25.0f, 25.0f, 25.0f });

	m_wearingEffect = NewGO<prefab::CEffect>(0);
	m_wearingEffect->Init(u"Assets/effect/Blackhole3.efk");
	m_wearingEffect->SetScale({ 25.0f, 25.0f, 25.0f });
	m_wearingEffect->SetPosition(m_position);
	m_wearingEffect->Play();

	OutputDebugStringA("afterStart\n");

	return true;
}

void GravityBullet::Update()
{
	OutputDebugStringA("beforeUpdate\n");

	//ポーズ中ならスキップ。
	if (m_gameScene->GetGameState() == SampleScene::GameState::enPause)
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

	OutputDebugStringA("afterUpdate\n");
}

void GravityBullet::AsBulletBehave()
{
	OutputDebugStringA("beforeBulletBehave\n");
	//移動処理。
	m_position += m_moveDirection * m_velocity;

	//プレイヤーとの当たり判定用
	m_bulletCollider.SetStartPoint(m_oldPosition);
	m_bulletCollider.SetEndPoint(m_position);
	m_bulletCollider.SetRadius(30.0f);

	QueryGOs<Player>("Player", [this](Player* player)->bool
		{
			//発射したプレイヤーと違う時
			if (player->m_playerNum != m_parent->m_playerNum)
			{
				//当たり判定にヒットしているなら爆発。
				if (player->m_collider.isHitCapsule(m_bulletCollider))
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
	angle += 0.2f;
	if (angle >= 360.0f)
		angle = 0.0f;
	Quaternion qRot;
	qRot.SetRotation(Vector3::AxisY, angle);
	m_skinModelRender->SetRotation(qRot);
	m_skinModelRender->SetPosition(m_position);

	OutputDebugStringA("afterBulletBehave\n");
}

void GravityBullet::AsExplodeBehave()
{
	OutputDebugStringA("beforeExplodeBehave\n");

	//周囲のガレキを浮かせるモードに。
	QueryGOs<Debris>("debris", [this](Debris* debris)->bool
		{
			Vector3 diff = m_position - debris->m_position;

			if (diff.Length() < 400.0f && debris->m_debrisState == Debris::enDrop)
			{
				//HACK:浮いてる途中に拾われないかつダメージを直で受けないようにPopにしている。
				//正直あんまり良いとは思わない DebrisのStateを増やす?
				debris->m_debrisState = Debris::enPop;

				//浮かせたガレキを後ほど攻撃状態に移行させるためにコンテナに格納。
				m_controlDebrisVector.push_back(debris);
			}

			return true;
		});

	//爆発の瞬間にエフェクトを発生させる。
	m_effect->SetPosition(m_position);
	m_effect->Play();
	m_inflateEffect->SetPosition(m_position);
	m_inflateEffect->Play();

	//弾が纏うエフェクトをストップ
	m_wearingEffect->Stop();
	//弾のモデルは削除
	DeleteGO(m_skinModelRender);

	//音を鳴らす
	prefab::CSoundSource* ssGravity = NewGO<prefab::CSoundSource>(0);
	ssGravity->Init(L"Assets/sound/暗黒魔法.wav", SoundType::enSE);
	ssGravity->SetVolume(1.5f);
	ssGravity->Play(false);

	//爆発したので引力を発生させる状態へ
	m_gravityBulletState = enGravity;

	OutputDebugStringA("afterExplodeBehave\n");
}

void GravityBullet::AsGravityBehave()
{
	OutputDebugStringA("beforeGravityBehave\n");

	//HACK:Pop状態では常にm_position.yがマイナス10され続けているので
	//毎フレーム+11することで1ずつ浮く形になっている。
	//またenPop状態でm_position.yがマイナス10された時地形に当たっていたら
	//Drop状態に移行するため毎フレームPopにしている。処理が汚い。
	//TODO:後から直す。
	for (auto debris : m_controlDebrisVector)
	{
		debris->m_debrisState = Debris::enPop;
		debris->m_position.y += 11.0f;
	}

	//引力状態のカウンターを進める
	m_gravityTimeCount++;

	//カウンターが180以上か、プレイヤーから攻撃指示が出たらフィニッシュ状態へ移行。
	if (m_gravityTimeCount >= 180 || m_parent->m_isGravityBulletAttack == true)
	{
		m_gravityBulletState = enFinish;
	}
	//爆発してからプレイヤーを吸い寄せ始めるまでに少しタイムラグを設ける。
	else if (m_gravityTimeCount >= 30)
	{
		QueryGOs<Player>("Player", [this](Player* player)->bool
			{
				Vector3 diff = m_position - player->m_position;
				if (diff.Length() > 60.0f &&	//近すぎてもダメ
					diff.Length() < 400.0f && player != m_parent)
				{
					//Y軸も吸い寄せると床抜けすることがあるのでy軸を除く。
					Vector3 toGravity = diff;
					toGravity.y = 0.0f;
					toGravity.Normalize();

					//吸い寄せスピード分乗算
					toGravity *= m_bacuumSpeed;

					//敵プレイヤーのキャラコンに実行させる
					//正直このやり方も良いといえるのか分からない。
					player->m_charaCon.Execute(toGravity, 1.0f);
				}
				return true;
			});
	}
	if (m_gravityTimeCount % 60 == 0) 
	{
		m_effect2->SetPosition(m_position);
		m_effect2->Play();
	}
	else if (m_gravityTimeCount % 30 == 0)
	{
		m_effect->SetPosition(m_position);
		m_effect->Play();
	}

	OutputDebugStringA("afterGravityBehave\n");
}

void GravityBullet::AsFinishBehave()
{
	OutputDebugStringA("beforeFinishBehave\n");
	//浮かせたガレキに攻撃を指示。
	for (auto debris : m_controlDebrisVector)
	{
		//ガレキの状態をBulletに
		debris->m_debrisState = Debris::enBullet;

		//どちらのプレイヤーの攻撃かをガレキに渡す。
		debris->m_parent = m_parent;

		//攻撃先は引力弾の中心(敵プレイヤーの位置に飛ぶわけではない)
		Vector3 toGravity = m_position - debris->m_position;
		toGravity.Normalize();

		//ガレキに移動方向を指定。
		debris->m_moveDirection = toGravity;
	}

	//フィニッシュ攻撃を指示したので引力弾としての役目は終わり。
	DeleteGO(this);

	OutputDebugStringA("afterFinishBehave\n");
}
