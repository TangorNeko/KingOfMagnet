#include "stdafx.h"
#include "Machinegun_item.h"
#include "Character_base.h"


Machinegun_item::~Machinegun_item()
{
	DeleteGO(m_skinModelRender);
}
bool Machinegun_item::Start()
{
	//���f�����쐬
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	
	m_position.y += 50;
	m_startpositionY = m_position.y;
	m_skinModelRender->Init("Assets/modelData/Gun.tkm");
	m_collider.SetStartPoint(m_position);
	topPos = m_position;
	topPos.y += 50;
	m_collider.SetEndPoint(topPos);
	m_collider.SetRadius(15.0f);
	//m_skinModelRender->SetPosition(m_position);

	return true;
}
void Machinegun_item::Update()
{
	//�e�v���C���[������
	QueryGOs<Character_base>("Player", [this](Character_base* player)->bool
		{
			//�v���C���[���߂����

			if (player->m_collider.isHitCapsule(m_collider))
			{				
				player->m_MachinegunHave = true;
				player->m_weaponModel->Init("Assets/modelData/Gun.tkm");
				DeleteGO(this);
			}
			return true;
		});
	ItemMotion();
}