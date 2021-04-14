#include "stdafx.h"
#include "Bullet.h"
#include "ShowModel.h"
#include "Character_base.h"

Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
	//DeleteGO(m_pointLight);
}

bool Bullet::Start()
{ 
	//弾自身のモデルを作成
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/syuriken1.tkm");
	m_skinModelRender->SetScale({ 7.0f, 7.0f, 7.0f });

	//弾から出る光
	/*m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 1.0f,1.0f,0.0f });
	m_pointLight->SetRange(200.0f);*/

	return true;
}

void Bullet::Update()
{
	//前フレームの弾の位置を記録。
	Vector3 oldPos = m_position;

	//弾の移動処理
	m_position += m_moveDirection * m_velocity;

	//プレイヤーとの衝突判定用のカプセルの位置を更新。
	m_collider.SetStartPoint(oldPos);
	m_collider.SetEndPoint(m_position);
	m_collider.SetRadius(15.0f);

	//TODO:後からきちんとした衝突判定は作る。　これはプロトタイプ用　障害物の座標と同じなら弾を消す
	if ((-113 < m_position.x && m_position.x < 105 && 857 < m_position.z && m_position.z < 1103) ||
		(915 < m_position.x && m_position.x < 1075 && -94 < m_position.z && m_position.z < 62) ||
		(-105 < m_position.x && m_position.x < 107 && -1103 < m_position.z && m_position.z < -863) ||
		(-1047 < m_position.x && m_position.x < -920 && -94 < m_position.z && m_position.z < 82))
	{
		DeleteGO(this);
	}

	//各プレイヤーを検索
	QueryGOs<Character_base>("Player", [this,oldPos](Character_base* player)->bool
		{
			//発射されてから15フレーム後に、発射したプレイヤーの磁力を与える(加速or減速)
			if (m_liveCount == 15 && player->m_playerNum == m_parentNo)
			{
				m_velocity += 5.0f * player->m_magPower;
			}

			//弾を発射したプレイヤーと違う場合(敵の場合)
			if (player->m_playerNum != m_parentNo)
			{
				//敵との距離
				Vector3 diff = player->m_magPosition - m_position;

				//前フレームの位置と移動後の位置を結んだ線が敵プレイヤーの当たり判定の三角形を通っている場合
				if (player->m_collider.isHitCapsule(m_collider))
				{
					//敵プレイヤーに速度に応じてダメージを与える
					player->Damage(m_velocity/2);
					DeleteGO(this);
				}

				//敵との距離が500以内なら
				if (diff.Length() < 500.0f)
				{
					//敵が引力モードなら少し引き寄せる。
					if (player->m_magPower < 0)
					{
						Vector3 toPlayer = diff;
						toPlayer.Normalize();
						//m_position += toPlayer * player->m_magPower * 3 * -1;
						Vector3 newDirection = m_moveDirection * m_velocity + toPlayer * player->m_magPower * 2 * -1;
						newDirection.Normalize();
						m_moveDirection = newDirection;
					}

					//発射したプレイヤーの磁力を与える(加速or減速)
					if (m_isAffectedFromEnemyPower == false)
					{
						m_velocity -= player->m_magPower;
						m_isAffectedFromEnemyPower = true;
					}
				}
			}

			return true;
		}
	);

	//モデルとライトの位置をセット。
	m_skinModelRender->SetPosition(m_position);
	//m_pointLight->SetPosition(m_position);

	//100フレーム生存したら消去
	m_liveCount++;
	if (m_liveCount > 100)
	{
		DeleteGO(this);
	}
}