#include "stdafx.h"
#include "UsedGravityGrenade.h"
#include "ShowModel.h"
#include "Character_base.h"
#include "BackGround.h"

UsedGravityGrenade::~UsedGravityGrenade()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_pointLight);
}

bool UsedGravityGrenade::Start()
{
	//引力弾自身のモデルを作成

	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/Gravity.tkm");
	m_skinModelRender->SetScale({ 0.5f, 0.5f, 0.5f });

	//引力弾から出る光
	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 1.0f,1.0f,0.0f });
	m_pointLight->SetRange(200.0f);
	//投げた方向に引力弾を向ける
	m_direction_me = m_moveDirection;
	float t = m_direction_me.Dot(Vector3::AxisZ);

	t = acos(t);
	if (m_direction_me.x < 0) {
		t *= -1;
	}

	m_rot.SetRotation(Vector3::AxisY, t);
	m_skinModelRender->SetRotation(m_rot);

	m_stageModel = FindGO<BackGround>("background");

	return true;
}

void UsedGravityGrenade::Update()
{
	//前フレームの弾の位置を記録。
	Vector3 oldPos = m_position;
	
	if (hitFlag == false) {
		//引力弾の移動処理
		m_position += m_moveDirection * m_velocity;
		m_moveDirection.y -= 1.5f * 1.5f * 0.05f;

		//プレイヤーとの衝突判定用のカプセルの位置を更新。
		m_collider.SetStartPoint(oldPos);
		m_collider.SetEndPoint(m_position);
		m_collider.SetRadius(15.0f);

		hitFlag = m_stageModel->isLineHitModel(oldPos, m_position, crossPoint);
	}

	else {
		m_position = crossPoint;

	}
	

	////各プレイヤーを検索
	//QueryGOs<Character_base>("Player", [this, oldPos](Character_base* player)->bool
	//	{
	//		//発射されてから15フレーム後に、発射したプレイヤーの磁力を与える(加速or減速)
	//		if (m_liveCount == 15 && player->m_playerNum == m_parentNo)
	//		{
	//			m_velocity += 5.0f * player->m_magPower;
	//		}

	//		//弾を発射したプレイヤーと違う場合(敵の場合)
	//		if (player->m_playerNum != m_parentNo)
	//		{
	//			//敵との距離
	//			Vector3 diff = player->m_magPosition - m_position;

	//			//前フレームの位置と移動後の位置を結んだ線が敵プレイヤーの当たり判定の三角形を通っている場合
	//			if (player->m_collider.isHitCapsule(m_collider))
	//			{
	//				//敵プレイヤーに速度に応じてダメージを与える
	//				player->Damage(m_velocity / 2);
	//				DeleteGO(this);
	//			}

	//			//敵との距離が500以内なら
	//			if (diff.Length() < 500.0f)
	//			{

	//				//敵が引力モードなら少し引き寄せる。
	//				if (player->m_magPower < 0)
	//				{
	//					Vector3 toPlayer = diff;
	//					toPlayer.Normalize();
	//					//m_position += toPlayer * player->m_magPower * 3 * -1;
	//					Vector3 newDirection = m_moveDirection * m_velocity + toPlayer * player->m_magPower * 2 * -1;
	//					newDirection.Normalize();
	//					m_moveDirection = newDirection;

	//				}

	//				//発射したプレイヤーの磁力を与える(加速or減速)
	//				if (m_isAffectedFromEnemyPower == false)
	//				{
	//					m_velocity -= player->m_magPower;
	//					m_isAffectedFromEnemyPower = true;
	//				}
	//			}
	//		}

	//		return true;
	//	}
	//);

	//モデルとライトの位置をセット。
	m_skinModelRender->SetPosition(m_position);
	m_pointLight->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rot);
	//100フレーム生存したら消去
	m_liveCount++;
	if (m_liveCount > 100)
	{
		DeleteGO(this);
	}

}