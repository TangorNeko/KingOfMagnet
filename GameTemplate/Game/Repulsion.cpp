#include "stdafx.h"
#include "Repulsion.h"
#include "Player.h"
#include "GameScene.h"
#include "SampleScene.h"

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

	//m_gameScene = FindGO<GameScene>("gamescene");
	return true;
}
void Repulsion::Update()
{	
	QueryGOs<Player>("Player", [this](Player* player)->bool
		{			
				//°‚Æ‚Ì‹——£
				diff = player->m_position - m_position;
				//—§•û‘Ì‚Ì”ÍˆÍ‚É“ü‚ê‚ÎË—Í‚ğ—^‚¦‚é
				if ((diff.x < 160 && diff.x > -160) &&
					(diff.z < 160 && diff.z > -160) &&
					 diff.y < 200 )//°‚Æ‚Ì‹——£‚ª‹ß‚¯‚ê‚Î
				{

					Vector3 up = Vector3::Up;
					up *= 5;
					player->m_charaCon.Execute(up, 1.0f);
					player->m_fallLoop = 0;

				}	
			return true;
		}
	);
		
}