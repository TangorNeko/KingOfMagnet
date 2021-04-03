#include "stdafx.h"
#include "Have_Machinegun.h"
#include "Character_base.h"
#include "Bullet.h"
Have_Machinegun::~Have_Machinegun()
{

}
bool Have_Machinegun::Start()
{
	QueryGOs<Character_base>("Player", [this](Character_base* player)->bool
	{
			if (m_playerNum == player->m_playerNum)
			{
				player->m_weaponModel->Init("Assets/modelData/Gun.tkm");
				player->m_gunAnimeSelect = true;
			}
			return true;
	});
	return true;
}
void Have_Machinegun::Update()
{
	if (m_deletetime > 0 && m_bulletNum > 0) {
		m_deletetime--;		
		if (g_pad[m_playerNum]->IsTrigger(enButtonRB1) )
		{
			m_bulletNum--;
		}
	}
	else
	{
		QueryGOs<Character_base>("Player", [this](Character_base* player)->bool
			{
				if (m_playerNum == player->m_playerNum)
				{
					player->m_weaponModel->Init("Assets/modelData/Knight_weapon.tkm");//‚»‚ê‚¼‚ê‚Ì•Ší
					player->m_gunAnimeSelect = false;
				}
				DeleteGO(this);
				return true;
			});
	}
}