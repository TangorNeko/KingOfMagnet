#include "stdafx.h"
#include "Repulsion.h"
#include "Character_base.h"
#include "GameScene.h"

Repulsion::~Repulsion()
{
	DeleteGO(m_skinModelRender);
}
bool Repulsion::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/repulsion.tkm");
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rot);
	m_skinModelRender->SetScale(m_scale);

	m_gameScene = FindGO<GameScene>("gamescene");
	return true;
}
void Repulsion::Update()
{
	/*
	if (m_gameScene->GetGameEndFlag() == true)
	{
		DeleteGO(this);
	}
	*/
	
	QueryGOs<Character_base>("Player", [this](Character_base* player)->bool
		{
			if (player->m_playerNum == 0) {//プレイヤー０				
				//床との距離
				diff0 = player->m_position - m_position;
				//立方体の範囲に入れば斥力を与える
				if ((diff0.x < 200 && diff0.x > -200) &&
					(diff0.z < 200 && diff0.z > -200) &&
					 diff0.y < 200 )//床との距離が近ければ
				{
					//時間が立つに連れスピードが上がる
					m_loop0++;
					if (m_objNum == 0) 
					{
						player->m_Yspeed0.y += (0.0005f * (m_loop0 * m_loop0));
					}
					if (m_objNum == 1)
					{						
						player->m_Yspeed1.y += (0.0005f * (m_loop0 * m_loop0));
					}
					player->m_loop = 0;//エレベーター減少対策

				}	
				else//範囲外は値を０に
				{
					if (m_objNum == 0)
					{
						player->m_Yspeed0.y = 0;						
					}
					if (m_objNum == 1)
					{
						player->m_Yspeed1.y = 0;
					}
					m_loop0 = 0;					
				}
			}
			else if(player->m_playerNum==1)//プレイヤー１
			{
				//床との距離
				diff1 = player->m_position - m_position ;
				//立方体の範囲に入れば斥力を与える
				if ((diff1.x < 200 && diff1.x > -200) &&
					(diff1.z < 200 && diff1.z > -200) &&
					diff1.y < 200)//床との距離が近ければ
				{
					//床に近ければ近いほど加算する値を大きくする
					m_loop1++;
					if (m_objNum == 0)
					{
						player->m_Yspeed0.y += (0.0005f * (m_loop1 * m_loop1));
					}
					if (m_objNum == 1)
					{
						player->m_Yspeed1.y += (0.0005f * (m_loop1 * m_loop1));
					}
					player->m_loop = 0;//エレベーター減少対策

				}
				else
				{
					if (m_objNum == 0)
					{
						player->m_Yspeed0.y = 0;
					}
					if (m_objNum == 1)
					{
						player->m_Yspeed1.y = 0;
					}
					m_loop1 = 0;				
				}
			}
			return true;
		}
	);
		
}