#include "stdafx.h"
#include "Psychokinesis.h"
#include "Mage.h"
#include "ShowModel.h"
#include "Character_base.h"

Psychokinesis::~Psychokinesis()
{
	DeleteGO(m_skinModelRender[0]);
	DeleteGO(m_skinModelRender[1]);
	if (m_level2)//チャージ33.3以上
	{
		DeleteGO(m_skinModelRender[2]);
		DeleteGO(m_skinModelRender[3]);
	}
	if (m_level3)//チャージ66.6以上
	{
		DeleteGO(m_skinModelRender[4]);
		DeleteGO(m_skinModelRender[5]);
	}
	DeleteGO(m_pointLight);
	//各プレイヤーを検索
	QueryGOs<Mage>("Player", [this](Mage* player)->bool
	{
		//弾を発射したプレイヤーと違う場合(敵の場合)
			if (player->m_playerNum == m_parentNo)
			{
				player->m_Psycho_on = false;
			}
			return true;
	});
}
bool Psychokinesis::Start() // L2R2同時押しした時点で
{	
	for (int i = 0; i < 2; i++)//２つの石を生成
	{
		m_skinModelRender[i] = NewGO<prefab::CSkinModelRender>(0);
		m_skinModelRender[i]->Init("Assets/modelData/MageBullet.tkm");
	}
	for (int i = 0; i < 6; i++)//自分が向いている方向を石に適応
	{
		m_rot.Apply(toPos[i]);
	}
	//弾から出る光
	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 1.0f,1.0f,0.0f });
	m_pointLight->SetRange(200.0f);
	return true;
}
void Psychokinesis::Update()
{
	if (g_pad[m_playerNum]->IsPress(enButtonX) && m_charge > 10)
	{
		m_shot_on = true;//Xを押すと石が飛んでいく
	}
	if (m_shot_on)
	{		
		if (m_level2)//レベル2のとき
		{
			if (m_isOnce2)//一度もNewGOしていなければ
			{
				for (int i = 2; i < 4; i++)
				{
					m_skinModelRender[i] = NewGO<prefab::CSkinModelRender>(0);
					m_skinModelRender[i]->Init("Assets/modelData/MageBullet.tkm");
				}
				m_isOnce2 = false;
			}
		}
		if (m_level3)
		{
			if (m_isOnce3)//一度もNewGOしていなければ
			{
				for (int i = 4; i < 6; i++)
				{
					m_skinModelRender[i] = NewGO<prefab::CSkinModelRender>(0);
					m_skinModelRender[i]->Init("Assets/modelData/MageBullet.tkm");
				}
				m_isOnce3 = false;
			}
		}
		//前フレームの弾の位置を記録。
		for(int i=0;i<6;i++)
		{
			oldPos[i] = m_position[i];
		}		
		//弾の移動処理
		//プレイヤーとの衝突判定用のカプセルの位置を更新。
		for (int i = 0; i < 2; i++)
		{
			m_position[i] += m_moveDirection * m_velocity;
			m_collider[i].SetStartPoint(oldPos[i]);
			m_collider[i].SetEndPoint(m_position[i]);
			m_collider[i].SetRadius(30.0f);
		}
		if (m_level2)
		{
			for (int i = 2; i < 4; i++)
			{
				m_position[i] += m_moveDirection * m_velocity;
				m_collider[i].SetStartPoint(oldPos[i]);
				m_collider[i].SetEndPoint(m_position[i]);
				m_collider[i].SetRadius(30.0f);
			}
		}
		if (m_level3)
		{
			for (int i = 4; i < 6; i++)
			{
				m_position[i] += m_moveDirection * m_velocity;
				m_collider[i].SetStartPoint(oldPos[i]);
				m_collider[i].SetEndPoint(m_position[i]);
				m_collider[i].SetRadius(30.0f);
			}
		}

		//各プレイヤーを検索
		QueryGOs<Mage>("Player", [this](Mage* player)->bool
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
					Vector3 diff[6];
					for (int i = 0; i < 6; i++)
					{
						diff[i] = player->m_magPosition - m_position[i];
						//前フレームの位置と移動後の位置を結んだ線が敵プレイヤーの当たり判定の三角形を通っている場合
						if (player->m_collider.isHitCapsule(m_collider[i]))
						{
							//敵プレイヤーに速度に応じてダメージを与える
							player->Damage((m_velocity * m_chargelevel)/5);
							
							DeleteGO(this);
						}
						//敵との距離が500以内なら
						if (diff[i].Length() < 500.0f)
						{
							//敵が引力モードなら少し引き寄せる。
							if (player->m_magPower < 0)
							{
								Vector3 toPlayer = diff[i];
								toPlayer.Normalize();
								//m_position += toPlayer * player->m_magPower * 4 * -1;
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
				}
				return true;
		});
		//100フレーム生存したら消去
		m_liveCount++;
		if (m_liveCount > 100)
		{
			DeleteGO(this);
		}
		//ライトの位置をセット。
		m_pointLight->SetPosition(m_position[0]);//今は0だけ
		/// 
		/// ここ怪しい
		/// １レベルのときあやしい
		for (int i = 0; i < 2; i++)
		{
			m_skinModelRender[i]->SetPosition(m_position[i]);		
		}
		if (m_level2)
		{
			for (int i = 2; i < 4; i++)
			{
				m_skinModelRender[i]->SetPosition(m_position[i]);
			}
		}
		if (m_level3)
		{
			for (int i = 4; i < 6; i++)
			{
				m_skinModelRender[i]->SetPosition(m_position[i]);				
			}
		}
		
	}
	else//X押していない間
	{
		if (m_level2)
		{
			if (m_isOnce2)//一度もNewGOしていなければ
			{
				for (int i = 2; i < 4; i++)
				{
					m_skinModelRender[i] = NewGO<prefab::CSkinModelRender>(0);
					m_skinModelRender[i]->Init("Assets/modelData/MageBullet.tkm");
				}
				m_isOnce2 = false;
			}
		}
		if (m_level3)
		{
			if (m_isOnce3)//一度もNewGOしていなければ
			{
				for (int i = 4; i < 6; i++)
				{
					m_skinModelRender[i] = NewGO<prefab::CSkinModelRender>(0);
					m_skinModelRender[i]->Init("Assets/modelData/MageBullet.tkm");
				}
				m_isOnce3 = false;
			}
		}
		//Y軸回りに回転するクォータニオンをつくる。
		qRotY.SetRotationDeg(Vector3::AxisY, g_pad[m_playerNum]->GetRStickXF() * 1.5f);

		//クォータニオンを使ってtoPosを回す。
		for (int i = 0; i < 6; i++)
		{
			qRotY.Apply(toPos[i]);

			m_position[i] = toPos[i] + m_playerpos;			
		}
		if (m_Up_On == true)//上に動くフラグ
		{
			if (m_UpDown > 25)//25上に行けば
			{
				m_Up_On = false;//フラグを下ろす
			}
			m_UpDown += 0.5f;//0.5ずつあげる
		}
		else//下げる
		{
			if (m_UpDown < 0)
			{
				m_Up_On = true;
			}
			m_UpDown -= 0.5;
		}
		//y座標にm_UpDownを入れる
		for (int i = 0; i < 6; i++)
		{
			m_position[i].y += m_UpDown;			
		}
		//位置と回転をセットする
		for (int i = 0; i < 2; i++)
		{
			m_skinModelRender[i]->SetPosition(m_position[i]);			
			m_skinModelRender[i]->SetRotation(m_rot);			
		}
		if (m_level2)
		{
			for (int i = 2; i < 4; i++)
			{
				m_skinModelRender[i]->SetPosition(m_position[i]);
				m_skinModelRender[i]->SetRotation(m_rot);
			}
		}
		if (m_level3)
		{
			for (int i = 4; i < 6; i++)
			{
				m_skinModelRender[i]->SetPosition(m_position[i]);
				m_skinModelRender[i]->SetRotation(m_rot);
			}
		}
		//ライトの位置をセット。
		m_pointLight->SetPosition(m_position[0]);//今は0だけ
	}
}
